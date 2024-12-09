#ifndef RENDERING_ENGINE_HPP
#define RENDERING_ENGINE_HPP

#include <memory>

#include "GUI.hpp"
#include "game.hpp"
#include "string"
#include "map.hpp"
#include "tile_map.hpp"
#include "render_map.hpp"



class Renderer;

/**
 * @brief This class handles all rendering but it does not create the actual instances that will be rendered.
 * As thus this acts as an "engine" that runs the provided resources
 */
class Rendering_Engine {
public:

    Rendering_Engine() {}
    /**
     * @param game A pointer to the game object
     * @param texture_path A path to the texture file.
     */
    Rendering_Engine(std::shared_ptr<Game>& game, const std::string& texture_path);

    /**
     * @brief draws stuff to sfml window it.
     * Works as a factory for sf::RenderWindow objects
     * @param window_width The width of the sfml window in pixels.
     * @param window_height The height of the sfml window in pixels.
     */
    void render(size_t window_width, size_t window_height, sf::RenderWindow& window, Render_Map& r_map, Tile_Map& tile_map, Renderer& renderer);

    /**
     * @brief Used for updating the map, for example when we go to the next level
     * 
     * @param level_idx 
     * @return true if map was updated successfully
     * @return false if there was an error
     */
    bool update_map( size_t level_idx );

private:
    /**
     * @brief Keyinputs can be implemented with a bunch of if statements in sfml.
     * 
     * @param moveSpeed Determines how much the tiles will be moved.
     * @param zoom How fast the zoom will be.
     */
    void key_inputs(Render_Map& r_map, Renderer& renderer);
    /**
     * @brief Events are also handled with bunch of if statements in sfml.
     */
    void events(const Render_Map& render_map, sf::RenderWindow& target, sf::Event event);
    std::shared_ptr<Game> game_;
    std::string text_path_;

    GUI gui_;


    static inline const float move_speed = 5;
    static inline const float zoom_speed = 1;
};


#endif
