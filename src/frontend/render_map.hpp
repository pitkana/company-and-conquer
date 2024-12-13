#ifndef RENDER_MAP
#define RENDER_MAP

#include <memory>

#include "map.hpp"
#include "SFML/Graphics.hpp"
#include "terrain.hpp"
#include "tile_map.hpp"
#include "auxiliary_renderable.hpp"

/**
 * @brief A class that inherits properties from sfml:s Drawable and Transformable
 * classes. Render_Map object can be passed as a parameter to sf::RenderWindow::Draw
 * function. 
 */
class Render_Map : public Auxiliary_renderable {
public:
    /**
     * @param tile_map Can be used to access the map and the game object.
     */
    Render_Map(std::shared_ptr<Tile_Map>& tile_map);

    /**
     * @brief Constructs a sf::VertexArray along with dimensions specified in certain Map.
     * 
     * @param tiles A path to the texture file.
     * 
     * @returns True or False based on the fact if the read on the texture file is succesful.
     */
    bool load(const std::string& tiles);

    /**
     * @brief Used to keep positions of all sprites and text objects up to date. This needs to be called on every tick.
     */
    void update() override;

    std::weak_ptr<Tile_Map> get_tile_map();
    void set_tile_map(std::shared_ptr<Tile_Map>& tile_map);

private:
    sf::VertexArray tile_VertexArr_; //VertexArray that will be drawn.
    sf::Texture tile_texture_; //Contains the texture,
    std::shared_ptr<Tile_Map> tile_map_;

    /**
     * @brief Sets up the positions and textures for each vertex in tile_VertexArr.
     */
    void update_tile_position_and_textures();

    /**
     * Inherited method from parent classes.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = &tile_texture_;
        target.draw(tile_VertexArr_,states);
        return;
    }
};

#endif
