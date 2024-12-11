#include "render_units.hpp"

Render_Units::Render_Units(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map), x0y0_(tile_map->Getx0y0()), tileDim_(tile_map->GetTileDim()) {}


bool Render_Units::load(const std::string& unit_texture_path) {
    if (!unit_text.loadFromFile(unit_texture_path)) {
        return false;
    }
    Game& game = *tile_map_->GetGame().lock();
    int text_idx = 1;
    int textW = unit_text.getSize().y;
    for (auto& team : game.get_teams()) {
        team_id_text_idx_map_[team.get_id()] = text_idx;
        text_idx++;
        for (auto& unit : team.get_units()) {
            unit_sprite_map_[&unit] = sf::Sprite();
            sf::Sprite& sprite = unit_sprite_map_[&unit];
            //sprite.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
            sprite.setOrigin(x0y0_.first,x0y0_.second);
            sprite.setTexture(unit_text);
            double scale = tileDim_ / textW;
            sprite.setScale(scale,scale);
        }
    }
    draw_units();
    return true;
}

void Render_Units::update() {
    std::pair<int,int> map_x0y0 = tile_map_->Getx0y0();
    int map_tile_dim = tile_map_->GetTileDim();
    if (x0y0_.first != map_x0y0.first || x0y0_.second != map_x0y0.second || tileDim_ != map_tile_dim) {
        x0y0_ = map_x0y0;
        tileDim_ = map_tile_dim;
    }
    draw_units();
}

void Render_Units::draw_units() {
    Map& map = tile_map_->GetMap();
    for (auto& unit_spr : unit_sprite_map_) {
        coordinates<size_t> coords = map.get_unit_location(unit_spr.first);
        if (coords.x < 0 || coords.y < 0) assert(false);
        std::pair<int,int> pixel_coords = tile_map_->get_tile_coords(coords.y,coords.x);
        sf::Vector2i spr_coords = sf::Vector2i(coords.x*tileDim_,coords.y*tileDim_);
        unit_spr.second.setPosition(x0y0_.first+spr_coords.x,x0y0_.second+spr_coords.y);
        int text_idx = 0;
        int textW = unit_text.getSize().y;
        if (tile_map_->is_tile_drawn(coords)) {
            int unit_team_id = tile_map_->GetGame().lock()->get_unit_team_id(unit_spr.first->get_id());
            text_idx = (unit_spr.first->is_dead()) ? 3 : team_id_text_idx_map_[unit_team_id];
        }
        unit_spr.second.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
    }
}

void Render_Units::clear()
{
    unit_sprite_map_.clear();
    tile_map_.reset();
    tileDim_ = 0;
    team_id_text_idx_map_.clear();
}

std::weak_ptr<Tile_Map> Render_Units::get_tile_map()
{
    return tile_map_;
}

void Render_Units::set_tile_map(std::shared_ptr<Tile_Map>& tile_map)
{
    tile_map_ = tile_map;
}
