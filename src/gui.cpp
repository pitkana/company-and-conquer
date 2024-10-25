#define DEBUG 1 //set to 0 to disable debug features

#include <iostream>
#include <memory>
#include <SFML/Window.hpp>
#include "backend/game.hpp"
#include "backend/action.hpp"

int main() 
{
    Game game;
    game.add_action(Action::CharacterAction(0.5, 20, helper::coordinates<size_t>(5, 3)));
    game.add_action(Action::BuildingAction("turret", helper::coordinates<size_t>(15, 1)));
    game.add_action(Action::CharacterAction(0.3, 50, helper::coordinates<size_t>(2, 10)));

    game.execute_actions();
    return 0;
}

