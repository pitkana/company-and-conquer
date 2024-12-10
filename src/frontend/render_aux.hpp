#ifndef RENDER_AUX_HPP
#define RENDER_AUX_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class Render_Aux: public sf::Drawable, public sf::Transformable {
public:
    Render_Aux(std::shared_ptr<Tile_Map>& tile_map);
    bool load(const std::string& aux_texture_path);
    void update();
    //TODO: Implement. This could be used to update unit texture when it dies.
    //void update_textures();
    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

    void draw_unit_highlight(const coordinates<size_t>& coords);
    void draw_cursor_highlight(const coordinates<size_t>& coords);
    void hide_unit_highlight();
    void hide_cursor_highlight();

private:
    sf::Sprite highlight_unit_;
    sf::Sprite highlight_cursor_;
    std::shared_ptr<Tile_Map> tile_map_;
    std::pair<int,int> x0y0_;
    int tileDim_;
    sf::Texture highlight_text;
    
    void draw_highlight(const coordinates<size_t>& coords, sf::Sprite& highlight_sprite, size_t texture_idx);
    void hide_highlight(sf::Sprite& highlight_sprite);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(highlight_unit_,states);
        target.draw(highlight_cursor_,states);
    }
};


#endif