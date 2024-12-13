#include "scenario.hpp"

#include "game.hpp"

std::shared_ptr<Game> Scenario::generate_game() {
    Team player_team = shop_.form_team();

    std::shared_ptr<Game> game = std::make_shared<Game>(map_);



    game->add_team(player_team);
    game->add_team(enemy_team_);
    std::vector<Team>& teams = game->get_teams();

    int player_team_id = teams[teams.size() - 2].get_id();
    int enemy_team_id = teams[teams.size() - 1].get_id();


    std::vector<Unit>& player_units = game->get_team_by_id(player_team_id).get_units();
    std::vector<Unit>& enemy_units = game->get_team_by_id(enemy_team_id).get_units();


    for (unsigned int i = 0; i < player_units.size(); i++) {
        Unit* player_unit = &player_units[i];
        coordinates player_position = player_positions_[i];
        game->get_map().add_unit(player_position, player_unit);
    }
    for (unsigned int i = 0; i < enemy_units.size(); i++) {
        Unit* enemy_unit = &enemy_units[i];
        coordinates enemy_position = enemy_positions_[i];
        game->get_map().add_unit(enemy_position, enemy_unit);
    }

    // Enemy is AI
    if (!multiplayer_)
        game->set_ai_controlled_team(enemy_team_id);


    return game;
}
