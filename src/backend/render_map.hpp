#ifndef RENDER_MAP
#define RENDER_MAP

#include "map.hpp"
#include "SFML/Graphics.hpp"
#include "terrain.hpp"

/**
 * A class that inherits properties from sfml:s Drawable and Transformable
 * classes. Render_Map object can be passed as a parameter to sf::RenderWindow::Draw
 * function. 
 */

class Render_Map : public sf::Drawable, public sf::Transformable {
public:

    /**
     * @brief Constructs a sf::VertexArray along with dimensions specified in certain Map.
     * 
     * @param tiles A path to the texture file.
     * @param tileSize Initial size of each map tile.
     * 
     * @returns True or False based on the fact if the read on the texture file is succesful.
     */
    bool load(const std::string& tiles, int tileSize, Map map);

    //TODO: Movement needs to be capped.
    /**
     * @brief Moves all tiles.
     * 
     * @param x How much the x coordinate of a tile will be changed.
     * @param y How much the y coordinate of a tile will be changed.
     */
    void moveTiles(float x, float y);

    //TODO: this method needs to zoom in to center of the screen.
    //TODO: zoom needs to be capped.
    /**
     * @brief Zooms in/out by making each map tile bigger/smaller.
     * 
     * @param z Positive z will zoom in and negative z will zoom out.
     */
    void zoom(float z);


    //This method could be used to center to camera at certain units!!
    //TODO: Make this method.
    /*
    void centerAt(int x, int y) {
        std::cout << "TODO!!" << std::endl;
    }
    */

private:
    sf::VertexArray g_VertexArr; //VertexArray that will be drawn.
    sf::Texture g_texture; //Contains the texture,
    sf::Vector2f widthHeight; //Current width and height for each tile.
    sf::Vector2i x0y0; //Current top-right coordinate of the entire drawn map.
    int mapWidth;
    int mapHeight;

    /**
     * Inherited method from parent classes.
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &g_texture;
        target.draw(g_VertexArr,states);
    }
};





#endif