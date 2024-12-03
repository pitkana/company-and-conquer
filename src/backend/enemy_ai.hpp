#pragma once
#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <array>
#include <cmath>

#include "coordinates.hpp"
#include "map.hpp"
#include "helper_tools.hpp"

class Game;
class Team;
class Turn;
class Unit;
class Action;

class EnemyAI {
public:
    /**
     * @brief constructor for the EnemyAI class
     *
     * @param game the game in which this AI will make moves for
     * @param team the team that this AI controls
     */
    EnemyAI(Game& game, Team& team);

    /**
     * @brief initializes patrol ranges of all units to be centered on their current location, called in constructor
     *
     * @return void
     */
    void initialize_patrol_ranges();

    /**
     * @brief Returns true if given coordinates are inside of the specified unit's patrol range
     *
     * @param coords
     * @param unit_id
     * @return bool
     */
    bool is_in_units_patrol_range(const coordinates<size_t>& coords, int unit_id);

    /**
     * @brief Generates coordinates for the given unit to move to
     *
     * @param unit the unit that we want to generate movement for
     * @return coordinates<size_t>
     */
    coordinates<size_t> generate_movement(Unit& unit, const coordinates<size_t>& unit_loc);

    /**
     * @brief Returns the coordinates that have the lowest HP unit on them. Causes an exception if the coordinates don't have any units at all.
     *
     * @param all_coords the coordinates to look through
     * @return coordinates<size_t> the coordinates of the lowest hp enemy found
     */
    coordinates<size_t> get_lowest_hp_unit_coords(const std::vector<coordinates<size_t>>& all_coords) const;

    /**
     * @brief check if a unit at given coordinates can see an enemy
     *
     * @param unit_loc location of the unit
     * @return bool true if can see, false if cant
     */
    bool unit_sees_enemy(const coordinates<size_t>& unit_loc) const;

    /**
     * @brief Adds found units to their associated vectors (enemies/teammates) given as parameter pointers. Pass nullptr to ignore that group of units.
     *
     * @param coords all coords to look through for units
     * @param enemy_coords ptr to vector where the enemy unit coordinates will be stored. pass nullptr to ignore enemies
     * @param teammate_coords ptr to vector where the teammate unit coordinates will be stored. pass nullptr to ignore teammates
     * @return void
     */
    void get_visible_unit_coords(const std::vector<coordinates<size_t>>& vision_coords, std::vector<coordinates<size_t>>* enemy_coords, std::vector<coordinates<size_t>>* teammate_coords);

    /**
     * @brief Generates an action that heals on the target location, returns nullptr if unit has no healing item to use
     *
     * @param unit unit that will execute this action
     * @param unit_loc location of the unit
     * @param target location that the action is targeted on
     * @return std::shared_ptr<Action> shared_ptr to the healing action, nullptr if there is not healing item to use
     */
    std::shared_ptr<Action> generate_heal_action(Unit& unit, const coordinates<size_t>& unit_loc, const coordinates<size_t>& target);

    /**
     * @brief generate a weapon action for the specified unit, targeting specified location. returns nullptr if the unit has no weapons to use
     *
     * @param unit unit that this action is generated for
     * @param unit_loc location of the unit
     * @param target targeted coordinates
     * @return std::shared_ptr<Action> the action to be executed, nullptr if no weapon to use
     */
    std::shared_ptr<Action> generate_weapon_action(Unit& unit, const coordinates<size_t>& unit_loc, const coordinates<size_t>& target);

    /**
     * @brief generates an action for using a building part
     *
     * @param unit the unit that this action is for
     * @param visible_coords coords that the unit can see
     * @return std::shared_ptr<Action> ptr to the action. nullptr if cannot perform a building part action (ie. doesn't have right item)
     */
    std::shared_ptr<Action> generate_building_part_action(Unit& unit, const std::vector<coordinates<size_t>>& visible_coords);

    /**
     * @brief Generate action for the unit to do
     *
     * @param unit unit which the action is generated for
     * @return std::shared_ptr<Action>
     */
    std::shared_ptr<Action> generate_action(Unit& unit, const coordinates<size_t>& unit_loc);

    /**
     * @brief Generates a turn (movement and action) for the specified unit and adds it to the team's action queue
     *
     * @param unit the unit for whom the turn is for
     * @return
     */
    void generate_turn(Unit& unit);

    /**
     * @brief generates turns for all the units in the team managed by this ai
     *
     * @return void
     */
    void generate_whole_teams_turns();

private:
    Game& game_;
    Map& map_;
    Team& team_;


    struct PatrolRange {
        PatrolRange(coordinates<size_t> center, const Map& map):
            center(std::move(center)) {
            size_t half_side_length_floor = std::floor(( patrol_range_side_length - 1 ) / 2.0f);
            size_t half_side_length_ceil = std::ceil(( patrol_range_side_length - 1) / 2.0f);

            leftmost_x = get_valid_coord(center.x, -half_side_length_floor, map.width());
            rightmost_x = get_valid_coord(center.x, half_side_length_ceil, map.width());
            top_y = get_valid_coord(center.y, -half_side_length_floor, map.height());
            bottom_y = get_valid_coord(center.y, half_side_length_ceil, map.height());
        }

        size_t leftmost_x;
        size_t rightmost_x;
        size_t top_y;
        size_t bottom_y;

        coordinates<size_t> center;
    private:
        size_t get_valid_coord(size_t val, int operand, size_t max) const {
            if (operand < 0) {
                if (val >= -operand) return val + operand;
                else return 0;
            } else {
                if (val + operand >= max) return max;
                else return val + operand;
            }
        }
    };

    std::unordered_map<int, PatrolRange> patrol_ranges_;

    static inline size_t patrol_range_side_length = 6;
    static inline float heal_self_hp_percent_threshold_ = 0.5;
    static inline float heal_others_hp_percent_threshold_ = 0.3;

};

