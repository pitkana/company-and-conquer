#include "render_buildings.hpp"

Render_Buildings::Render_Buildings(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map), x0y0_(tile_map->Getx0y0()), tileDim_(tile_map->GetTileDim()) {}


bool Render_Buildings::load(const std::string& buildings_texture_path) {
    if (!buildings_text.loadFromFile(buildings_texture_path)) {
        return false;
    }
    update_sprite_map();
    draw_buildings();
    return true;
}

void Render_Buildings::update() {
    std::pair<int,int> map_x0y0 = tile_map_->Getx0y0();
    int map_tile_dim = tile_map_->GetTileDim();
    x0y0_ = map_x0y0;
    tileDim_ = map_tile_dim;
    update_sprite_map();
    draw_buildings();
}

void Render_Buildings::update_sprite_map() {
    Map& map = tile_map_->GetMap();
    if (map.get_all_buildings().size() == building_sprite_map_.size()) return;
    for (auto& building : map.get_all_buildings()) {
        if (building_sprite_map_.contains(building)) continue;
        int textW = buildings_text.getSize().y;
        building_sprite_map_[building] = sf::Sprite();
        sf::Sprite& sprite = building_sprite_map_[building];
        sprite.setOrigin(x0y0_.first,x0y0_.second);
        sprite.setTexture(buildings_text);
        double scale = tileDim_ / textW;
        sprite.setScale(scale,scale);
    }
}

void Render_Buildings::draw_buildings() {
    Map& map = tile_map_->GetMap();
    for (auto& building_spr : building_sprite_map_) {
        coordinates<size_t> coords = map.get_building_location(building_spr.first);
        int textW = buildings_text.getSize().y;
        if (!tile_map_->is_inside_map_tile(coords)) continue;
        int text_idx = (tile_map_->is_tile_drawn(coords)) ? building_spr.first->get_texture_idx() : 0;
        std::pair<int,int> pixel_coords = tile_map_->get_tile_coords(coords.y,coords.x);
        sf::Vector2i spr_coords = sf::Vector2i(coords.x*tileDim_,coords.y*tileDim_);
        building_spr.second.setPosition(x0y0_.first+spr_coords.x,x0y0_.second+spr_coords.y);
        building_spr.second.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
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