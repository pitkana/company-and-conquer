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


class Renderer
{
    public:
        Renderer( size_t width, size_t height );

        void initialise_level( size_t level_idx );


        void start();

        Game& get_game() const;

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
        std::shared_ptr<sf::RenderWindow> renderable_; // contains the actual window into which we'll render stuff
};

#endif