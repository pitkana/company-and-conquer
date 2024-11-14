#include <iostream>
#include <memory>
#include <SFML/Window.hpp>

#include "item.hpp"
#include "coordinates.hpp"
#include "game.hpp"
#include "action.hpp"
#include "coordinates.hpp"
#include "turn.hpp"
#include <random>

void building_test() 
{
    Game game;

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
        std::shared_ptr<BuildingPart> turret_legs = std::make_shared<BuildingPart>(BuildingPartType::TurretLegs);
        unit.add_item(std::static_pointer_cast<Item>(turret_legs));
        team1.enqueue_turn(Turn(unit, origin, dest, unit.get_inventory()[0]->get_action(target)));
    }

    game.end_turn(0);
}
