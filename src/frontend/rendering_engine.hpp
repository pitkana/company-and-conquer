#ifndef RENDERING_ENGINE_HPP
#define RENDERING_ENGINE_HPP

#include <memory>

#include "GUI.hpp"
#include "game.hpp"
#include "string"
#include "map.hpp"
#include "tile_map.hpp"
#include "render_map.hpp"
#include "render_units.hpp"
#include "render_buildings.hpp"
#include "render_aux.hpp"
#include "game_manager.hpp"
#include "inventory_ui.hpp"
#include "window_to_render.hpp"



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
    Rendering_Engine(std::shared_ptr<Game>& game);

    /**
     * @brief draws stuff to sfml window it.
     * Works as a factory for sf::RenderWindow objects
     * @param window_width The width of the sfml window in pixels.
     * @param window_height The height of the sfml window in pixels.
     */
    void render(size_t window_width, size_t window_height, sf::RenderWindow& window, Renderer& renderer, const std::shared_ptr<Window_To_Render>& renderables);

    /**
     * @brief Used for updating the map, for example when we go to the next level
     * 
     * @param level_idx 
     * @return true if map was updated successfully
     * @return false if there was an error
     */
    bool update_map( size_t level_idx );

    /**
     * @brief Most likely only used for developing.
     */
    Game& get_game() const;

    /**
     * @brief used to update the pointer in the rendering_engine when initialising new level
     * 
     * @return std::shared_ptr<Game>& 
     */
    std::shared_ptr<Game>& get_game();

private:
    /**
     * @brief Keyinputs can be implemented with a bunch of if statements in sfml.
     * 
     * @param moveSpeed Determines how much the tiles will be moved.
     * @param zoom How fast the zoom will be.
     */
    void key_inputs(float moveSpeed, float zoom, Renderer& renderer);
    /**
     * @brief Events are also handled with bunch of if statements in sfml.
     */
    void events(/* Tile_Map& tile_map,  */sf::RenderWindow& target, sf::Event event, /* Game_Manager& manager, */ Renderer& renderer);
    std::shared_ptr<Game> game_;

    std::string text_path_;
    GUI gui_;

    // members that make it possible to handle logic, they will be obtained from the Renderer object
    // Pointers to the same objects are also stored in the Renderer object, but 
    std::shared_ptr<Render_Map> r_map_;
    std::shared_ptr<Tile_Map> tile_map_;
    std::shared_ptr<Render_Aux> r_aux_;
    std::shared_ptr<Game_Manager> manager_;


    static inline const float move_speed = 5;
    static inline const float zoom_speed = 1;

};


#endif
