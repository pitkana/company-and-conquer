#ifndef RENDER_AUX_HPP
#define RENDER_AUX_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include "auxiliary_renderable.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class Render_Aux: public Auxiliary_renderable {
public:
    Render_Aux(std::shared_ptr<Tile_Map>& tile_map);
    bool show_text = false;
    bool load(const std::string& aux_texture_path, const std::string& text_font_path);
    void update() override;
    //TODO: Implement. This could be used to update unit texture when it dies.
    //void update_textures();
    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

    void draw_unit_highlight(const coordinates<size_t>& coords);
    void draw_text(int pixel_x, int pixel_y, const std::string& msg);
    void draw_logs(const std::string& logs);
    void clear_logs();
    void clear_cursor_text();
    void draw_cursor_highlight(int pixel_x, int pixel_y);
    void draw_cursor_highlight(const coordinates<size_t>& coords);
    void hide_unit_highlight();
    void hide_cursor_highlight();

private:
    sf::Sprite highlight_unit_;
    sf::Sprite highlight_cursor_;
    sf::Text action_info_text_;
    sf::Text log_text_;
    std::shared_ptr<Tile_Map> tile_map_;
    std::pair<int,int> x0y0_;
    int tileDim_;
    sf::Texture highlight_text;
    sf::Font text_font_;
    
    void draw_highlight(const coordinates<size_t>& coords, sf::Sprite& highlight_sprite, size_t texture_idx);
    void hide_highlight(sf::Sprite& highlight_sprite);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(highlight_unit_,states);
        target.draw(highlight_cursor_,states);
        target.draw(action_info_text_);
        target.draw(log_text_);
    }
};

#endif
