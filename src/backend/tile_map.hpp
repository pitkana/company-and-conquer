#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include "game.hpp"
#include <utility>


class Tile_Map {
public:
    Tile_Map(Game& game, int tileDim);
    Tile_Map(Game& game, std::pair<int,int> x0y0, int tileDim);
    void move(int x, int y);
    void zoom(int z);
    std::pair<int,int> get_tile_coords(int x, int y) const;
    std::pair<int,int> get_map_coords(int x, int y) const;
    int GetTileDim() const;
    std::pair<int,int>& Getx0y0() const;
    Map& GetMap() const;
    Game& GetGame() const;
private:
    Game& game_;
    std::pair<int,int> x0y0_;
    int tileDim_;
};

#endif