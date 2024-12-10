#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <memory>
#include <utility>

#include "game.hpp"
#include "coordinates.hpp"


/**
 * This class will help to render each map layer such as tiles and units together.
 */
class Tile_Map {
public:
    Tile_Map(std::shared_ptr<Game>& game, int tileDim);
    Tile_Map(std::shared_ptr<Game>& game, std::pair<float, float> x0y0, int tileDim);

    bool is_tile_drawn(size_t x, size_t y) const;

    bool is_tile_drawn(const coordinates<size_t>& coords) const;

    bool fog_of_war = true;

    //TODO: Movement needs to be capped.
    /**
     * @brief Moves all tiles.
     * 
     * @param x How much the x coordinate of a tile will be changed.
     * @param y How much the y coordinate of a tile will be changed.
     */
    void move(float x, float y);
    //TODO: this method needs to zoom in to center of the screen.
    //TODO: zoom needs to be capped.
    /**
     * @brief Zooms in/out by making each map tile bigger/smaller.
     * 
     * @param z Positive z will zoom in and negative z will zoom out.
     */
    // void zoom(int z);
    
    //This method could be used to center to camera at certain units or tiles!!
    //TODO: Make this method.
    /*
    void centerAt(int x, int y) {
        std::cout << "TODO!!" << std::endl;
    }
    */

   void center_at(const coordinates<size_t>& coords, int window_width, int window_height); 

    /**
     * @brief Transforms matrix (indexes?) into pixels coordinates.
     * 
     */
    std::pair<int, int> get_tile_coords(int x, int y) const;
    std::pair<int, int> get_tile_coords(const coordinates<size_t>& coords) const;
    /**
     * @brief Transfroms pixel coordinates into matrix (indexes?)
     */
    coordinates<size_t> get_map_coords(int pixel_x, int pixel_y) const;

    bool is_inside_map_tile(int x, int y) const;

    bool is_inside_map_tile(const coordinates<size_t>& coords) const;

    bool is_inside_map_pixel(int pixel_x, int pixel_y) const;


    int GetTileDim() const;
    /**
     * @brief Gets the top-left pixel coordinates of the entire map.
     */
    std::pair<int,int> Getx0y0() const;
    Map& GetMap() const;
    std::weak_ptr<Game> GetGame() const;
    void SetGame( std::shared_ptr<Game> game );

private:
    std::shared_ptr<Game> game_;
    std::pair<float, float> x0y0_;
    int tileDim_;
};

#endif