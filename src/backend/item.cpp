#include "item.hpp"

// Item base class
const std::string& Item::get_name() const {
    return name_;
}

const std::string& Item::get_description() const {
    return description_;
}

// Weapon derived class
std::variant<ACTION_TYPES> Weapon::get_action(const coordinates<size_t> &target) const {
    return std::move(Action::CharacterAction(accuracy_, -damage_, area_of_effect_, target));
}

// Consumable derived class
std::variant<ACTION_TYPES> Consumable::get_action(const coordinates<size_t> &target) const {
    return std::move(Action::CharacterAction(1.0f, heal_amount_, area_of_effect_, target));
}
