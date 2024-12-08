#pragma once

#include <string>
#include "team.hpp"
#include "shop.hpp"
#include "map.hpp"
#include "scenario.hpp"
#include "coordinates.hpp"
#include "yaml-cpp/yaml.h"

class ScenarioLoader {

public:
    ScenarioLoader(const std::string& path);

    Scenario load_scenario();

private:
    const std::string& path_;

    YAML::Node scenario_;

    Team construct_enemy();
    Shop construct_shop();
    Map construct_map();

    int get_player_team_size();

    std::vector<coordinates<size_t>> enemy_positions_;
    std::vector<coordinates<size_t>> player_positions_;
};


