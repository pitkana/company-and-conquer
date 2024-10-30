#pragma once
#include "action.hpp"
#include <deque>
#include <memory>
#include <variant>
#include <iostream>


class Game {
public:
    void execute_actions() {
        while (!actions_.empty()) {
            const auto& action = actions_.front();
            std::visit(visitor_, action);

            actions_.pop_front();
        }
    }

    void add_action(const std::variant<ACTION_TYPES>& action) {
        actions_.push_back(action);
    }


private:
    std::deque<std::variant<ACTION_TYPES>> actions_;

    struct ActionVisitor {
        void operator() (const Action::BuildingAction& buildAction) {
            std::cout << "Building a " << buildAction.building_type() << " at " << buildAction.target().toString() << std::endl;
        }

        void operator() (const Action::CharacterAction& charAction) {
            std::cout << "Dealing " << charAction.hp_effect() << " damage to enemy at " << charAction.target().toString() 
                      << " with accuracy " << charAction.accuracy() << std::endl;
        }
    };

    ActionVisitor visitor_;
};
