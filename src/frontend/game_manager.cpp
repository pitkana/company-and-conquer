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
    if (!action_ontheway()) return false;

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


