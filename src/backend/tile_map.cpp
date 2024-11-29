#include "tile_map.hpp"

Tile_Map::Tile_Map(std::shared_ptr<Game>& game, int tileDim) : tileDim_(tileDim) 
{
    game_ = game;
}

Tile_Map::Tile_Map(std::shared_ptr<Game>& game, std::pair<float, float> x0y0, int tileDim) : x0y0_(x0y0), tileDim_(tileDim) 
{
    game_ = game;
}

/*
void Tile_Map::move(float x, float y) {
    x0y0_.first = x0y0_.first + x;
    x0y0_.second = x0y0_.second + y;
}

void Tile_Map::zoom(int z) {
    tileDim_ = tileDim_ + z;

    if ( tileDim_ < 0 ) {
        tileDim_ = 0;
    }
}
*/

std::pair<int,int> Tile_Map::get_tile_coords(int x, int y) const {
    std::pair<int,int> crds;
    crds.first = tileDim_ * x;
    crds.second = tileDim_ * y;
    return crds;
}

std::pair<int,int> Tile_Map::get_map_coords(int x, int y) const {
    std::pair<int,int> crds;
    crds.first = x / tileDim_;
    crds.second = y / tileDim_;
    return crds;
}

int Tile_Map::GetTileDim() const {
    return tileDim_;
}
std::pair<int,int> Tile_Map::Getx0y0() const {
    return x0y0_;
}

Map& Tile_Map::GetMap() const {
    return game_->get_map();
}

std::weak_ptr<Game> Tile_Map::GetGame() const {
    return game_;
}


void Tile_Map::SetGame( std::shared_ptr<Game> game )
{
    game_ = game;
}


