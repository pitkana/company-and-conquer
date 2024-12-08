#include <iostream>
#include <memory>
#include <random>
#include <ctime>

#include "const_items.hpp"
#include "coordinates.hpp"
#include "game.hpp"
#include "action.hpp"
#include "coordinates.hpp"

void action_test() 
{
    srand(time(0));

    {
    Game game(20, 20);
    std::vector<coordinates<size_t>> locations; locations.reserve(10);

    {
        Team team1;

        for (int i = 0; i < 10; ++i) {
            team1.add_unit(std::to_string(i));
            coordinates<size_t> location(std::rand() % 20, std::rand() % 20);
            locations.push_back(std::move(location));
        }
        for (int i = 0; i < 10; i++) {
            game.get_map().add_unit(locations[i], team1.get_unit(i));
        }
        game.add_team(std::move(team1));
    }

    Team& team1 = game.get_team_by_id(0);

    std::cout << "Unit locations before moving" << std::endl;
    game.get_map().print_units();

    { int i = 0;
    for (Unit& unit : team1.get_units()) {
        int choice = std::rand() % 3;

        coordinates<size_t> target(std::rand() % 20, std::rand() % 20);
        coordinates<size_t> dest(std::rand() % 20, std::rand() % 20);
        if (choice == 0) {
            unit.add_item(ConstItem::turret_weapon);
        } else if (choice == 1) {
            unit.add_item(ConstItem::turret_legs);
        } else {
            unit.add_item(ConstItem::medic_tent_heal_item);
        }
        // game.add_turn(Turn(unit, locations[i++], dest, unit.get_inventory()[0]->get_action(target)), 0);
        game.add_action(std::make_shared<MovementAction>(locations[i++], dest, unit), 0);
        game.add_action(unit.get_inventory()[0]->get_action(target, unit), 0);
    } }
    std::cout << "\nLocations after moving" << std::endl;
    game.get_map().print_units();

    std::cout << "\nBuildings before undoing everything" << std::endl;
    game.get_map().print_buildings();

    for (int i = 0; i < 20; i++) {
        game.undo_action(0);
    }

    std::cout << "\nBuildings after undoing everything" << std::endl;
    game.get_map().print_buildings();

    std::cout << "\nLocations after undoing everything" << std::endl;
    game.get_map().print_units();

    std::cout << "\nTest undoing healing, jack's hp should be 100 on both prints" << std::endl;
    Unit jack = Unit("Jack");
    game.get_map().add_unit(5, 5, &jack);
    std::cout << "Jack's hp:" << jack.get_hp() << std::endl;
    // game.add_turn(Turn(*team1.get_unit(0), locations[0], {5, 5}, ConstItem::medic_tent_heal_item->get_action({5, 5})), 0);
    game.add_action(ConstItem::medic_tent_heal_item->get_action({5, 5}, *team1.get_unit(0)), 0);
    game.undo_action(0);
    std::cout << "Jack's hp:" << jack.get_hp() << std::endl;

    }

    Game game(20, 20);
    //AoE test
    std::vector<Unit*> aoe_test_units;

    {
        Team team1;
        for (int i = 0; i < 10; i++) {
            // 11 units already created
            team1.add_unit(std::to_string(i + 11));
        }

        for (auto& unit : team1.get_units()) {
            game.get_map().add_unit(10, unit.get_id() - 6, &unit);
        }
        game.add_team(std::move(team1));

    }

    Team& team1 = game.get_team_by_id(1);

    for (int i = 0; i < 10; i++) {
        Unit* unit = team1.get_unit(i + 11);
        game.get_map().add_unit(10, i + 5, unit);
        aoe_test_units.push_back(unit);
    }
    // game.add_turn(Turn(*team1.get_unit(11), {5, 10}, {10, 7}, ConstItem::grenade->get_action({10, 10})), 1);
    game.add_action(ConstItem::grenade->get_action({10, 10}, *team1.get_unit(11)), 1);

    std::cout << "===== AoE test =====" << std::endl;
    std::cout << "Unit locations:" << std::endl;
    game.get_map().print_units();
    std::cout << "Grenade with AoE of 3 will be thrown at {10, 10}" << std::endl;
    std::cout << "Unit HPs before grenade" << std::endl;
    for (const auto* unit : aoe_test_units) {
        std::cout << "HP of unit " << unit->get_name() << ": " << unit->get_hp() << std::endl;
    }

    game.end_team_turns(1);
    std::cout << "Unit HPs after grenade" << std::endl;
    for (const auto* unit : aoe_test_units) {
        std::cout << "HP of unit " << unit->get_name() << ": " << unit->get_hp() << std::endl;
    }
}

