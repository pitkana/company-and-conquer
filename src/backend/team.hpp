#pragma once

#include <vector>
#include <deque>

#include "action.hpp"
#include "turn.hpp"
#include "unit.hpp"

class Team {
public:

    Team() {
        team_id_ = id_count_;
        id_count_++;
    }

    // Add turn to be executed to the queue
    void enqueue_turn(const Turn& turn);
    //moved turn version
    void enqueue_turn(const Turn&& turn);

    //remove last turn to be added and return it
    Turn undo_turn();

    //remove first turn to be added and return it
    [[nodiscard]]
    Turn dequeue_turn();

    //add Unit to the team
    void add_unit(Unit& unit);

    //Remove unit TODO: Add when way to identify Units exists
    // void remove_unit();

    [[nodiscard]]
    std::vector<Unit>& get_units();

    [[nodiscard]]
    size_t team_size() const;

    [[nodiscard]]
    int get_id() const;

private:
    std::vector<Unit> units_;
    std::deque<Turn> turns_;
    size_t team_size_;
    int team_id_;

    inline static int id_count_ = 0;
};

