#include "action.hpp"
#include "item.hpp"
#include "game.hpp"

const coordinates<size_t>& Action::target() const {
    return target_;
}

/* ----- MovementAction ----- */
void MovementAction::execute(Game& game, coordinates<size_t> unit_location [[maybe_unused]]) {
    if (has_been_executed_) return;

    game.get_output_stream() << "Unit: " << this->get_unit().get_id() << " movement result: ";
    if (game.get_map().move_unit(source_location_, target_)) {
        game.get_output_stream() << " Success!\n";
    } else {
        game.get_output_stream() << " Failure!\n";
    }

    has_been_executed_ = true;
}

void MovementAction::undo(Game &game) {
    game.get_map().move_unit(target_, source_location_);
}

/* ----- WeaponAction ----- */

void WeaponAction::execute(Game &game, coordinates<size_t> unit_location) {
    if (!has_been_executed_) { 
        if (executing_unit_.is_dead()) return;
        game.get_output_stream() << "Unit: " << executing_unit_.get_id() << " attacks with weapon: " << weapon_.get_name() << " result: ";

        if (weapon_.get_aoe() == 0) { //Single target attack
            Unit* target_unit = game.get_map().get_unit(target_.y, target_.x);

            if (target_unit == nullptr) {
                game.get_output_stream() << "failure, no target enemy found.\n";
                return;
            }

            int distance = unit_location.distance_to(target_);

            // Hit enemy
            if (target_unit->deal_damage(weapon_.calculate_damage_dealt(distance), weapon_.get_accuracy())) {
                game.get_output_stream() << "success, dealt " << weapon_.get_damage() << " damage to enemy unit " << target_unit->get_id() <<
                " enemy has " << target_unit->get_hp() << " hp.\n";
            } else {
                game.get_output_stream() << executing_unit_.get_name() << " missed!\n";
            }

        } else { // Area of effect attack
            Map& map = game.get_map();
            std::vector<coordinates<size_t>> affected_coords = map.get_aoe_affected_coords(target_, weapon_.get_aoe());
            for (const auto& coords : affected_coords) {
                Unit* target_unit = map.get_unit(coords);
                if (target_unit == nullptr) continue;

                // Distance is calculated from the origin of the AoE (aka target), not from the unit that executes this action
                int distance = target_.distance_to(coords);
                if (target_unit->deal_damage(weapon_.calculate_damage_dealt(distance), weapon_.get_accuracy())) {
                    game.get_output_stream() << "success, dealt " << weapon_.get_damage() << " damage to enemy unit " << target_unit->get_id() <<
                        " enemy has " << target_unit->get_hp() << " hp.\n";
                } else {
                    game.get_output_stream() << executing_unit_.get_name() << " missed!\n";
                }
            }
        }
    }

    has_been_executed_ = true;
}

/* ----- HealingAction ----- */

void HealingAction::execute(Game &game, coordinates<size_t> unit_location) {

    if (!has_been_executed_) {
        if (executing_unit_.is_dead()) return;
        if (healing_item_.get_aoe() == 0) { // Single target healing
            Unit* target_unit = game.get_map().get_unit(target_.y, target_.x);
            if (target_unit == nullptr) return;
            int healed_amount = target_unit->heal(healing_item_);
            game.get_output_stream() << "Healed " << target_unit->get_name() << " for " << healed_amount << ".\n";
            healed_amounts_.emplace_back(target_unit, healed_amount);

        } else { // AoE healing
            Map& map = game.get_map();
            std::vector<coordinates<size_t>> affected_coords = map.get_aoe_affected_coords(target_, healing_item_.get_aoe());
            for (const auto& coords : affected_coords) {
                Unit* target_unit = map.get_unit(coords);
                if (target_unit == nullptr) continue; 
                int healed_amount = target_unit->heal(healing_item_);
                game.get_output_stream() << "Healed " << target_unit->get_name() << " for " << healed_amount << ".\n";
                healed_amounts_.emplace_back(target_unit, healed_amount);
            }
        }
    }
    has_been_executed_ = true;
}

void HealingAction::undo(Game& game) {
    if (has_been_executed_) {
        for (const auto [target_unit, healed_amount] : healed_amounts_) {
            if (target_unit == nullptr) continue;

            // Undo the healed amount on target and set it back to 0
            target_unit->change_hp_by(-healed_amount);
        }
        healed_amounts_.clear();
    }

    has_been_executed_ = false;
}

/* ----- BuildingAction ----- */

BuildingPartType BuildingAction::get_part_type() const {
    return building_part_.get_part_type();
}

const BuildingPart& BuildingAction::get_part() const {
    return building_part_;
}

void BuildingAction::execute(Game &game, coordinates<size_t> unit_location) {
    if (executing_unit_.is_dead()) return;

    if (!has_been_executed_) {
        Map& map = game.get_map();
        if (map.has_building(target())) {
            // Try to add part, fails if part is wrong for the building or this part is already added to building
            if (map.get_building(target())->add_part(building_part_))  { //success
                game.get_output_stream() << "Added " << building_part_.get_name() << " to building that was already at " << target().toString() << "\n";
            } else {
                game.get_output_stream() << "Part " << building_part_.get_name() << " is wrong for the building at " << target() << " or already added to it" << "\n";
            }

        } else if (map.can_build_on(target())) {
            map.add_building(building_part_.get_building(), target());
            game.get_output_stream() << "Built building using " << building_part_.get_name() << " at " << target() << "\n";
        } else {
            game.get_output_stream() << "No building to add to or build at " << target() << "\n";
        }
    }

    has_been_executed_ = true;
}

void BuildingAction::undo(Game &game) {
    if (has_been_executed_) {
        Map& map = game.get_map();

        // If a building exists on the target (it should), remove this part from it. If it has no parts as a consequence, remove the building from map
        if (map.has_building(target())) {
            std::shared_ptr<Building> building = map.get_building(target());
            building->remove_part(building_part_);

            if (building->has_no_parts()) {
                map.remove_building(target());
            }
        }
    }

    has_been_executed_ = false;
}
