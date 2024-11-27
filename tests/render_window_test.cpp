#include "render_window_test.hpp"
#include "render_window.hpp"
#include "map_builder.hpp"
#include "game.hpp"

//Use WASD to move map and FG to zoom in and out.
//Clicking tile will print the coordinates of that tile.
void render_window_test() {
    Map_Builder builder = Map_Builder();
    std::vector<std::vector<char>> terrain_vec = builder.read_map_file(TESTMAP_PATH);
    size_t test_map_height = terrain_vec.size();
    size_t test_map_width = terrain_vec[0].size();
    Game g(test_map_height,test_map_width);
    Game* game = &g;
    builder.load(terrain_vec,game->get_map());
    Render_Window wind(game,TEXTURE_PATH);
    wind.spawn_window(700,500);
}