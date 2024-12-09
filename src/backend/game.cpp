#include <vector>
#include <algorithm>
#include <cassert>
#include <variant>

#include "game.hpp"
#include "map.hpp"
#include "action.hpp"
#include "team.hpp"

Team& Game::get_team_by_id(int team_id) {
    //find the specified team by its team_id using std::find_if
    auto team_it = std::find_if(teams_.begin(), teams_.end(), 
                            [team_id](const Team& team) {
                                  return team.get_id() == team_id;
                            });

    //assert should never fail
    assert(team_it != teams_.end());
    return *team_it;
}

int Game::get_unit_amount() const {
    int sum = 0;
    for (const Team& team : get_teams()) {
        sum += team.team_size();
    }

    return sum;
}

std::vector<Unit*> Game::get_units() {
    std::vector<Unit*> units;
    units.reserve(get_unit_amount());

    for (Team& team : get_teams()) {
        for (Unit& unit : team.get_units()) {
            units.push_back(&unit);
        }
    }

    return units;
}

Unit* Game::get_unit(int id) {
    std::vector<Unit*> all_units = get_units();
    auto it = std::find_if(all_units.begin(), all_units.end(), [id](Unit* unit) {
        return unit->get_id() == id;
    });

    assert(it != all_units.end() && "Specified unit does not exist");
    return *it;
}

int Game::get_unit_team_id(int unit_id) const {
    for (const Team& team : teams_) {
        const std::vector<Unit>& team_units = team.get_units();

        // Search for unit with given ID inside of each team
        auto it = std::find_if(team_units.begin(), team_units.end(), [unit_id](const Unit& unit) {
            return unit.get_id() == unit_id;
        });

        //If unit is found, return team's id
        if (it == team_units.end()) continue;
        return team.get_id();
    }

    assert(false && "Unit with specified ID does not exist in this game");
}

coordinates<size_t> Game::get_unit_location(int id) {
    return map_.get_unit_location(get_unit(id));
}

std::unordered_map<int, std::vector<Unit>*> Game::get_units_map() {
    std::unordered_map<int, std::vector<Unit>*> units_map;
    for (Team& team : get_teams()) {
        units_map[team.get_id()] = &team.get_units();
    }

    return units_map;
}

void Game::update_visible_tiles() {
    std::vector<Unit>& active_units = teams_[active_team_idx_].get_units();
    std::vector<coordinates<size_t>> visible_coords_vec;
    for (auto& unit : active_units) {
        coordinates<size_t> unit_location = map_.get_unit_location(&unit);
        std::vector<coordinates<size_t>> visible_coords_unit = map_.tiles_unit_sees(unit_location,5);
        visible_coords_vec.insert(visible_coords_vec.end(),visible_coords_unit.begin(),visible_coords_unit.end());
    }
    std::sort(visible_coords_vec.begin(),visible_coords_vec.end());
    std::unique(visible_coords_vec.begin(),visible_coords_vec.end());
    visible_coords = visible_coords_vec;
}

std::vector<coordinates<size_t>> Game::get_visible_tiles() {
    return visible_coords;
}

bool Game::add_action(std::shared_ptr<Action> action, int team_id) {
    Unit& executing_unit = action->get_unit();

    // Check if the unit has already performed this kind of action in this turn
    if (action->is_movement()) {
        if (executing_unit.has_moved) return false;
        executing_unit.has_moved = true;
    } else {
        if (executing_unit.has_added_action) return false;
        executing_unit.has_added_action = true;
    }

    Team& team = get_team_by_id(team_id);
    // Only execute the action instantly if it's not random, otherwise it gets executed at the end of the turn when it cant be undone
    if (!action->contains_randomness()) {
        execute_action(action);
    }
    team.enqueue_action(std::move(action));
    update_visible_tiles();
    return true;
}


void Game::execute_action(std::shared_ptr<Action> action) {
    if (action == nullptr) return;
    action->execute(*this, get_unit_location(action->get_unit().get_id()));
}

void Game::undo_action(int team_id) {
    Team& team = get_team_by_id(team_id);

    std::shared_ptr<Action> action = team.undo_action();
    // If no actions were queued, return early
    if (action == nullptr) {
        return;
    }

    Unit& executing_unit = action->get_unit();

    // Reset the units flags for performing this action when its undone
    if (action->is_movement()) {
        executing_unit.has_moved = false;
    } else {
        executing_unit.has_added_action = false;
    }
    update_visible_tiles();
    action->undo(*this);
}

void Game::end_team_turns(int team_id) {
    Team& team = get_team_by_id(team_id);
    //loop until no more turns left, moving the unit and executing actions
    while (std::shared_ptr<Action> action = team.dequeue_action()) {
        execute_action(action);
    }

    // Reset the whole team's action flags since their turn is over
    for (Unit& unit : get_team_by_id(team_id).get_units()) {
        unit.clear_action_flags();
    }
}


Map& Game::get_map() {
    return map_;
}

const Map& Game::get_map() const {
    return map_;
}

std::string Game::get_output() const {
    return std::string(output_.str());
}

std::stringstream& Game::get_output_stream() {
    return output_;
}

void Game::clear_output() {
    output_.str("");
    output_.clear();
}

bool Game::init_game() {
    output_ << "Initiating game\n";
    next_team();
    update_visible_tiles();
    bool valid_team = active_team_idx_ != -1;
    if (valid_team) output_ << "Team " << teams_[active_team_idx_].get_id() << " turn\n";
    return valid_team;
}

bool Game::game_started() const { return active_team_idx_ > -1; }

void Game::next_turn() {
    if (!game_started()) return; 
    end_team_turns(teams_[active_team_idx_].get_id());
    next_team();
    if (active_team_idx_ == -1) return;
    output_ << "Team " << teams_[active_team_idx_].get_id() << " turn\n";
    update_visible_tiles();
}

void Game::next_team() {
    if (teams_.size() < 1) return;

    if (!game_started()) {
        active_team_idx_ = 0;
        return;
    }

    active_team_idx_++;
    active_team_idx_ = (active_team_idx_ >= int(teams_.size())) ? 0 : active_team_idx_;
    return;
}

Team* Game::get_active_team() {
    return (game_started()) ? &teams_[active_team_idx_] : nullptr;
}
