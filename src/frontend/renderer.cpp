#include "renderer.hpp"
#include "rendering_engine.hpp"


/**
 * @brief Construct a new Renderer::Renderer object that will handle the logic for rendering different parts
 * 
 * @param width the width of the window
 * @param height the height of the window
 */
Renderer::Renderer( size_t width, size_t height ) : width_(width), height_(height)
{
    game_ = std::make_shared<Game>( height, width );

    renderable_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "Game");

    tile_map_ = std::make_shared<Tile_Map>( game_, 100 );

    r_map_ = std::make_shared<Render_Map>( tile_map_ );

    r_units_ = std::make_shared<Render_Units>( tile_map_ );
}


void Renderer::initialise_level( size_t level_idx )
{
    // used for storing the tiles corresponding character in the level map 
    std::vector<std::vector<char>> terrain_vec;

    if ( level_idx == 0 ) {
        terrain_vec = builder_.read_map_file(TESTMAP_PATH);
    }
    if ( level_idx == 1 ) {
        terrain_vec = builder_.read_map_file(TESTMAP_PATH1);
    }
    

    size_t test_map_height = terrain_vec.size();
    size_t test_map_width = terrain_vec[0].size();

    // create the new Game object with the new level
    game_ = std::make_shared<Game>( test_map_height, test_map_width );

    // store the <terrain_vec> data into the new game object
    builder_.load(terrain_vec, game_->get_map());

    // store the pointer to the new level into the <tile_map_>, and
    // add it into the renderable
    tile_map_->SetGame( game_ );
    r_map_->set_tile_map( tile_map_ );
    r_units_->set_tile_map( tile_map_ );


}


void Renderer::start()
{
    window_ = Rendering_Engine(game_, TEXTURE_PATH, UNITS_TEXTURE_PATH);
    window_.render( width_, height_, *renderable_, *r_map_, *tile_map_, *r_units_, *this );
}

Game& Renderer::get_game() const {
    return *game_;
}