#include "renderer.hpp"

//Use WASD to move map and FG to zoom in and out.
//Clicking tile will print the coordinates of that tile.
void rendering_engine_test() {
    Renderer renderer(700,700);
    renderer.initialise_level(0);
    Game& test_game = renderer.get_game();
    Map& test_map = test_game.get_map();
    Team team1;
    Unit unit1("0");
    team1.add_unit(unit1);
    test_game.add_team(team1);
    Team& t = test_game.get_teams()[0];
    Unit* u = &t.get_units()[0];
    test_map.add_unit(0,0,u);
    std::shared_ptr<Building> tent = std::make_shared<MedicTent>();
    std::shared_ptr<Building> turret = std::make_shared<Turret>();;
    test_map.add_building(tent,2,2);
    test_map.add_building(turret,2,1);
    renderer.start();
}