#include <iostream>
#include <memory>
#include <SFML/Window.hpp>

#include "coordinates.hpp"
#include "game.hpp"
#include "action.hpp"
#include "coordinates.hpp"
#include "turn.hpp"
#include <random>

#include "renderer.hpp"

/*
int main() 
{
    Game game(10, 10);

    {
        Team team1;

        for (int i = 0; i < 10; ++i) {
            team1.add_unit(std::to_string(i));
        }

        game.add_team(std::move(team1));
    }

    Team& team1 = game.get_team_by_id(0);

    coordinates<size_t> origin(5, 5);
    coordinates<size_t> dest(10, 10);
    coordinates<size_t> target(7, 7);
    for (Unit& unit : team1.get_units()) {
        int accuracy = std::rand() % 101;
        int damage = (std::rand() % 8) + 1;
        int falloff = (std::rand() % 5) + 1;
        std::shared_ptr<Weapon> gun = std::make_shared<Weapon>("Rifle", accuracy, damage, falloff);
        unit.add_item(std::static_pointer_cast<Item>(gun));
        team1.enqueue_turn(Turn(unit, origin, dest, unit.get_inventory()[0]->get_action(target)));
        // std::cout << std::holds_alternative<Action::CharacterAction>(unit.GetInventory()[0]->get_action(target)) << std::endl;
    }

    game.end_turn(0);
}
*/



int main()
{
    //start(1000, 1000);
    Renderer renderer(1000, 1000);
    renderer.initialise_level(0);
    renderer.start();
    

    return 0;
}
