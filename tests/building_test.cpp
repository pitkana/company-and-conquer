#include <iostream>
#include <memory>

#include "game.hpp"
#include "team.hpp"
#include "unit.hpp"
#include "item.hpp"
#include "const_items.hpp"

void building_test() 
{
    Game game(20, 20);

    game.add_team(Team());

    Team& team1 = game.get_team_by_id(0);

    {team1.add_unit(Unit("0"));
    Unit& unit0 = team1.get_units().front();
    unit0.add_item(ConstItem::turret_legs);

    team1.add_unit(Unit("1"));
    Unit& unit1 = team1.get_units()[1];
    unit1.add_item(ConstItem::turret_barrel);

    team1.add_unit(Unit("2"));
    Unit& unit2 = team1.get_units()[2];
    unit2.add_item(ConstItem::medic_tent_tent);

    team1.add_unit(Unit("3"));
    Unit& unit3 = team1.get_units()[3];
    unit3.add_item(ConstItem::medic_tent_medkit);}

    Unit& unit0 = *team1.get_unit(0);
    Unit& unit1 = *team1.get_unit(1);
    Unit& unit2 = *team1.get_unit(2);
    Unit& unit3 = *team1.get_unit(3);

    Turn u0_turn = Turn(unit0, {0, 0}, {1, 1}, unit0.get_inventory().front()->get_action({5, 5}));
    Turn u1_turn = Turn(unit1, {0, 0}, {1, 1}, unit1.get_inventory().front()->get_action({5, 5}));
    
    Turn u2_turn = Turn(unit2, {0, 0}, {1, 1}, unit2.get_inventory().front()->get_action({4, 4}));
    Turn u3_turn = Turn(unit3, {0, 0}, {1, 1}, unit3.get_inventory().front()->get_action({4, 4}));

    Turn u4_turn = Turn(unit3, {0, 0}, {1, 1}, unit3.get_inventory().front()->get_action({4, 4}));
    Turn u5_turn = Turn(unit2, {0, 0}, {1, 1}, unit2.get_inventory().front()->get_action({4, 4}));
    Turn u6_turn = Turn(unit0, {0, 0}, {1, 1}, unit0.get_inventory().front()->get_action({5, 5}));
    Turn u7_turn = Turn(unit1, {0, 0}, {1, 1}, unit1.get_inventory().front()->get_action({5, 5}));
    Turn u8_turn = Turn(unit3, {0, 0}, {1, 1}, unit3.get_inventory().front()->get_action({5, 5}));
    Turn u9_turn = Turn(unit2, {0, 0}, {1, 1}, unit2.get_inventory().front()->get_action({5, 5}));
    Turn u10_turn = Turn(unit0, {0, 0}, {1, 1}, unit0.get_inventory().front()->get_action({4, 4}));
    Turn u11_turn = Turn(unit1, {0, 0}, {1, 1}, unit1.get_inventory().front()->get_action({4, 4}));

    std::cout << "Following 4 should work" << std::endl;
    team1.enqueue_turn(u0_turn);
    team1.enqueue_turn(u1_turn);
    team1.enqueue_turn(u2_turn);
    team1.enqueue_turn(u3_turn);

    game.end_team_turns(0);

    std::cout << "======= Following 4 should not work, since the part is alraedy in the building =======" << std::endl;
    team1.enqueue_turn(u4_turn);
    team1.enqueue_turn(u5_turn);
    team1.enqueue_turn(u6_turn);
    team1.enqueue_turn(u7_turn);

    game.end_team_turns(0);

    std::cout << "======= Following 4 should not work, since the part is for wrong building =======" << std::endl;
    team1.enqueue_turn(u8_turn);
    team1.enqueue_turn(u9_turn);
    team1.enqueue_turn(u10_turn);
    team1.enqueue_turn(u11_turn);

    game.end_team_turns(0);

    // Use the buildings
    std::cout << "Use the turret and medic tent" << std::endl;
    team1.enqueue_turn(Turn(unit0, {0, 0}, {1, 1}, game.get_map().get_building(5, 5)->use_building({10, 10})));
    team1.enqueue_turn(Turn(unit0, {0, 0}, {1, 1}, game.get_map().get_building(4, 4)->use_building({10, 10})));
    game.end_team_turns(0);
}
