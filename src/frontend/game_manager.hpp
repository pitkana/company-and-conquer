#ifndef ACTION_MANAGER_HPP
#define ACTION_MANAGER_HPP

#include "game.hpp"
#include "action.hpp"

/**
 * @brief Used through the UI to manage turns in Game class.
 */
class Game_Manager {
public:
    Game_Manager(std::weak_ptr<Game> game);

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

    /**
     * @brief Enqueues movement action for priority_unit_ from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_movement_action(const coordinates<size_t>& target);

    /**
     * @brief Enqueues item action for priority_unit_ from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_item_action(coordinates<size_t> target);

    std::string get_action_info(const coordinates<size_t>& potential_target, const Item* action_item);

private:
    void get_movement_action_info(std::stringstream& info_stream, const coordinates<size_t>& potential_target);
    void get_item_action_info(std::stringstream& info_stream, const coordinates<size_t>& potential_target, const Item* action_item);

    bool can_selected_unit_move_to(const coordinates<size_t>& potential_target) const;
    bool can_selected_unit_attack_to(const coordinates<size_t>& potential_target) const;

    bool are_valid_coords(const coordinates<size_t>& coords) const;

private:
    std::weak_ptr<Game> game_;

    coordinates<size_t> selected_unit_coords_ = invalid_coord;
    Unit* selected_unit_ptr_ = nullptr; //Potential action source.
    std::vector<coordinates<size_t>> coords_selected_unit_can_move_to_;
    std::vector<coordinates<size_t>> coords_selected_unit_can_shoot_to_;


    static inline const coordinates<size_t> invalid_coord = coordinates<size_t>(-1,-1);
};

#endif


