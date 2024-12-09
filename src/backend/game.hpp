#pragma once

#include <memory>
#include <variant>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

#include "map.hpp"
#include "team.hpp"
#include "unit.hpp"
#include "map_builder.hpp"

class Action;

//Game class that instances the playing of a level
class Game {
public:

    Game(size_t map_height, size_t map_width) : map_(map_height, map_width) {}

    Game(Map& map): map_(map) {}

    //Add team to teams_
    inline void add_team(Team team) {
        teams_.push_back(std::move(team));
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

    [[nodiscard]]
    Unit* get_unit(int id);

    [[nodiscard]]
    int get_unit_team_id(int unit_id) const;

    [[nodiscard]]
    coordinates<size_t> get_unit_location(int id);

    /**
     * @brief Used map_ to calculate all visible coords for the active team.
     * This method will be used by rendering classes.
     * 
     * @param team Active team.
     * 
     * @returns Visibles coords in a vector.
     */
    std::vector<coordinates<size_t>> get_visible_tiles(const Team& team);
    /**
     * @brief Adds an action to specific team's action queue
     *
     * @param action The action to be added
     * @param team_id The team id that the action gets added to
     * @return True or false if adding action was succesful.
     */
    bool add_action(std::shared_ptr<Action> action, int team_id);
    
    //return all units as values in an unordered_map, keys being their team's id
    //pointers since you cant store references in map
    [[nodiscard]]
    std::unordered_map<int, std::vector<Unit>*> get_units_map();

    /**
     * @brief Executes the action given as parameter
     *
     * @param action the action that gets executed
     * @return void
     */
    void execute_action(std::shared_ptr<Action> action);

    void undo_action(int team_id);

    //End the turn, thus executing all of the selected actions
    void end_team_turns(int team_id);

    Map& get_map();
    const Map& get_map() const;

    std::string get_output() const;

    std::stringstream& get_output_stream();

    void clear_output();

    //Turn handlers.

    /**
     * @brief Initiates the first turn.
     * 
     * @returns True if successful. False if game_ does not contain teams.
     */
    bool init_game();

    /**
     * @returns True if init_game has been called succesfully.
     */
    bool game_started() const;

    /**
     * @brief Ends turn, executes all action and gives the turn to the next team.
     */
    void next_turn();

    Team* get_active_team();

private:
    std::vector<Team> teams_;
    Map map_;
    std::stringstream output_;
    int active_team_idx_ = -1;

    /**
     * @returns Increments active_team_it_. If end then jump to begin.
     */
    void next_team();
};
