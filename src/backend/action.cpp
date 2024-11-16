#include "action.hpp"
#include "item.hpp"
#include "game.hpp"

const coordinates<size_t>& Action::target() const {
    return target_;
}

/* ----- WeaponAction ----- */

int WeaponAction::accuracy() const {
    return accuracy_;
}

int WeaponAction::hp_effect() const {
    return hp_effect_;
}

int WeaponAction::area_of_effect() const {
    return area_of_effect_;
}

void WeaponAction::execute(Game &game) const {
    std::cout << "Dealing " << hp_effect() << " damage to enemy at " << target().toString() 
              << " with accuracy " << accuracy() << " %" << std::endl;
}

/* ----- HealingAction ----- */

int HealingAction::heal_amount() const {
    return heal_amount_;
}

int HealingAction::area_of_effect() const {
    return area_of_effect_;
}

void HealingAction::execute(Game &game) const {
    std::cout << "Healing unit at " << target() << std::endl;
}

/* ----- BuildingAction ----- */

BuildingPartType BuildingAction::get_part_type() const {
    return building_part_.get_part_type();
}

const BuildingPart& BuildingAction::get_part() const {
    return building_part_;
}

void BuildingAction::execute(Game &game) const {
    Map& map = game.get_map();
    if (map.has_building(target())) {
        // Try to add part, fails if part is wrong for the building or this part is already added to building
        if (map.get_building(target())->add_part(building_part_))  { //success
            std::cout << "Added " << building_part_.get_name() << " to building that was already at " << target().toString() << std::endl;
        } else {
            std::cout << "Part " << building_part_.get_name() << " is wrong for the building at " << target() << " or already added to it" << std::endl;
        }
        (std::string) "aaa";

    } else if (map.can_build_on(target())) {
        map.add_building(building_part_.get_building(), target());
        std::cout << "Built building using " << building_part_.get_name() << " at " << target() << std::endl;
    } else {
        std::cout << "No building to add to or build at " << target() << std::endl;
    }
}
