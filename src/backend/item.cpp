#include <memory>
#include <cmath>
#include <sstream>

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
std::shared_ptr<Action> Weapon::get_action(coordinates<size_t> target, Unit& executing_unit) const {
    return std::static_pointer_cast<Action>(std::make_shared<WeaponAction>(*this, std::move(target), executing_unit));
}

float Weapon::calculate_damage_dealt(int distance) const {
    double falloff_rate = (100.0 - (double)falloff_) / 100.0;

    double falloff_effect = std::pow(falloff_rate, (double) distance);
    int final_dmg = (int)round(damage_ * falloff_effect);

    return final_dmg;
}

std::string Weapon::get_info(const coordinates<size_t>& from_coords, const coordinates<size_t> &target_coords) const {
    std::stringstream output;
    output << name_ << ":\n";

    if (area_of_effect_ == 0) {
        output << "Single target\n";
        int distance = from_coords.distance_to(target_coords);
        float expected_damage = calculate_damage_dealt(distance);
        output << "Expected damage: " << expected_damage << "\n";
    } else {
        output << "Area of effect: " << area_of_effect_ << "\n";
    }

    output << "Accuracy: " << accuracy_;

    return output.str();
}

// HealingItem derived class
std::shared_ptr<Action> HealingItem::get_action(coordinates<size_t> target, Unit& executing_unit) const {
    return std::static_pointer_cast<Action>(std::make_shared<HealingAction>(*this, std::move(target), executing_unit));
}

std::string HealingItem::get_info(const coordinates<size_t> &from_coords, const coordinates<size_t> &target_coords) const {
    return name_ + ": will heal " + std::to_string(heal_amount_);
}

// BuildingPart derived class
std::shared_ptr<Action> BuildingPart::get_action(coordinates<size_t> target, Unit& executing_unit) const {
    return std::static_pointer_cast<Action>(std::make_shared<BuildingAction>(*this, std::move(target), executing_unit));
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
