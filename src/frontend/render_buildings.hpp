#ifndef RENDER_BUILDINGS_HPP
#define RENDER_BUILDINGS_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include "building.hpp"
#include "auxiliary_renderable.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>


/**
 * @brief A rendering class used to render buildings.
 */
class Render_Buildings: public Auxiliary_renderable {
public:
    Render_Buildings(std::shared_ptr<Tile_Map>& tile_map);

    /**
     * @brief Initializes all drawable objects in this class. Nothing can be drawn before this method is called.
     * 
     * @param buildings_texture_path Path to .png file that contains textures for aux sprites.

     * @returns bool. Will return false if invalid paths are given as parameters.
     */

    bool load(const std::string& buildings_texture_path);

    /**
     * @brief Used to keep positions of all sprites and text objects up to date. This needs to be called on every tick.
     */
    void update() override;

    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

private:
    std::shared_ptr<Tile_Map> tile_map_;
    std::unordered_map<std::shared_ptr<Building>,sf::Sprite> building_sprite_map_; //Connects a building ptr to a sprite.
    sf::Texture buildings_text; //Contains all textures for buildings. Initialized on load.
    
    /**
     * @brief Calculates positions for all building sprites. Updates building textures if necessary.
     */
    void update_building_positions_and_textures();

    /**
     * @brief Makes sure that building_sprite_map_ contains all buildings located in map and everyone of them has a texture.
     */
    void update_sprite_map();

    /**
     * @brief Used to draw all drawables on a sf::RenderWindow.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (auto spr : building_sprite_map_) {
            target.draw(spr.second,states);
        }
    }
};


#endif
