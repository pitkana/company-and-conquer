#include <memory>

#include "building_part_type.hpp"
#include "building.hpp"
#include "action.hpp"
#include "item.hpp"

//TODO: finish after action is created for this

// std::shared_ptr<Action> Turret::use_building(const coordinates<size_t> &target) const {
//
// }

bool Turret::add_part(const BuildingPart &part) {
    if (!has_part(part)) {
        switch (part.get_part_type()) {
        case BuildingPartType::TurretLegs:
            has_legs_ = true;
            return true;
        case BuildingPartType::TurretBarrel:
            has_barrel_ = true;
            return true;
        default:
            return false;
        }
    }

    return false;
}

bool Turret::remove_part(const BuildingPart &part) {
    if (!has_part(part)) {
        switch (part.get_part_type()) {
        case BuildingPartType::TurretLegs:
            has_legs_ = true;
            return true;
        case BuildingPartType::TurretBarrel:
            has_barrel_ = true;
            return true;
        default:
            return false;
        }
    }

    return false;

}

bool Turret::is_ready() const {
    return has_barrel() && has_legs();
}

bool Turret::has_part(const BuildingPart &part) const {
    switch (part.get_part_type()) {
        case BuildingPartType::TurretLegs:
            return has_legs();
        case BuildingPartType::TurretBarrel:
            return has_barrel();
        default:
            return false;
    }
}

bool Turret::has_legs() const {
    return has_legs_;
}

bool Turret::has_barrel() const {
    return has_barrel_;
}
