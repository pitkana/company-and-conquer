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

