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

    game.get_map().add_unit(0, 0, &unit0);
    game.get_map().add_unit(0, 1, &unit1);
    game.get_map().add_unit(0, 2, &unit2);
    game.get_map().add_unit(0, 3, &unit3);

    std::shared_ptr<Action> u0_action = unit0.get_inventory().front()->get_action({5, 5}, unit0);
    std::shared_ptr<Action> u1_action = unit1.get_inventory().front()->get_action({5, 5}, unit1);
    
    std::shared_ptr<Action> u2_action = unit2.get_inventory().front()->get_action({4, 4}, unit2);
    std::shared_ptr<Action> u3_action = unit3.get_inventory().front()->get_action({4, 4}, unit3);

    std::shared_ptr<Action> u4_action = unit3.get_inventory().front()->get_action({4, 4}, unit3);
    std::shared_ptr<Action> u5_action = unit2.get_inventory().front()->get_action({4, 4}, unit2);
    std::shared_ptr<Action> u6_action = unit0.get_inventory().front()->get_action({5, 5}, unit0);
    std::shared_ptr<Action> u7_action = unit1.get_inventory().front()->get_action({5, 5}, unit1);
    std::shared_ptr<Action> u8_action = unit3.get_inventory().front()->get_action({5, 5}, unit3);
    std::shared_ptr<Action> u9_action = unit2.get_inventory().front()->get_action({5, 5}, unit2);
    std::shared_ptr<Action> u10_action = unit0.get_inventory().front()->get_action({4, 4}, unit0);
    std::shared_ptr<Action> u11_action = unit1.get_inventory().front()->get_action({4, 4}, unit1);

    std::cout << "Following 4 should work" << std::endl;
    team1.enqueue_action(u0_action);
    team1.enqueue_action(u1_action);
    team1.enqueue_action(u2_action);
    team1.enqueue_action(u3_action);

    game.end_team_turns(0);

    std::cout << "======= Following 4 should not work, since the part is alraedy in the building =======" << std::endl;
    team1.enqueue_action(u4_action);
    team1.enqueue_action(u5_action);
    team1.enqueue_action(u6_action);
    team1.enqueue_action(u7_action);

    game.end_team_turns(0);

    std::cout << "======= Following 4 should not work, since the part is for wrong building =======" << std::endl;
    team1.enqueue_action(u8_action);
    team1.enqueue_action(u9_action);
    team1.enqueue_action(u10_action);
    team1.enqueue_action(u11_action);

    game.end_team_turns(0);

    // Use the buildings
    std::cout << "Use the turret and medic tent" << std::endl;
    team1.enqueue_action(game.get_map().get_building(5, 5)->use_building({10, 10}, unit0));
    team1.enqueue_action(game.get_map().get_building(4, 4)->use_building({10, 10}, unit0));
    game.end_team_turns(0);
}
