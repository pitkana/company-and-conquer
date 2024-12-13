#pragma once

#include "team.hpp"
#include "map.hpp"
#include "shop.hpp"

class Scenario {

  private:
    Team enemy_team_;
    Shop shop_;
    Map map_;

    bool multiplayer_;

    std::vector<coordinates<size_t>> enemy_positions_;
    std::vector<coordinates<size_t>> player_positions_;

  public:

    Scenario(Team enemy_team, Map map, Shop shop, std::vector<coordinates<size_t>> enemy_positions, std::vector<coordinates<size_t>> player_positions, bool multiplayer):
    enemy_team_(enemy_team), shop_(shop), map_(map), multiplayer_(multiplayer), enemy_positions_(enemy_positions), player_positions_(player_positions) {}

    Shop& get_shop() { return shop_; }

    [[nodiscard]] bool get_multiplayer() const { return multiplayer_; }

    std::shared_ptr<Game> generate_game();


};
