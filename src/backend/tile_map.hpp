#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include "game.hpp"
#include <utility>

/**
 * This class will help to render each map layer such as tiles and units together.
 */
class Tile_Map {
public:
    Tile_Map(Game& game, int tileDim);
    Tile_Map(Game& game, std::pair<int,int> x0y0, int tileDim);
    //TODO: Movement needs to be capped.
    /**
     * @brief Moves all tiles.
     * 
     * @param x How much the x coordinate of a tile will be changed.
     * @param y How much the y coordinate of a tile will be changed.
     */
    void move(int x, int y);
    //TODO: this method needs to zoom in to center of the screen.
    //TODO: zoom needs to be capped.
    /**
     * @brief Zooms in/out by making each map tile bigger/smaller.
     * 
     * @param z Positive z will zoom in and negative z will zoom out.
     */
    void zoom(int z);
    //This method could be used to center to camera at certain units or tiles!!
    //TODO: Make this method.
    /*
    void centerAt(int x, int y) {
        std::cout << "TODO!!" << std::endl;
    }
    */
    /**
     * @brief Transforms matrix (indexes?) into pixels coordinates.
     * 
     */
    std::pair<int,int> get_tile_coords(int x, int y) const;
    /**
     * @brief Transfroms pixel coordinates into matrix (indexes?)
     */
    std::pair<int,int> get_map_coords(int x, int y) const;
    int GetTileDim() const;
    /**
     * @brief Gets the top-left pixel coordinates of the entire map.
     */
    std::pair<int,int> Getx0y0() const;
    Map& GetMap() const;
    Game& GetGame() const;
private:
    Game& game_;
    std::pair<int,int> x0y0_;
    int tileDim_;
};

#endif