#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include "render_map.hpp"
#include "game.hpp"
#include "string"
#include "map.hpp"

class Render_Window {
public:
    Render_Window(Game* game, const std::string& texture_path, const std::string& map_path);

    void spawn_window(int window_width, int window_height);

    void key_inputs(float moveSpeed, float zoom);
private:
    Render_Map r_map_;
    Game* game_;
    std::string text_path_;
    std::string map_path_;
};


#endif