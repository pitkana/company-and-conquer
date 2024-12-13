#include "render_buildings.hpp"

Render_Buildings::Render_Buildings(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map) {}


bool Render_Buildings::load(const std::string& buildings_texture_path) {
    if (!buildings_text.loadFromFile(buildings_texture_path)) {
        return false;
    }
    update_sprite_map();
    update_building_positions_and_textures();
    return true;
}

void Render_Buildings::update() {
    update_sprite_map();
    update_building_positions_and_textures();
    return;
}

void Render_Buildings::update_sprite_map() {
    Map& map = tile_map_->get_map();
    std::vector<std::shared_ptr<Building>> all_buildings = map.get_all_buildings();
    //If no new buildings have appeared then return.
    if (all_buildings.size() == building_sprite_map_.size()) return;

    //Otherwise initialize sprites for every building in map.
    building_sprite_map_.clear();
    int textW = buildings_text.getSize().y;
    double scale = tile_map_->get_TileDim() / textW;
    
    for (auto& building : all_buildings) {
        building_sprite_map_[building] = sf::Sprite();
        sf::Sprite& sprite = building_sprite_map_[building];
        sprite.setTexture(buildings_text);
        sprite.setScale(scale,scale);
    }
    return;
}

void Render_Buildings::update_building_positions_and_textures() {
    std::pair<int,int> x0y0 = tile_map_->get_x0y0();
    int tileDim = tile_map_->get_TileDim();
    Map& map = tile_map_->get_map();
    int textW = buildings_text.getSize().y;

    for (auto& building_spr : building_sprite_map_) {
        coordinates<size_t> coords = map.get_building_location(building_spr.first);
        if (!tile_map_->is_inside_map_tile(coords)) continue; //If somehow given invalid coords then continue.

        int text_idx = (tile_map_->is_tile_drawn(coords)) ? building_spr.first->get_texture_idx() : 0;
        sf::Vector2i spr_coords = sf::Vector2i(coords.x*tileDim,coords.y*tileDim);
        //Make sure that building position is up to date.
        building_spr.second.setPosition(x0y0.first+spr_coords.x,x0y0.second+spr_coords.y);
        //Make sure that building sprite is up to date.
        building_spr.second.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
    }
    return;
}

std::weak_ptr<Tile_Map> Render_Buildings::get_tile_map() { return tile_map_; }

void Render_Buildings::set_tile_map(std::shared_ptr<Tile_Map>& tile_map) { tile_map_ = tile_map; return; }
