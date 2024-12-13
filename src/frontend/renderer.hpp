#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <memory>
#include "SFML/Graphics.hpp"


#include "building.hpp"
#include "game.hpp"
#include "map_builder.hpp"
#include "rendering_engine.hpp"
#include "render_units.hpp"
#include "render_buildings.hpp"
#include "render_map.hpp"
#include "window_to_render.hpp"
#include "inventory_ui.hpp"
#include "game_manager.hpp"
#include "game_logs.hpp"

class Renderer
{
    public:
        Renderer( size_t width, size_t height );

        void initialise_level( size_t level_idx );

        inline size_t width() const {
            return width_;
        }

        inline size_t height() const {
            return height_;
        }

        void start();

        Game& get_game() const;

        std::shared_ptr<Tile_Map>& get_tile_map() { return tile_map_; }
        std::shared_ptr<Render_Map>& get_r_map() { return r_map_; }
        std::shared_ptr<Render_Units>& get_r_units() { return r_units_; }
        std::shared_ptr<Render_Buildings>& get_r_buildings() { return r_buildings_; }
        std::shared_ptr<Render_Aux>& get_r_aux() { return r_aux_; }
        std::shared_ptr<Window_To_Render>& get_renderables() { return renderables_; }
        std::shared_ptr<Game_Logs>& get_logs() { return logs_; }

    private:
        size_t width_ = 0;
        size_t height_ = 0;
        size_t level_idx_ = 0; // will be used to identify the level to be loaded

        std::shared_ptr<Game> game_;  // current level
        Map_Builder builder_ = Map_Builder{};
        Rendering_Engine window_; // the class that contains logic for rendering

        std::shared_ptr<Tile_Map> tile_map_;
        std::shared_ptr<Render_Map> r_map_;
        std::shared_ptr<Render_Units> r_units_;
        std::shared_ptr<Render_Buildings> r_buildings_;
        std::shared_ptr<Render_Aux> r_aux_;
        std::shared_ptr<sf::RenderWindow> render_window_; // contains the actual window into which we'll render stuff
        std::shared_ptr<Window_To_Render> renderables_;
        std::shared_ptr<Game_Logs> logs_;

        std::string map_text_path_;
        std::string unit_text_path_;
        std::string building_text_path_;
        std::string aux_text_path_;
        std::string text_font_path_;
};

#endif
