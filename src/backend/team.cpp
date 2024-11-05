#include "team.hpp"


void Team::enqueue_turn(const Turn& turn) {
    turns_.push_back(turn);
}

void Team::enqueue_turn(const Turn&& turn) {
    turns_.push_back(turn);
}

Turn Team::undo_turn() {
    Turn& last = turns_.back();
    turns_.pop_back();
    return last;
}

Turn Team::dequeue_turn() {
    Turn& first = turns_.front();
    turns_.pop_front();
    return first;
}

void Team::add_unit(Unit& unit) {
    team_size_++;
    units_.push_back(unit);
}

std::vector<Unit>& Team::get_units() {
    return units_;
}

size_t Team::team_size() const {
    return team_size_;
}

int Team::get_id() const {
    return team_id_;
}
