#include <algorithm>
#include <cassert>

#include "team.hpp"
#include "unit.hpp"


void Team::enqueue_turn(Turn turn) {
    turns_.push_back(std::move(turn));
}

std::optional<Turn> Team::undo_turn() {
    if (turns_.empty()) {
        return std::nullopt;
    }

    Turn last = turns_.back();
    turns_.pop_back();
    return last;
}

std::optional<Turn> Team::dequeue_turn() {
    if (turns_.empty()) {
        return std::nullopt;
    }
    Turn first = turns_.front();
    turns_.pop_front();
    return first;
}

void Team::add_unit(Unit unit) {
    units_.push_back(std::move(unit));
}

bool Team::remove_unit(int id) {
    auto it = std::find_if(units_.begin(), units_.end(), [id](const Unit& unit) {
        return unit.get_id() == id;
    });
    if (it != units_.end()) {
        units_.erase(it);
        return true;
    }

    return false;
}

Unit* Team::get_unit(int id) {
    auto it = std::find_if(units_.begin(), units_.end(), [id](const Unit& unit) {
        return unit.get_id() == id;
    });

    if (it != units_.end()) {
        return &*it;
    }

    //should not be reached
    assert(false);
    return nullptr;
}


std::vector<Unit>& Team::get_units() {
    return units_;
}

const std::vector<Unit>& Team::get_units() const {
    return units_;
}

size_t Team::team_size() const {
    return units_.size();
}

int Team::get_id() const {
    return team_id_;
}
