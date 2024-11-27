#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include "game.hpp"
#include "string"
#include "map.hpp"
#include "tile_map.hpp"
#include "render_map.hpp"

/**
 * This class handles all rendering.
 * This is an initial implementation.
 */
class Render_Window {
public:
    /**
     * @param game A pointer to the game object
     * @param texture_path A path to the texture file.
     */
    Render_Window(Game* game, const std::string& texture_path);

    /**
     * @brief Creates a sfml window and draws stuff to it.
     * 
     * @param window_width The width of the sfml window in pixels.
     * @param window_height The height of the sfml window in pixels.
     */
    void spawn_window(int window_width, int window_height);

private:
    /**
     * @brief Keyinputs can be implemented with a bunch of if statements in sfml.
     * 
     * @param moveSpeed Determines how much the tiles will be moved.
     * @param zoom How fast the zoom will be.
     */
    void key_inputs(Tile_Map& tile_map,float moveSpeed, float zoom);
    /**
     * @brief Events are also handled with bunch of if statements in sfml.
     */
    void events(Tile_Map& tile_map, sf::RenderWindow& target, sf::Event event);
    Game* game_;
    std::string text_path_;
};


#endif