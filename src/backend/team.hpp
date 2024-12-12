#pragma once

#include <vector>
#include <deque>

#include "action.hpp"
#include "unit.hpp"

class Team {
public:

    Team() {
        team_id_ = id_count_;
        id_count_++;
    }

    // Add turn to be executed to the queue
    void enqueue_action(std::shared_ptr<Action> action);

    //remove last turn to be added and return, nullptr if queue was empty
    std::shared_ptr<Action> undo_action();

    //remove first turn to be added and return, nullptr if queue was empty
    [[nodiscard]]
    std::shared_ptr<Action> dequeue_action();

    //add Unit to the team
    void add_unit(Unit unit);

    //Remove unit by id, return true if worked, false if unit didn't exist
    bool remove_unit(int id);

    //Get unit by id, pointer since unit by that id might not exist
    Unit* get_unit(int id);

    //return reference to units vector
    [[nodiscard]]
    std::vector<Unit>& get_units();

    [[nodiscard]]
    std::vector<Unit*> get_alive_units();

    //const version of get_units
    [[nodiscard]]
    const std::vector<Unit>& get_units() const;

    //return the amount of Units on this team
    [[nodiscard]]
    size_t team_size() const;

    //return the unique id of this team
    [[nodiscard]]
    int get_id() const;

private:
    std::vector<Unit> units_;
    std::deque<std::shared_ptr<Action>> turns_;
    int team_id_;

    //gives team a unique id, incremented once for each team constructed
    inline static int id_count_ = 0;
};

