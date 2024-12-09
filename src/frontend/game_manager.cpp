#include "game_manager.hpp"


Game_Manager::Game_Manager(std::weak_ptr<Game> game) : game_(game) {}

bool Game_Manager::init_game() {
    Game& game = *game_.lock();
    game.get_output_stream() << "Initiating game\n";
    priority_team_ = next_team();
    bool valid_team = priority_team_ != nullptr;
    if (valid_team) game.get_output_stream() << "Team " << priority_team_->get_id() << " turn\n";
    return valid_team;
}

bool Game_Manager::action_ontheway() const {
    return action_origin != invalid_coord && priority_unit_ != nullptr;
}

void Game_Manager::terminate_action() {
    action_origin = invalid_coord;
    priority_unit_ = nullptr;
}

bool Game_Manager::init_priority(const coordinates<size_t>& origin) {
    std::shared_ptr<Game> game = game_.lock();
    if (!game->get_map().has_unit(origin) || priority_team_ == nullptr) return false;

    Unit* unit_ptr = game->get_map().get_unit(origin);

    if (unit_ptr->is_dead() || game->get_unit_team_id(unit_ptr->get_id()) != priority_team_->get_id()) return false;

    action_origin = origin;
    priority_unit_ = unit_ptr;
    return true;
}

const coordinates<size_t>& Game_Manager::get_priority_coords() const { return action_origin; }

Team* Game_Manager::get_priority_team() const { return priority_team_; }


bool Game_Manager::enqueue_movement_action(coordinates<size_t> target) {
    if (!action_ontheway()) return false;

    std::shared_ptr<MovementAction> next_action = std::make_shared<MovementAction>(action_origin,target,*priority_unit_);
    if (!(game_.lock()->add_action(next_action,priority_team_->get_id()))) return false;

    terminate_action();
    return true;
}

bool Game_Manager::enqueue_item_action(coordinates<size_t> target) {
    if (!action_ontheway() || !priority_unit_->has_weapon()) return false;

    std::shared_ptr<WeaponAction> next_action = std::make_shared<WeaponAction>(*priority_unit_->get_weapons()[0],target,*priority_unit_);
    if (!(game_.lock()->add_action(next_action,priority_team_->get_id()))) return false;

    terminate_action();
    return true;

}

void Game_Manager::next_turn() {
    Game& game = *game_.lock();
    game.end_team_turns(priority_team_->get_id());
    priority_team_ = next_team();
    if (priority_team_ == nullptr) return;
    game.get_output_stream() << "Team " << priority_team_->get_id() << " turn\n";
}

Team* Game_Manager::next_team() const {
    std::shared_ptr<Game> game = game_.lock();
    if (!game) return nullptr;

    std::vector<Team>& teams = game->get_teams();
    if (teams.size() < 0) return nullptr;

    if (priority_team_ == nullptr) return &*teams.begin();

    auto team_it = std::find_if(teams.begin(),teams.end(),[this](const Team& t)
        { return t.get_id() == priority_team_->get_id(); }
    );
    if (team_it == teams.end()) return nullptr; //SHOULD NOT HAPPEN!!!!

    team_it++;
    return (team_it != teams.end()) ? &*team_it : &*teams.begin();
}