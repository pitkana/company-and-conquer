#include <memory>

#include "building_part_type.hpp"
#include "building.hpp"
#include "action.hpp"
#include "item.hpp"
#include "const_items.hpp"

std::shared_ptr<Action> Building::use_building(coordinates<size_t> target, Unit& executing_unit) const {
    return building_item_->get_action(target, executing_unit);
}


bool Building::has_part(const BuildingPart &part) const {
    const bool* part_bool = get_part_bool_ptr(part);
    if (part_bool == nullptr || !*part_bool) return false;
    return true;
}

bool Building::takes_part(const BuildingPart &part) const {
    return get_part_bool_ptr(part) != nullptr;
}

bool Building::add_part(const BuildingPart &part) {
    bool* part_bool = get_part_bool_ptr(part);
    // If part isn't part of the building or it's already in the building, return false
    if (part_bool == nullptr || *part_bool) return false;

    *part_bool = true;
    return true;
}

bool Building::remove_part(const BuildingPart &part) {
    bool* part_bool = get_part_bool_ptr(part);
    // If part isn't part of the building or it's already not in the building, return false
    if (part_bool == nullptr || !*part_bool) return false;

    *part_bool = false;
    return true;
}

std::shared_ptr<const Item> Building::get_item() const {
    return building_item_;
}

/* ----- Turret ----- */
bool Turret::is_ready() const {
    return has_barrel() && has_legs();
}

bool Turret::has_no_parts() const {
    return !has_barrel() && !has_legs();
}

bool Turret::has_legs() const {
    return has_legs_;
}

bool Turret::has_barrel() const {
    return has_barrel_;
}


/* ----- MedicTent ----- */
bool MedicTent::is_ready() const {
    return has_medkit() && has_tent();
}

bool MedicTent::has_no_parts() const {
    return !has_medkit() && !has_tent();
}

bool MedicTent::has_tent() const {
    return has_tent_;
}

bool MedicTent::has_medkit() const {
    return has_medkit_;
}
