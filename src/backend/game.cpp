#include <vector>
#include <algorithm>
#include <cassert>
#include <variant>

#include "game.hpp"



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

void Game::end_turn(int team_id) {
    Team& team = get_team_by_id(team_id);
    //loop until no more turns left, moving the unit and executing actions
    while (const std::optional<Turn> turn = team.dequeue_turn()) {
        //Placeholder for when this function is implemented
        //move_unit(turn->unit, turn->unit_origin, turn->movement_destination);
        Turn tTurn = *std::move(turn);
        std::visit(visitor_, tTurn.action);
    }

}
