#ifndef RENDER_UNITS_HPP
#define RENDER_UNITS_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include "auxiliary_renderable.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

/**
 * @brief A rendering class used to render buildings.
 */
class Render_Units: public Auxiliary_renderable {
public:
    Render_Units(std::shared_ptr<Tile_Map>& tile_map);

    /**
     * @brief Initializes all drawable objects in this class. Nothing can be drawn before this method is called.
     * 
     * @param unit_texture_path Path to .png file that contains textures for unit sprites.

     * @returns bool. Will return false if invalid paths are given as parameters.
     */
    bool load(const std::string& unit_texture_path);

    /**
     * @brief Used to keep positions of all sprites and text objects up to date. This needs to be called on every tick.
     */
    void update() override;

    /**
     * @brief Used to clear the members such as the unit data so that 
     * the new units of a level can be added.
     * This way we dont have to create a new Render_Units object every time
     */
    void clear();

    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

private:
    std::shared_ptr<Tile_Map> tile_map_;
    std::unordered_map<Unit*,sf::Sprite> unit_sprite_map_; //Contains sprites for every unit found in map.
    std::unordered_map<int,int> team_id_text_idx_map_; //Assigns a textures id to a certain team id.
    sf::Texture unit_text; //Contains all textures for units.

    /**
     * @brief Makes sure that textures and postions for every unit are up to date.
     */
    void update_unit_positions_and_textures();
    //void update_textures();

    /**
     * @brief Used to draw all drawables on a sf::RenderWindow.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (auto spr : unit_sprite_map_) {
            target.draw(spr.second,states);
        }
        return;
    }
};


#endif