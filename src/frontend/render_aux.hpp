#ifndef RENDER_AUX_HPP
#define RENDER_AUX_HPP

#include "tile_map.hpp"
#include "coordinates.hpp"
#include "auxiliary_renderable.hpp"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

/**
 * @brief Used for defining temporary renderables, such as 
 * an arrow to point the chosen Unit, or the highlights around a tile
 */
class Render_Aux: public Auxiliary_renderable {
public:
    /**
     * @brief Construct a new Render_Aux object
     * 
     * @param tile_map a std::shared_ptr to the Tile_Map object which will be used to handle the logic for 
     * rendering. The tile_map should be the one used with the other renderables.
     */
    Render_Aux(std::shared_ptr<Tile_Map>& tile_map);
    bool show_text = false;
    bool load(const std::string& aux_texture_path, const std::string& text_font_path);
    void update() override;
    //TODO: Implement. This could be used to update unit texture when it dies.
    //void update_textures();
    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

    /**
     * @brief Used for drawing the arrow that points which Unit is chosen
     * 
     * @param coords the coordinates of the Unit
     */
    void draw_unit_highlight(const coordinates<size_t>& coords);

    /**
     * @brief Text that is rendered next to the mouse cursor
     * 
     * @param pixel_x the x coordiante of the mouse
     * @param pixel_y the y coordinate of the mouse
     * @param msg The text to be displayed
     */
    void draw_text(int pixel_x, int pixel_y, const std::string& msg);

    /**
     * @brief Remove the text that has been rendered
     */
    void clear_text();

    /**
     * @brief Draw a highlight onto the tile on which the cursor currently is.
     * 
     * @param pixel_x the x coordinate of the mouse
     * @param pixel_y the y coordinate of the mouse
     */
    void draw_cursor_highlight(int pixel_x, int pixel_y);

    /**
     * @brief Draw a highlight onto the tile on which the cursor currently is.
     * 
     * @param coords the coordinate of the mouse
     */
    void draw_cursor_highlight(const coordinates<size_t>& coords);

    /**
     * @brief Hide the arrow highlight 
     */
    void hide_unit_highlight();

    /**
     * @brief Hide the highlight that was over the tile which the cursor was over
     */
    void hide_cursor_highlight();

private:
    sf::Sprite highlight_unit_;
    sf::Sprite highlight_cursor_;
    sf::Text action_info_text_;
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
    }
};


#endif
