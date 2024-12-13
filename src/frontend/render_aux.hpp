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
 * @brief A rendering class used to render everything else other than units,tiles,buildings.
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

    /**
     * @brief Initializes all drawable objects in this class. Nothing can be drawn before this method is called.
     * 
     * @param aux_texture_path Path to .png file that contains textures for aux sprites.
     * @param text_font_path Path to a .tff font file used in rendering text.
     * 
     * @returns bool. Will return false if invalid paths are given as parameters.
     */
    bool load(const std::string& aux_texture_path, const std::string& text_font_path);

    /**
     * @brief Used to keep positions of all sprites and text objects up to date. This needs to be called on every tick.
     */
    void update() override;

    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

    /**
     * @brief Makes highlight_unit_ sprite appear around desired location.
     * 
     * @param coords The desired location.
     */
    void show_unit_highlight(const coordinates<size_t>& coords);

    /**
     * @brief Makes action_info_text_ show msg string around desired location. Usually mouse pixel coordinates in this case.
     */
    void show_cursor_text(int pixel_x, int pixel_y, const std::string& msg);

    /**
     * @brief Makes log_text_ show logs string in the top left corner of the screen.
     */
    void show_logs(const std::string& logs);

    /**
     * @brief Hides logs.
     */
    void clear_logs();

    /**
     * @brief Hides cursor text.
     */
    void clear_cursor_text();

    /**
     * @brief Makes highlight_cursor_ sprite appear around desired location. Pixel coordinates in this case.
     */
    void show_cursor_highlight(int pixel_x, int pixel_y);
    void show_cursor_highlight(const coordinates<size_t>& coords);

    /**
     * @brief Hides highlight_unit_ sprite.
     */
    void hide_unit_highlight();

    /**
     * @brief Hides highlight_cursor_ sprite.
     */
    void hide_cursor_highlight();
    void update_movement_range(const std::vector<coordinates<size_t>>& coordinates);
    void clear_movement_range_rects();

    /**
     * @brief Updates victory text to show which team won.
     */
    void show_victory_text(Team& team);

private:
    sf::Sprite highlight_unit_; //Marks active unit in gui.
    sf::Sprite highlight_cursor_; //Marks the tile below cursor in gui.
    sf::Text action_info_text_; //Text that appears next cursor and gives some information to the player.
    sf::Text log_text_; //Shows executed game actions to the user.
    sf::Text victory_text_;
    std::shared_ptr<Tile_Map> tile_map_;
    sf::Texture highlight_text;
    sf::Font text_font_;

    std::vector<sf::RectangleShape> movement_range_rects_;
    

    /**
     * @brief A generalized method used to show a sprite in certain map coords.
     */
    void show_highlight(const coordinates<size_t>& coords, sf::Sprite& highlight_sprite, size_t texture_idx);

    /**
     * @brief A generalized method used to hide a sprite.
     */
    void hide_highlight(sf::Sprite& highlight_sprite);

    /**
     * @brief Used to draw drawables on a sf::RenderWindow.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        for (const auto& rect : movement_range_rects_) {
            target.draw(rect);
        }
        target.draw(highlight_unit_,states);
        target.draw(highlight_cursor_,states);
        target.draw(action_info_text_);
        target.draw(log_text_);
        target.draw(victory_text_);
    }
};

#endif
