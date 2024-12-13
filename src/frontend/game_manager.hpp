#ifndef ACTION_MANAGER_HPP
#define ACTION_MANAGER_HPP

#include "game.hpp"
#include "action.hpp"

class Tile_Map;
/**
 * @brief Used through the UI to manage turns in Game class.
 */
class Game_Manager {
public:
    Game_Manager(std::weak_ptr<Game> game, std::weak_ptr<Tile_Map> tile_map);

    /**
     * @returns True if action is queued.
     */
    bool selected_valid_unit() const;

    /**
     * @brief Clears current action.
     */
    void deselect_unit();

    /**
     * @brief Initiates priority for unit located in origin.
     * 
     * @returns True if successful. False if origin does not have an unit or the unit is dead or if the unit is not in priority team.
     */
    bool select_unit_on_coords(const coordinates<size_t>& origin);

    const coordinates<size_t>& selected_unit_coords() const;

    const std::vector<coordinates<size_t>>& selected_unit_possible_movements() const;

    Unit* selected_unit_ptr();

    /**
     * @brief Enqueues movement action for active_unit from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_movement_action(const coordinates<size_t>& target);

    /**
     * @brief Enqueues item action for priority_unit_ from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_item_action(coordinates<size_t> target, const Item* action_item);

    /**
     * @brief Can be called to undo_action.
     */
    bool undo_action();

    /**
     * @brief Can be called to advance turn.
     */
    void next_turn();

    /**
     * @brief Used to cycle units within current active team. Moves active_unit status also.
     */
    void cycle_units(int window_width, int window_height);

    /**
     * @brief Gives relevant information about a certain tile in a map and whether active_unit can
     * act in that tile.
     */
    std::string get_action_info(const coordinates<size_t>& potential_target, const Item* action_item);

    Map& get_map();

private:
    /**
     * @brief Gives info about movement action to potential_target coords.
     */
    void get_movement_action_info(std::stringstream& info_stream, const coordinates<size_t>& potential_target);
    /**
     * @brief Gives info about item action to potential_target coords.
     */
    void get_item_action_info(std::stringstream& info_stream, const coordinates<size_t>& potential_target, const Item* action_item);
    /**
     * @brief Gives info about potential_target coords.
     */
    void get_tile_info(std::stringstream& info_stream, const coordinates<size_t>& potential_target);
    /**
     * @brief Checks if active unit can move from selected_unit coords to potential target.
    */
    bool can_selected_unit_move_to(const coordinates<size_t>& potential_target) const;
    /**
     * @brief Checks if active unit can use item from selected_unit coords to potential target.
     */
    bool can_selected_unit_use_item_to(const coordinates<size_t>& potential_target) const;
    bool are_valid_coords(const coordinates<size_t>& coords) const;
private:
    std::weak_ptr<Game> game_;
    std::weak_ptr<Tile_Map> tile_map_;

    coordinates<size_t> selected_unit_coords_ = invalid_coord;
    Unit* selected_unit_ptr_ = nullptr; //Potential action source.
    std::vector<coordinates<size_t>> coords_selected_unit_can_move_to_;
    std::vector<coordinates<size_t>> coords_selected_unit_can_shoot_to_;

    int unit_cycle_idx_ = 0;

    static inline const coordinates<size_t> invalid_coord = coordinates<size_t>(-1,-1);
};

#endif
