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

class Render_Buildings: public Auxiliary_renderable {
public:
    Render_Buildings(std::shared_ptr<Tile_Map>& tile_map);
    bool load(const std::string& buildings_texture_path);
    void update() override;
    //TODO: Implement. This could be used to update unit texture when it dies.
    //void update_textures();
    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);
private:
    std::shared_ptr<Tile_Map> tile_map_;
    std::unordered_map<std::shared_ptr<Building>,sf::Sprite> building_sprite_map_;
    std::pair<int,int> x0y0_;
    int tileDim_;
    sf::Texture buildings_text;
    
    void draw_buildings();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (auto spr : building_sprite_map_) {
            target.draw(spr.second,states);
        }
    }
};


#endif