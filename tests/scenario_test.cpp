#include "scenario_test.hpp"
#include "scenario_loader.hpp"
#include "scenario.hpp"
#include "game.hpp"

int scenario_test() {
    ScenarioLoader loader = ScenarioLoader("./scenarios/scenario.yaml");
    Scenario scenario = loader.load_scenario();

    std::shared_ptr<Game> game = scenario.generate_game();

    return 1;
}
