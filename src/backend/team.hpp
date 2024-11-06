#pragma once

#include <vector>
#include <deque>
#include <optional>

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

    //remove last turn to be added and return it in optional
    std::optional<Turn> undo_turn();

    //remove first turn to be added and return it in optional
    [[nodiscard]]
    std::optional<Turn> dequeue_turn();

    //add Unit to the team
    void add_unit(Unit& unit);

    //Remove unit TODO: Add when way to identify Units exists
    // void remove_unit();

    //return reference to units vector
    [[nodiscard]]
    std::vector<Unit>& get_units();

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
    std::deque<Turn> turns_;
    int team_id_;

    //gives team a unique id, incremented once for each team constructed
    inline static int id_count_ = 0;
};

