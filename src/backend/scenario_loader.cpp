#include "const_items.hpp"
#include "scenario_loader.hpp"
#include "yaml-cpp/yaml.h"

#include "map_builder.hpp"

ScenarioLoader::ScenarioLoader(const std::string &path) : path_(path) {
    // Load the scenario root node from file:
    try {
        scenario_ = YAML::LoadFile(path);
    } catch (const YAML::Exception &e) {
        throw std::runtime_error("Invalid scenario file path: " + std::string(e.what()));
    }
}

Scenario ScenarioLoader::load_scenario() {
    try {
        Map map = construct_map();
        Team enemy_team = construct_enemy();
        Shop shop = construct_shop();
        bool multiplayer = get_multiplayer();

        Scenario scenario = Scenario(enemy_team, map, shop, enemy_positions_, player_positions_, multiplayer);

        return scenario;
    }
    catch (std::exception) {
        throw;
    }

}

Team ScenarioLoader::construct_enemy() {
    try {
        Team enemy_team;

        YAML::Node enemy_node = scenario_["enemy"];
        assert(enemy_node.IsSequence());
        for (auto && enemy_unit : enemy_node) { // add each enemy (name-items pair)
            auto name = enemy_unit["name"].as<std::string>();
            Unit unit(name);
            for (auto && enemy_item : enemy_unit["items"]) { // items is a sequence of item names
                try {
                    auto item = ConstItem::item_ids[enemy_item.as<std::string>()];
                    unit.add_item(item);
                } catch (const std::exception &e) {
                    throw std::runtime_error("Invalid item name: " + std::string(e.what()));
                }
            }

            enemy_team.add_unit(unit);
        }

        return enemy_team;

    } catch (const YAML::Exception &e) {
        throw std::runtime_error("Invalid enemy team data: " + std::string(e.what()));
    }
}

Shop ScenarioLoader::construct_shop() {
    try {
        YAML::Node shop_node = scenario_["shop"];
        YAML::Node shop_items = shop_node["items"];
        assert(shop_items.IsSequence());
        std::map<std::shared_ptr<const Item>, int> catalogue;
        for (auto && shop_item : shop_items) { // add each item (name-price pair) to catalogue
            try {
                auto item = ConstItem::item_ids[shop_item["name"].as<std::string>()];
                int price = shop_item["price"].as<int>();
                catalogue.insert(std::make_pair(item, price));
            } catch (const std::exception &e) {
                throw std::runtime_error("Invalid shop item data: " + std::string(e.what()));
            }
        }

        int budget = shop_node["budget"].as<int>();
        int team_size = get_player_team_size();

        Shop shop = Shop(catalogue, team_size, budget);

        return shop;

    } catch (const YAML::Exception &e) {
        throw std::runtime_error("Invalid shop data: " + std::string(e.what()));
    }
}

Map ScenarioLoader::construct_map() {
    try {
        Map_Builder builder = Map_Builder();
        YAML::Node map_node = scenario_["map"];
        Map map = builder.load(map_node["path"].as<std::string>());

        // load enemy positions, throw error if not enough positions for all enemies
        YAML::Node enemies = map_node["enemies"];
        if (enemies.size() < scenario_["enemy"].size()) {
            throw(std::runtime_error("Not enough positions for all enemies!"));
        }
        // ditto for players
        YAML::Node players = map_node["players"];
        if (players.size() < get_player_team_size()) {
            throw(std::runtime_error("Not enough positions for all players!"));
        }
        assert(enemies.IsSequence() && players.IsSequence());

        for (auto enemy_coord : enemies) {
            enemy_positions_.push_back(coordinates(enemy_coord[0].as<size_t>(), enemy_coord[1].as<size_t>()));
        }
        for (auto player_coord : players) {
            player_positions_.push_back(coordinates(player_coord[0].as<size_t>(), player_coord[1].as<size_t>()));
        }

        return map;

    } catch (std::exception &e) {
        throw std::runtime_error("Invalid map data: " + std::string(e.what()));
    }
}

int ScenarioLoader::get_player_team_size() {
    try {
        return scenario_["team_size"].as<int>();
    } catch (std::exception &e) {
        throw std::runtime_error("Could not find team size: " + std::string(e.what()));
    }
}

bool ScenarioLoader::get_multiplayer() {
    try {
        return scenario_["multiplayer"].as<bool>();
    } catch (std::exception &e) {
        return false;
    }
}