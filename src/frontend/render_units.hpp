#ifndef RENDER_UNITS_HPP
#define RENDER_UNITS_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class Render_Units: public sf::Drawable, public sf::Transformable {
public:
    Render_Units(std::shared_ptr<Tile_Map>& tile_map);
    bool load(const std::string& unit_texture_path);
    void update();
    //TODO: Implement. This could be used to update unit texture when it dies.
    //void update_textures();
    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);
private:
    std::shared_ptr<Tile_Map> tile_map_;
    std::unordered_map<Unit*,sf::Sprite> unit_sprite_map_;
    std::pair<int,int> x0y0_;
    int tileDim_;
    sf::Texture unit_text;
    
    void draw_units();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (auto spr : unit_sprite_map_) {
            target.draw(spr.second,states);
        }
    }
};


#endif