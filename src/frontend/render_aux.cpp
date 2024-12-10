#include "render_aux.hpp"

Render_Aux::Render_Aux(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map), x0y0_(tile_map->Getx0y0()), tileDim_(tile_map->GetTileDim()) {}


bool Render_Aux::load(const std::string& aux_texture_path) {
    if (!highlight_text.loadFromFile(aux_texture_path)) {
        return false;
    }
    double scale = tileDim_ / highlight_text.getSize().y;
    //Setting up sprite for highlight_unit_
    //highlight_unit_.setTextureRect(sf::IntRect(textW*text_idx,0,textW,textW));
    highlight_unit_.setOrigin(x0y0_.first,x0y0_.second);
    highlight_unit_.setTexture(highlight_text);
    highlight_unit_.scale(scale,scale);
    //Setting up sprite for highlight_cursor_
    highlight_cursor_.setOrigin(x0y0_.first,x0y0_.second);
    highlight_cursor_.setTexture(highlight_text);
    highlight_cursor_.scale(scale,scale);
    hide_cursor_highlight();
    hide_unit_highlight();
    return true;
}

void Render_Aux::update() {
    std::pair<int,int> map_x0y0 = tile_map_->Getx0y0();
    int map_tile_dim = tile_map_->GetTileDim();
    if (x0y0_.first != map_x0y0.first || x0y0_.second != map_x0y0.second || tileDim_ != map_tile_dim) {
        x0y0_ = map_x0y0;
        tileDim_ = map_tile_dim;
    }
}

void Render_Aux::draw_unit_highlight(const coordinates<size_t>& coords) {
    draw_highlight(coords,highlight_unit_,1);
}

void Render_Aux::draw_cursor_highlight(const coordinates<size_t>& coords) {
    draw_highlight(coords,highlight_cursor_,2);
}

void Render_Aux::hide_unit_highlight() {
    hide_highlight(highlight_unit_);
}

void Render_Aux::hide_cursor_highlight() {
    hide_highlight(highlight_cursor_);
}

void Render_Aux::draw_highlight(const coordinates<size_t>& coords, sf::Sprite& highlight_sprite, size_t texture_idx) {
    highlight_sprite.setTextureRect(sf::IntRect(highlight_text.getSize().y*texture_idx,0,highlight_text.getSize().y,highlight_text.getSize().y));
    switch (texture_idx) {
    case 1:
        highlight_sprite.setPosition(x0y0_.first+coords.x*tileDim_,x0y0_.second+coords.y*tileDim_-tileDim_/2);
        break;
    case 2:
        highlight_sprite.setPosition(x0y0_.first+coords.x*tileDim_,x0y0_.second+coords.y*tileDim_);
        break;
    }
}

void Render_Aux::hide_highlight(sf::Sprite& highlight_sprite) {
    highlight_sprite.setTextureRect(sf::IntRect(highlight_text.getSize().y*0,0,highlight_text.getSize().y,highlight_text.getSize().y));
}

std::weak_ptr<Tile_Map> Render_Aux::get_tile_map()
{
    return tile_map_;
}

void Render_Aux::set_tile_map(std::shared_ptr<Tile_Map>& tile_map)
{
    tile_map_ = tile_map;
}