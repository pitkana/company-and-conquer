#include "game_manager.hpp"


Game_Manager::Game_Manager(std::weak_ptr<Game> game) : game_(game) {}

bool Game_Manager::action_ontheway() const {
    return action_origin != invalid_coord && priority_unit_ != nullptr;
}

void Game_Manager::terminate_action() {
    action_origin = invalid_coord;
    priority_unit_ = nullptr;
}

bool Game_Manager::init_priority(const coordinates<size_t>& origin) {
    std::shared_ptr<Game> game = game_.lock();
    if (!game->get_map().has_unit(origin) || !(game->game_started())) return false;

    Unit* unit_ptr = game->get_map().get_unit(origin);

    if (unit_ptr->is_dead() || game->get_unit_team_id(unit_ptr->get_id()) != game->get_active_team()->get_id()) return false;

    action_origin = origin;
    priority_unit_ = unit_ptr;
    return true;
}

const coordinates<size_t>& Game_Manager::get_priority_coords() const { return action_origin; }

bool Game_Manager::enqueue_movement_action(coordinates<size_t> target) {
    std::shared_ptr<Game> game = game_.lock();
    if (!action_ontheway() || priority_unit_->has_moved || !(can_priority_unit_move_to(target))) return false;

    std::shared_ptr<MovementAction> next_action = std::make_shared<MovementAction>(action_origin,target,*priority_unit_);
    if (!(game_.lock()->add_action(next_action,game_.lock()->get_active_team()->get_id()))) return false;

    terminate_action();
    return true;
}

bool Game_Manager::enqueue_item_action(coordinates<size_t> target) {
    if (!action_ontheway() || !priority_unit_->has_weapon()) return false;

    std::shared_ptr<WeaponAction> next_action = std::make_shared<WeaponAction>(*priority_unit_->get_weapons()[0],target,*priority_unit_);
    if (!(game_.lock()->add_action(next_action,game_.lock()->get_active_team()->get_id()))) return false;

    terminate_action();
    return true;
}

std::string Game_Manager::get_action_info(const coordinates<size_t>& potential_target, Item* action_item) const {
    if (!action_ontheway()) return "";
    std::stringstream ss;
    ss << "Unit " << priority_unit_->get_id() << "\n";
    ss << get_movement_action_info(potential_target);
    ss << get_item_action_info(potential_target,action_item);
    return ss.str();
}

std::string Game_Manager::get_movement_action_info(const coordinates<size_t>& potential_target) const {
    if (!action_ontheway()) return "";
    Map& map = game_.lock()->get_map();
    if (priority_unit_->has_moved) {
        return "has already moved\n";
    } else if (can_priority_unit_move_to(potential_target)) {
        return "can move here\n";
    } else {
        return "cannot move here\n";
    }
}

std::string Game_Manager::get_item_action_info(const coordinates<size_t>& potential_target, Item* action_item) const {
    if (!action_ontheway()) return "";
    Map& map = game_.lock()->get_map();
    if (action_item == nullptr) return "has no items\n";
    if (action_item->is_weapon()) {
        return action_item->get_name();
    } else if (action_item->is_healing_item()) {
        return action_item->get_name();
    } else if (action_item->is_building_part()) {
        return action_item->get_name();
    } else {
        return "Unknown item";
    }
}


bool Game_Manager::can_priority_unit_move_to(const coordinates<size_t>& potential_target) const {
    if (!action_ontheway()) return false;
    Map& map = game_.lock()->get_map();
    if (map.has_unit(potential_target) || map.has_building(potential_target)) return false;
    auto possible_tiles = map.possible_tiles_to_move_to(action_origin,5);
    auto tile_it = std::find(possible_tiles.begin(),possible_tiles.end(),potential_target);
    return tile_it != possible_tiles.end();
}

bool Game_Manager::can_priority_unit_attack_to(const coordinates<size_t>& potential_target, const Weapon& weapon_item) const {
    if (!action_ontheway()) return false;
    Map& map = game_.lock()->get_map();
    if (!map.has_unit(potential_target)) return false;
    return true;
}