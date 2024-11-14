#include "action.hpp"
#include "item.hpp"
#include "game.hpp"

const coordinates<size_t>& Action::target() const {
    return target_;
}

int ItemAction::accuracy() const {
    return accuracy_;
}

int ItemAction::hp_effect() const {
    return hp_effect_;
}


void ItemAction::execute(Game &game) const {
    std::cout << "Dealing " << hp_effect() << " damage to enemy at " << target().toString() 
              << " with accuracy " << accuracy() << " %" << std::endl;
}

BuildingPartType BuildingAction::get_part_type() const {
    return building_part_.get_part_type();
}

void BuildingAction::execute(Game &game) const {
    std::cout << "Adding a " << building_part_.get_name() << ", " << building_part_.get_description() << " to " << target().x << ", " << target().y << "." << std::endl;
}
