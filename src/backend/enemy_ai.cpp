#include <vector>
#include <random>
#include <ctime>
#include <memory>

#include "action.hpp"
#include "enemy_ai.hpp"
#include "team.hpp"
#include "unit.hpp"
#include "game.hpp"
#include "map.hpp"
#include "coordinates.hpp"


EnemyAI::EnemyAI(Game& game, Team& team):
    game_(game), map_(game.get_map()), team_(team) 
{
    initialize_patrol_ranges();
    srand(time(0));
}

void EnemyAI::initialize_patrol_ranges() {
    for (const Unit& unit : team_.get_units()) {
        patrol_ranges_.emplace(unit.get_id(), PatrolRange(game_.get_unit_location(unit.get_id()), map_));
    }
}


bool EnemyAI::is_in_units_patrol_range(const coordinates<size_t> &coords, int unit_id) {
    const auto& range_it = patrol_ranges_.find(unit_id);
    assert(range_it != patrol_ranges_.end() && "Unit with specified ID has not had their patrol range initialized or is not part of the team");
    const PatrolRange& range = (*range_it).second;

    return coords.y >= range.top_y && coords.y <= range.bottom_y && coords.x >= range.leftmost_x && coords.x <= range.rightmost_x;

}

coordinates<size_t> EnemyAI::generate_movement(Unit& unit, const coordinates<size_t>& unit_loc) {
    coordinates<size_t> chosen_movement;
    std::vector<coordinates<size_t>> movement_locations = map_.possible_tiles_to_move_to3(unit_loc, unit_consts.move_range);

    std::vector<coordinates<size_t>> vision_coords = map_.tiles_unit_sees(unit_loc, unit_consts.visual_range);
    std::vector<coordinates<size_t>> visible_enemy_coords;
    get_visible_unit_coords(vision_coords, &visible_enemy_coords, nullptr);

    if (visible_enemy_coords.empty()) { // Can't see enemy

        // If unit is already inside its patrol range and has no visible enemy, just move inside the patrol range
        if (is_in_units_patrol_range(unit_loc, unit.get_id())) {
            // Take random movement till it's inside of the patrol range
            while (true) {
                chosen_movement = movement_locations[rand() % movement_locations.size()];

                // Break if it's in the range
                if (is_in_units_patrol_range(chosen_movement, unit.get_id()))
                    break;

            }
        } else { // Otherwise move towards center of range
            const auto& range_it = patrol_ranges_.find(unit.get_id());
            assert(range_it != patrol_ranges_.end() && "Unit with specified ID has not had their patrol range initialized or is not part of the team");
            const PatrolRange& range = (*range_it).second;

            chosen_movement = map_.fastest_movement_to_target(unit_loc, range.center, unit_consts.move_range);
        }
    } else { //If can see enemy, move towards a random one
        chosen_movement = map_.fastest_movement_to_target(unit_loc, visible_enemy_coords[rand() % visible_enemy_coords.size()], unit_consts.move_range);
    }

    return chosen_movement;

}

coordinates<size_t> EnemyAI::get_lowest_hp_unit_coords(const std::vector<coordinates<size_t>> &all_coords) const {
    int current_min = unit_consts.max_hp + 1;
    const coordinates<size_t>* min_coords_ptr = nullptr;

    for (const auto& coords : all_coords) {
        if (Unit* unit = map_.get_unit(coords);
            unit != nullptr && !unit->is_dead()) {

            if (int unit_hp = unit->get_hp();
                unit_hp < current_min) {
                current_min = unit_hp;
                min_coords_ptr = &coords;
            }
        }
    }

    assert(min_coords_ptr != nullptr && "The coordinates given did not have any units on them");
    return *min_coords_ptr;
}


void EnemyAI::get_visible_unit_coords(const std::vector<coordinates<size_t>> &vision_coords, std::vector<coordinates<size_t>> *enemy_coords, std::vector<coordinates<size_t>> *teammate_coords) {
    int this_team_id = team_.get_id();
    for (const auto& coords : vision_coords) {
        // Check for unit at visible coords, only take alive units
        if (Unit* target_unit = map_.get_unit(coords);
            target_unit != nullptr && !target_unit->is_dead()) {
            // if seen unit is on same team
            if (game_.get_unit_team_id(target_unit->get_id()) == this_team_id) {
                if (teammate_coords == nullptr) continue;
                teammate_coords->push_back(coords);
            } else { // if on enemy team
                if (enemy_coords == nullptr) continue;
                enemy_coords->push_back(coords);
            }
        }
    }

}

std::shared_ptr<Action> EnemyAI::generate_heal_action(Unit& unit, const coordinates<size_t>& unit_loc, const coordinates<size_t>& target) {
    // Use building if possible
    if (map_.has_healing_building(unit_loc)) {
        return map_.get_building(unit_loc)->use_building(target, unit);
    }

    if (unit.has_healing_item()) {
        std::vector<std::shared_ptr<const HealingItem>> heal_items = unit.get_healing_items();
        return heal_items[rand() % heal_items.size()]->get_action(target, unit);
    }

    // If no healing item to use, return nullptr
    return nullptr;
}


std::shared_ptr<Action> EnemyAI::generate_weapon_action(Unit &unit, const coordinates<size_t>& unit_loc, const coordinates<size_t> &target) {
    // Use building is possible
    if (map_.has_weapon_building(unit_loc)) {
        return map_.get_building(unit_loc)->use_building(target, unit);
    }

    if (unit.has_weapon()) {
        std::vector<std::shared_ptr<const Weapon>> weapons = unit.get_weapons();
        return weapons[rand() % weapons.size()]->get_action(target, unit);
    }

    return nullptr;
}

std::shared_ptr<Action> EnemyAI::generate_building_part_action(Unit& unit, const std::vector<coordinates<size_t>>& visible_coords) {
    // If unit doesn't have a building part, this action is impossible
    std::vector<std::shared_ptr<const BuildingPart>> building_parts = unit.get_building_parts();
    if (building_parts.empty()) return nullptr;

    std::vector<const coordinates<size_t>*> coords_to_build_on;

    for (const coordinates<size_t>& coords : visible_coords) {
        // If you can't build on the coords, there can't be buildings there so continue
        if (!map_.can_build_on(coords)) continue;

        std::shared_ptr<Building> building = map_.get_building(coords);
        if (building == nullptr) {
            // can build on these coords and theres no buildings on it, so can start a new building
            coords_to_build_on.push_back(&coords);
            continue;
        } 

        for (const std::shared_ptr<const BuildingPart>& building_part : building_parts) {
            if (!building->takes_part(*building_part)) continue;

            // If there is any building to add a part to just add the part to it right away
            return building_part->get_action(coords, unit);
        }

    }

    //If there were no buildings to add a part to, just add it to an empty place
    coordinates<size_t> target = *coords_to_build_on[rand() % coords_to_build_on.size()];
    return building_parts[rand() % building_parts.size()]->get_action(std::move(target), unit);

}

std::shared_ptr<Action> EnemyAI::generate_action(Unit& unit, const coordinates<size_t>& unit_loc) {
    if (unit.is_dead()) return nullptr;

    // If unit is low and has a way to heal itself, heal self
    if (unit.get_hp() <= unit_consts.max_hp * heal_self_hp_percent_threshold_) {
        // Generate heal action and return it if it's not nullptr
        if (std::shared_ptr<Action> heal_action = generate_heal_action(unit, unit_loc, unit_loc);
            heal_action != nullptr) {
            return heal_action;
        }
    }


    // Get coordinates that have a teammate or enemy on them
    std::vector<coordinates<size_t>> vision_coords = map_.tiles_unit_sees(unit_loc, unit_consts.visual_range);
    std::vector<coordinates<size_t>> visible_enemy_coords;
    std::vector<coordinates<size_t>> visible_teammate_coords;

    get_visible_unit_coords(vision_coords, &visible_enemy_coords, &visible_teammate_coords);

    // Prioritize attacking enemies. kills > teamplay
    if (!visible_enemy_coords.empty()) {
        // Prioritize attacking enemies with lower health, find lowest hp enemy and attack those coords
        coordinates<size_t> target_coords = get_lowest_hp_unit_coords(visible_enemy_coords);

        // If weapon_action was possible (AKA has something to attack with), return that action. otherwise keep going
        std::shared_ptr<Action> weapon_action = generate_weapon_action(unit, unit_loc, target_coords);
        if (weapon_action != nullptr) {
            return weapon_action;
        }
    }

    // If can't attack enemies, heal teammates if they're low enough
    if (!visible_teammate_coords.empty()) {
        coordinates<size_t> target_coords = get_lowest_hp_unit_coords(visible_teammate_coords);
        if (Unit* target_unit = map_.get_unit(target_coords);
            target_unit != nullptr && target_unit->get_hp() < unit_consts.max_hp * heal_others_hp_percent_threshold_) {
            if (std::shared_ptr<Action> heal_action = generate_heal_action(unit, unit_loc, target_coords);
                heal_action != nullptr) {
                return heal_action;
            }
        }
    }

    // If can't heal teammates, place any building parts anywhere, prioritizing already staretd buildings
    if (std::shared_ptr<Action> building_part_action = generate_building_part_action(unit, vision_coords);
        building_part_action != nullptr) {
        return building_part_action;
    }

    // At this point cant do anything, so just return nullptr aka no action
    return nullptr;
}

void EnemyAI::generate_turn(Unit &unit) {
    coordinates<size_t> unit_location = game_.get_unit_location(unit.get_id());
    coordinates<size_t> movement_target = generate_movement(unit, unit_location);

    // First generate and queue the movement action, so that the unit moves before executing the other action
    std::shared_ptr<Action> movement_action = std::make_shared<MovementAction>(unit_location, movement_target, unit);
    game_.add_action(std::move(movement_action), team_.get_id());

    std::shared_ptr<Action> action = generate_action(unit, movement_target);
    if (action == nullptr) return;
    game_.add_action(std::move(action), team_.get_id());
}


void EnemyAI::generate_whole_teams_turns() {
    for (Unit& unit : team_.get_units()) {
        if (unit.is_dead()) continue;
        generate_turn(unit);
    }
}

int EnemyAI::team_id() const {
    return team_.get_id();
}
