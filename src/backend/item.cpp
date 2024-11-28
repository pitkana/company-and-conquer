#include <memory>

#include "action.hpp"
#include "item.hpp"
#include "building.hpp"
#include "building_part_type.hpp"

// Item base class
const std::string& Item::get_name() const {
    return name_;
}

const std::string& Item::get_description() const {
    return description_;
}


// Weapon derived class
std::shared_ptr<Action> Weapon::get_action(coordinates<size_t> target) const {
    return std::static_pointer_cast<Action>(std::make_shared<WeaponAction>(*this, std::move(target)));
}

// HealingItem derived class
std::shared_ptr<Action> HealingItem::get_action(coordinates<size_t> target) const {
    return std::static_pointer_cast<Action>(std::make_shared<HealingAction>(*this, std::move(target)));
}

// BuildingPart derived class
std::shared_ptr<Action> BuildingPart::get_action(coordinates<size_t> target) const {
    return std::static_pointer_cast<Action>(std::make_shared<BuildingAction>(*this, std::move(target)));
}

BuildingPartType BuildingPart::get_part_type() const {
    return part_type_;
}

std::shared_ptr<Building> BuildingPart::get_building() const {
    switch (part_type_) {
        case BuildingPartType::TurretLegs:
        case BuildingPartType::TurretBarrel: {
            std::shared_ptr<Turret> turret = std::make_shared<Turret>();
            turret->add_part(*this);
            return turret;
        }

        case BuildingPartType::MedicTentTent:
        case BuildingPartType::MedicTentMedkit: {
            std::shared_ptr<MedicTent> medic_tent = std::make_shared<MedicTent>();
            medic_tent->add_part(*this);
            return medic_tent;
        }
    }
    //Unreachable case since all enum class cases are handled
    __builtin_unreachable();
}
