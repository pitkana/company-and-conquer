#pragma once

#include <memory>
#include <variant>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "map.hpp"
#include "team.hpp"
#include "unit.hpp"
#include "map_builder.hpp"

class Action;

//Game class that instances the playing of a level
class Game {
public:

    Game(size_t map_height, size_t map_width): map_(map_height, map_width) {}

    //Add team to teams_
    inline void add_team(Team team) {
        teams_.push_back(team);
    };

    //return reference to teams_ vector
    [[nodiscard]]
    inline std::vector<Team>& get_teams() {
        return teams_;
    }

    [[nodiscard]]
    inline const std::vector<Team>& get_teams() const {
        return teams_;
    }

    //return specific team identified by its id
    [[nodiscard]]
    Team& get_team_by_id(int team_id);

    [[nodiscard]]
    int get_unit_amount() const;

    //return vector of pointers to units in all the teams. Pointers since you can't have vector of references.
    //Ownership of units is still in each Team, care for dangling pointers etc. Units should however live as long as Game does (unless we plan to remove them on death?)
    [[nodiscard]]
    std::vector<Unit*> get_units();


    //return all units as values in an unordered_map, keys being their team's id
    //pointers since you cant store references in map
    [[nodiscard]]
    std::unordered_map<int, std::vector<Unit>*> get_units_map();

    void execute_action(std::shared_ptr<Action> action);

    //End the turn, thus executing all of the selected actions
    void end_turn(int team_id);

    Map& get_map();
    const Map& get_map() const;

private:
    std::vector<Team> teams_;
    Map map_;

};
