#include "render_buildings.hpp"

Render_Buildings::Render_Buildings(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map), x0y0_(tile_map->Getx0y0()), tileDim_(tile_map->GetTileDim()) {}


bool Render_Buildings::load(const std::string& buildings_texture_path) {
    if (!buildings_text.loadFromFile(buildings_texture_path)) {
        return false;
    }
    Map& map = tile_map_->GetMap();
    int textW = buildings_text.getSize().y;
    for (auto building : map.get_all_buildings()) {
        int text_idx = building->get_texture_idx();
        building_sprite_map_[building] = sf::Sprite();
        sf::Sprite& sprite = building_sprite_map_[building];
        sprite.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
        sprite.setOrigin(x0y0_.first,x0y0_.second);
        sprite.setTexture(buildings_text);
        double scale = tileDim_ / textW;
        sprite.scale(scale,scale);
    }
    draw_buildings();
    return true;
}

void Render_Buildings::update() {
    std::pair<int,int> map_x0y0 = tile_map_->Getx0y0();
    int map_tile_dim = tile_map_->GetTileDim();
    if (x0y0_.first != map_x0y0.first || x0y0_.second != map_x0y0.second || tileDim_ != map_tile_dim) {
        x0y0_ = map_x0y0;
        tileDim_ = map_tile_dim;
        draw_buildings();
    }
}


void Render_Buildings::draw_buildings() {
    Map& map = tile_map_->GetMap();
    for (auto& building_spr : building_sprite_map_) {
        coordinates<size_t> coords = map.get_building_location(building_spr.first);
        if (coords.x != -1 && coords.y != -1) {
            std::pair<int,int> pixel_coords = tile_map_->get_tile_coords(coords.y,coords.x);
            sf::Vector2i spr_coords = sf::Vector2i(coords.x*tileDim_,coords.y*tileDim_);
            building_spr.second.setPosition(x0y0_.first+spr_coords.x,x0y0_.second+spr_coords.y);
        } //else { could do something here... }
    }
}

std::weak_ptr<Tile_Map> Render_Buildings::get_tile_map()
{
    return tile_map_;
}

void Render_Buildings::set_tile_map(std::shared_ptr<Tile_Map>& tile_map)
{
    tile_map_ = tile_map;
}