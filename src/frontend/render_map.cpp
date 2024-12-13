#include "render_map.hpp"

Render_Map::Render_Map(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map) { }

bool Render_Map::load(const std::string& tiles) {
    if (!tile_texture_.loadFromFile(tiles)) {
        return false;
    }
    Map& map = tile_map_->GetMap();
    int tileDim = tile_map_->GetTileDim();
    tile_VertexArr_.setPrimitiveType(sf::Quads);
    tile_VertexArr_.resize(4 * map.height() * map.width());
    update_tile_position_and_textures();
    return true;
}

void Render_Map::update_tile_position_and_textures() {
    Map& map = tile_map_->GetMap();
    int texW = tile_texture_.getSize().y;

    int mapWidth = map.width();
    int mapHeight = map.height();

    std::pair<float,float> x0y0 = tile_map_->Getx0y0();
    int tileDim = tile_map_->GetTileDim();
    float x0 = x0y0.first;
    float y0 = x0y0.second;
    //This implementation follows pretty closely sfml tutorial made with triangles instead of quads:
    //https://www.sfml-dev.org/tutorials/2.6/graphics-vertex-array.php
    for (int i = 0; i <  mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            int32_t tile = (tile_map_->is_tile_drawn(i, j)) ? map.get_terrain(j,i)->texture() : 0;
            int texture_x = tile % (tile_texture_.getSize().x / tile_texture_.getSize().y);
            int texture_y = tile / (tile_texture_.getSize().x / tile_texture_.getSize().y);
            int tile_idx = (i*mapHeight+j)*4;
            //Setting up vertex positions.
            tile_VertexArr_[tile_idx + 0].position = sf::Vector2f(x0 + tileDim * i,y0 + tileDim * j);
            tile_VertexArr_[tile_idx + 1].position = sf::Vector2f(x0 + tileDim * (i+1),y0 + tileDim * j);
            tile_VertexArr_[tile_idx + 2].position = sf::Vector2f(x0 + tileDim * (i+1),y0 + tileDim * (j+1));
            tile_VertexArr_[tile_idx + 3].position = sf::Vector2f(x0 + tileDim * i,y0 + tileDim * (j+1));                
            //Setting up textures.
            tile_VertexArr_[tile_idx + 0].texCoords = sf::Vector2f(texW * texture_x,texW * texture_y);
            tile_VertexArr_[tile_idx + 1].texCoords = sf::Vector2f(texW * (texture_x+1),texW * texture_y);
            tile_VertexArr_[tile_idx + 2].texCoords = sf::Vector2f(texW * (texture_x+1),texW * (texture_y+1));
            tile_VertexArr_[tile_idx + 3].texCoords = sf::Vector2f(texW * texture_x,texW * (texture_y+1));
        }
    }
    return;
}

void Render_Map::update() {
    update_tile_position_and_textures();
    return;
}

std::weak_ptr<Tile_Map> Render_Map::get_tile_map() { return tile_map_; }

void Render_Map::set_tile_map(std::shared_ptr<Tile_Map>& tile_map) { tile_map_ = tile_map; return; }
