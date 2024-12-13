#include "render_units.hpp"

Render_Units::Render_Units(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map) {}


bool Render_Units::load(const std::string& unit_texture_path) {
    if (!unit_text.loadFromFile(unit_texture_path)) {
        return false;
    }

    Game& game = *tile_map_->GetGame().lock();
    std::pair<int,int> x0y0 = tile_map_->Getx0y0();
    int tileDim = tile_map_->GetTileDim();
    int text_idx = 1;
    int textW = unit_text.getSize().y;
    double scale = tileDim / textW;

    //Creating a sprite for each unit.
    for (auto& team : game.get_teams()) {
        team_id_text_idx_map_[team.get_id()] = text_idx;
        text_idx++;
        for (auto& unit : team.get_units()) {
            unit_sprite_map_[&unit] = sf::Sprite();
            sf::Sprite& sprite = unit_sprite_map_[&unit];
            sprite.setOrigin(x0y0.first,x0y0.second);
            sprite.setTexture(unit_text);
            sprite.setScale(scale,scale);
        }
    }
    update_unit_positions_and_textures();
    return true;
}

void Render_Units::update() {
    update_unit_positions_and_textures();
}

void Render_Units::update_unit_positions_and_textures() {
    Map& map = tile_map_->GetMap();
    std::pair<int,int> x0y0 = tile_map_->Getx0y0();
    int tileDim = tile_map_->GetTileDim();

    for (auto& unit_spr : unit_sprite_map_) {
        //Update postion.
        coordinates<size_t> coords = map.get_unit_location(unit_spr.first);
        std::pair<int,int> pixel_coords = tile_map_->get_tile_coords(coords.y,coords.x);
        sf::Vector2i spr_coords = sf::Vector2i(coords.x*tileDim,coords.y*tileDim);
        unit_spr.second.setPosition(x0y0.first+spr_coords.x,x0y0.second+spr_coords.y);

        //Update texture.
        int text_idx = 0;
        int textW = unit_text.getSize().y;
        if (tile_map_->is_tile_drawn(coords)) {
            int unit_team_id = tile_map_->GetGame().lock()->get_unit_team_id(unit_spr.first->get_id());
            text_idx = (unit_spr.first->is_dead()) ? 3 : team_id_text_idx_map_[unit_team_id];
        }
        unit_spr.second.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
    }
}

void Render_Units::clear() {
    unit_sprite_map_.clear();
    tile_map_.reset();
    team_id_text_idx_map_.clear();
}

std::weak_ptr<Tile_Map> Render_Units::get_tile_map() { return tile_map_; }

void Render_Units::set_tile_map(std::shared_ptr<Tile_Map>& tile_map) { tile_map_ = tile_map; }
