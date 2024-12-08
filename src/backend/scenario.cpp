#include "scenario.hpp"

#include "game.hpp"

Game Scenario::generate_game() {
    Team player_team = shop_.form_team();

    std::vector<Unit> player_units = player_team.get_units();
    std::vector<Unit> enemy_units = enemy_team_.get_units();

    for (unsigned int i = 0; i < player_units.size(); i++) {
        Unit* player_unit = &player_units[i];
        coordinates player_position = player_positions_[i];
        map_.add_unit(player_position, player_unit);
    }
    for (unsigned int i = 0; i < enemy_units.size(); i++) {
        Unit* enemy_unit = &enemy_units[i];
        coordinates enemy_position = enemy_positions_[i];
        map_.add_unit(enemy_position, enemy_unit);
    }

    Game game = Game(map_);
    game.add_team(player_team);
    game.add_team(enemy_team_);

    return game;
}
