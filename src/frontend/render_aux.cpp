#include "render_aux.hpp"

Render_Aux::Render_Aux(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map) {}


bool Render_Aux::load(const std::string& aux_texture_path, const std::string& text_font_path) {
    if (!highlight_text.loadFromFile(aux_texture_path)) {
        return false;
    }
    if (!text_font_.loadFromFile(text_font_path)) {
        return false;
    }

    int tileDim = tile_map_->get_TileDim();
    std::pair<int,int> x0y0 = tile_map_->get_x0y0();
    double scale = tileDim / highlight_text.getSize().y;

    //Setting up sprite for highlight_unit_
    highlight_unit_.setOrigin(x0y0.first,x0y0.second);
    highlight_unit_.setTexture(highlight_text);
    highlight_unit_.setScale(scale,scale);

    //Setting up sprite for highlight_cursor_
    highlight_cursor_.setOrigin(x0y0.first,x0y0.second);
    highlight_cursor_.setTexture(highlight_text);
    highlight_cursor_.setScale(scale,scale);

    //Setting up cursor text.
    action_info_text_.setFont(text_font_);
    action_info_text_.setFillColor(sf::Color::Red);
    action_info_text_.setOutlineColor(sf::Color::Black);
    action_info_text_.setCharacterSize(12);
    action_info_text_.setOutlineThickness(3);

    //Setting up log text.
    log_text_.setFont(text_font_);
    log_text_.setFillColor(sf::Color::White);
    log_text_.setCharacterSize(16);
    log_text_.setOrigin(-10,-10);

    //Setting up victory text.
    victory_text_.setFont(text_font_);
    victory_text_.setFillColor(sf::Color::White);
    victory_text_.setCharacterSize(50);
    victory_text_.setOrigin(-20,-200);

    hide_cursor_highlight();
    hide_unit_highlight();
    return true;
}

void Render_Aux::update() {
    return; //Does nothing in case of this class. Just required in the parent class.
}

void Render_Aux::show_unit_highlight(const coordinates<size_t>& coords) {
    show_highlight(coords,highlight_unit_,1);
}

void Render_Aux::show_cursor_highlight(int pixel_x, int pixel_y) {
    show_highlight(tile_map_->get_map_coords(pixel_x,pixel_y),highlight_cursor_,2);
}

void Render_Aux::show_cursor_highlight(const coordinates<size_t>& coords) {
    show_highlight(coords,highlight_cursor_,2);
}

void Render_Aux::hide_unit_highlight() {
    hide_highlight(highlight_unit_);
}

void Render_Aux::hide_cursor_highlight() {
    hide_highlight(highlight_cursor_);
}

void Render_Aux::show_highlight(const coordinates<size_t>& coords, sf::Sprite& highlight_sprite, size_t texture_idx) {
    if (!tile_map_->is_inside_map_tile(coords) || !tile_map_->is_tile_drawn(coords)) {
        //Hide cursor but not unit highlight
        hide_cursor_highlight(); 
        // hide_unit_highlight(); 
        return;
    }
    int tileDim = tile_map_->get_TileDim();
    std::pair<int,int> x0y0 = tile_map_->get_x0y0();
    highlight_sprite.setTextureRect(sf::IntRect(highlight_text.getSize().y*texture_idx,0,highlight_text.getSize().y,highlight_text.getSize().y));
    switch (texture_idx) {
    case 1:
        highlight_sprite.setPosition(x0y0.first+coords.x*tileDim,x0y0.second+coords.y*tileDim-tileDim/2);
        break;
    case 2:
        highlight_sprite.setPosition(x0y0.first+coords.x*tileDim,x0y0.second+coords.y*tileDim);
        break;
    }
    return;
}

void Render_Aux::show_cursor_text(int pixel_x, int pixel_y, const std::string& msg) {
    bool cursor_inside_map = tile_map_->is_inside_map_pixel(pixel_x,pixel_y);
    bool tile_visible = tile_map_->is_tile_drawn(tile_map_->get_map_coords(pixel_x,pixel_y));
    if (cursor_inside_map && tile_visible) {
        action_info_text_.setString(msg);
        action_info_text_.setPosition(pixel_x+50,pixel_y);
    } else {
        clear_cursor_text();
    }
}

void Render_Aux::show_logs(const std::string& logs) {
    log_text_.setString(logs);
    return;
}


void Render_Aux::clear_movement_range_rects() {
    movement_range_rects_.clear();
}

void Render_Aux::update_movement_range(const std::vector<coordinates<size_t>>& coordinates) {
    clear_movement_range_rects();
    movement_range_rects_.reserve(coordinates.size());

    std::pair<float, float> x0y0 = tile_map_->get_x0y0();
    int tileDim = tile_map_->get_TileDim();

    for (const auto& coord : coordinates) {
        sf::RectangleShape rect(sf::Vector2f(tileDim, tileDim));
        std::pair<int, int> pixel_coords = tile_map_->get_tile_coords(coord.x, coord.y);
        rect.setPosition(pixel_coords.first, pixel_coords.second);
        rect.setFillColor({0, 255, 0, 128});

        movement_range_rects_.push_back(std::move(rect));
    }
}

void Render_Aux::clear_cursor_text() {
    action_info_text_.setString("");
    return;
}

void Render_Aux::clear_logs() {
    log_text_.setString("");
    return;
}

void Render_Aux::hide_highlight(sf::Sprite& highlight_sprite) {
    highlight_sprite.setTextureRect(sf::IntRect(highlight_text.getSize().y*0,0,highlight_text.getSize().y,highlight_text.getSize().y));
}

std::weak_ptr<Tile_Map> Render_Aux::get_tile_map() { return tile_map_; }

void Render_Aux::set_tile_map(std::shared_ptr<Tile_Map>& tile_map) { tile_map_ = tile_map; }

void Render_Aux::show_victory_text(Team& team, int window_width, int window_height) {

    std::stringstream msg;
    msg << "Team " << team.get_id() << " won!!!!!!!!";
    victory_text_.setString(msg.str());
    
    int x = window_width / 2 - victory_text_.getLocalBounds().width / 2;
    int y = window_height / 2 - victory_text_.getLocalBounds().height / 2;

    victory_text_.setOrigin(-x, -y);
}
