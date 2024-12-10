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

    render_window_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "Game");

    tile_map_ = std::make_shared<Tile_Map>( game_, 100 );

    r_map_ = std::make_shared<Render_Map>( tile_map_ );

    renderables_ = std::make_shared<Window_To_Render>();
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
    Unit* jack = new Unit("Jack");

    jack->add_item(ConstItem::medic_tent_tent);
    game_->get_map().add_unit(1, 1, jack);
    // store the <terrain_vec> data into the new game object
    builder_.load(terrain_vec, game_->get_map());

    // store the pointer to the new level into the <tile_map_>, and
    // add it into the renderable
    tile_map_->SetGame( game_ );
    r_map_->set_tile_map( tile_map_ );

    auto inv = std::make_shared<Inventory_UI>( game_ );
    inv->update();

    renderables_->add_drawable( inv );

}

void Renderer::start()
{
    window_ = Rendering_Engine(game_, TEXTURE_PATH);
    window_.render( width_, height_, *render_window_, *r_map_, *tile_map_, *this, renderables_);
}
