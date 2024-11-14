#include <memory>

#include "action.hpp"
#include "item.hpp"

// Item base class
const std::string& Item::get_name() const {
    return name_;
}

const std::string& Item::get_description() const {
    return description_;
}

// Weapon derived class
std::shared_ptr<Action> Weapon::get_action(const coordinates<size_t> &target) const {
    return std::static_pointer_cast<Action>(std::make_shared<ItemAction>(accuracy_, -damage_, area_of_effect_, target));
}

// Consumable derived class
std::shared_ptr<Action> Consumable::get_action(const coordinates<size_t> &target) const {
    return std::static_pointer_cast<Action>(std::make_shared<ItemAction>(100, heal_amount_, area_of_effect_, target));
}

// BuildingPart derived class
std::shared_ptr<Action> BuildingPart::get_action(const coordinates<size_t> &target) const {
    return std::make_shared<BuildingAction>(*this, target);
}

