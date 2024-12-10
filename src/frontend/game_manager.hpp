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
    bool action_ontheway() const;

    /**
     * @brief Clears current action.
     */
    void terminate_action();

    /**
     * @brief Initiates priority for unit located in origin.
     * 
     * @returns True if successful. False if origin does not have an unit or the unit is dead or if the unit is not in priority team.
     */
    bool init_priority(const coordinates<size_t>& origin);

    const coordinates<size_t>& get_priority_coords() const;

    /**
     * @brief Enqueues movement action for priority_unit_ from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_movement_action(coordinates<size_t> target);

    /**
     * @brief Enqueues item action for priority_unit_ from action_origin_ to target.
     * 
     * @returns bool based on the fact if enqueing was succesful.
     */
    bool enqueue_item_action(coordinates<size_t> target);

private:
    std::weak_ptr<Game> game_;
    const coordinates<size_t> invalid_coord = coordinates<size_t>(-1,-1);
    coordinates<size_t> action_origin = invalid_coord;
    Unit* priority_unit_ = nullptr; //Potential action source.
};

#endif


