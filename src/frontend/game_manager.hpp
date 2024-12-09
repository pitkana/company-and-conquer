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
     * @brief Initiates the first turn.
     * 
     * @returns True if successful. False if game_ does not contain teams.
     */
    bool init_game();

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
     * @returns Pointer to the team whose turn is it. Nullptr if init_game has not been called successfully.
     */
    Team* get_priority_team() const;

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

    /**
     * @brief Ends turn, executes all action and gives the turn to the next team.
     */
    void next_turn();

private:
    std::weak_ptr<Game> game_;
    const coordinates<size_t> invalid_coord = coordinates<size_t>(-1,-1);
    coordinates<size_t> action_origin = invalid_coord;
    Unit* priority_unit_ = nullptr; //Potential action source.
    Team* priority_team_ = nullptr; //Turn (holder?)

    /**
     * @returns A pointer to the next team. Nullpointer if team vector is empty or game pointer has expired.
     */
    Team* next_team() const;
};

#endif


