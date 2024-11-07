#include "team.hpp"

void Team::enqueue_turn(const Turn& turn) {
    turns_.push_back(turn);
}

void Team::enqueue_turn(const Turn&& turn) {
    turns_.push_back(turn);
}

std::optional<Turn> Team::undo_turn() {
    if (turns_.empty()) {
        return std::nullopt;
    }

    Turn& last = turns_.back();
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
    units_.push_back(unit);
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
