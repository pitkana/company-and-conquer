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
    std::vector<Unit*> units(get_unit_amount());
    for (Team& team : get_teams()) {
        for (Unit& unit : team.get_units()) {
            units.push_back(&unit);
        }
    }

    return units;
}

std::unordered_map<int, std::vector<Unit>*> Game::get_units_map() {
    std::unordered_map<int, std::vector<Unit>*> units_map;
    for (Team& team : get_teams()) {
        units_map[team.get_id()] = &team.get_units();
    }

    return units_map;
}

void Game::add_turn(Turn turn, int team_id) {
    Team& team = get_team_by_id(team_id);

    // Execute the movement of the turn instantly
    execute_turn_movement(turn);

    // Only execute the action instantly if it's not random, otherwise it gets executed at the end of the turn when it cant be undone
    if (!turn.action->contains_randomness()) {
        execute_turn_action(turn);
    }

    team.enqueue_turn(std::move(turn));
}

void Game::execute_turn_movement(Turn& turn) {
    map_.move_unit(turn.unit_origin, turn.movement_destination);
}

void Game::execute_turn_action(Turn& turn) {
    //Action is always executed after movement
    turn.action->execute(*this, turn.movement_destination);
}

void Game::undo_turn(int team_id) {
    Team& team = get_team_by_id(team_id);

    std::optional<Turn> turn = team.undo_turn();
    // If no turns were queued, return early
    if (!turn.has_value()) {
        return;
    }
    map_.move_unit(turn->movement_destination, turn->unit_origin);
    turn->action->undo(*this);
}

void Game::end_team_turns(int team_id) {
    Team& team = get_team_by_id(team_id);
    //loop until no more turns left, moving the unit and executing actions
    while (std::optional<Turn> turn = team.dequeue_turn()) {
        execute_turn_action(*turn);
    }
}

Map& Game::get_map() {
    return map_;
}

const Map& Game::get_map() const {
    return map_;
}

