#ifndef ACTION_MANAGER_HPP
#define ACTION_MANAGER_HPP

#include "game.hpp"
#include "action.hpp"

class Game_Manager {
public:
    Game_Manager(std::weak_ptr<Game> game) : game_(game) {}

    bool init_game() {
        Game& game = *game_.lock();
        game.get_output_stream() << "Initiating game\n";
        priority_team_ = next_team();
        bool valid_team = priority_team_ != nullptr;
        if (valid_team) {
            game.get_output_stream() << "Team " << priority_team_->get_id() << " turn\n";
        } 
        return valid_team;
    }

    bool action_ontheway() const {
        return action_origin.x != invalid_coord.x && action_origin.y != invalid_coord.y && priority_unit_ != nullptr;
    }

    void terminate_action() {
        action_origin = invalid_coord;
        priority_unit_ = nullptr;
    }

    bool init_priority(coordinates<size_t> origin) {
        if (!game_.lock()->get_map().has_unit(origin) || priority_team_ == nullptr) {
            return false;
        }

        Unit* unit_ptr = game_.lock()->get_map().get_unit(origin);

        if (unit_ptr->is_dead()) return false;

        Unit* team_unit_ptr = priority_team_->get_unit(unit_ptr->get_id());

        if (unit_ptr == team_unit_ptr) {
            action_origin = origin;
            priority_unit_ = unit_ptr;
            return true;
        }

        return false;
    }

    coordinates<size_t> get_priority_coords() const { return action_origin; }

    Team* get_priority_team() const { return priority_team_; }

    bool enqueue_movement_action(coordinates<size_t> target) {
        if (action_ontheway()) {
            std::shared_ptr<MovementAction> next_action = std::make_shared<MovementAction>(action_origin,target,*priority_unit_);
            if (!(game_.lock()->add_action(next_action,priority_team_->get_id()))) return false;
            terminate_action();
            return true;
        }
        return false;
    }

    bool enqueue_item_action(coordinates<size_t> target) {
        if (action_ontheway() && priority_unit_->get_weapons().size() > 0) {
            std::shared_ptr<WeaponAction> next_action = std::make_shared<WeaponAction>(*priority_unit_->get_weapons()[0],target,*priority_unit_);
            if (!(game_.lock()->add_action(next_action,priority_team_->get_id()))) return false;
            terminate_action();
            return true;
        }
        return false;
    }

    void next_turn() {
        game_.lock()->end_team_turns(priority_team_->get_id());
        priority_team_ = next_team();
        if (priority_team_ == nullptr) return;
        game_.lock()->get_output_stream() << "Team " << priority_team_->get_id() << " turn\n";
    }

private:
    std::weak_ptr<Game> game_;
    const coordinates<size_t> invalid_coord = coordinates<size_t>(-1,-1);
    coordinates<size_t> action_origin = invalid_coord;
    Unit* priority_unit_ = nullptr;
    Team* priority_team_ = nullptr;

    Team* next_team() const {
        std::vector<Team>& teams = game_.lock()->get_teams(); 
        if (!(teams.size() > 0)) return nullptr;
        if (priority_team_ == nullptr) {
            return &*teams.begin();
        }
        auto team_it = teams.begin();
        while (team_it != teams.end() && team_it->get_id() != priority_team_->get_id()) { team_it++; }
        if (team_it == teams.end()) return nullptr; //SHOULD NOT HAPPEN!!!!
        team_it++;
        if (team_it != teams.end()) {
            return &*team_it;
        } else {
            return &*teams.begin();
        }
    }
};

#endif