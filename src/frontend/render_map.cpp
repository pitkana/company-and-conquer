#include "render_map.hpp"

Render_Map::Render_Map(std::shared_ptr<Tile_Map>& tile_map) : tile_map_(tile_map), tileDim_(tile_map->GetTileDim()) { }

bool Render_Map::load(const std::string& tiles) {
    if (!g_texture.loadFromFile(tiles)) {
        return false;
    }
    Map& map = tile_map_->GetMap();
    tileDim_ = tile_map_->GetTileDim();
    g_VertexArr.setPrimitiveType(sf::Quads);
    g_VertexArr.resize(4 * map.height() * map.width());
    draw_map();
    return true;
}

void Render_Map::draw_map() {
    Map& map = tile_map_->GetMap();
    int texW = g_texture.getSize().y;
    
    int mapWidth = map.width();
    int mapHeight = map.height();

    float x0 = x0y0_.first;
    float y0 = x0y0_.second;
    for (int i = 0; i <  mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            int32_t tile = (tile_map_->is_tile_drawn(i, j)) ? map.get_terrain(j,i)->texture() + 1 : 0;
            int tu = tile % (g_texture.getSize().x / g_texture.getSize().y);
            int tv = tile / (g_texture.getSize().x / g_texture.getSize().y);
            int tile_idx = (i*mapHeight+j)*4;
            //Setting up vertex positions.
            g_VertexArr[tile_idx + 0].position = sf::Vector2f(x0 + tileDim_ * i,y0 + tileDim_ * j);
            g_VertexArr[tile_idx + 1].position = sf::Vector2f(x0 + tileDim_ * (i+1),y0 + tileDim_ * j);
            g_VertexArr[tile_idx + 2].position = sf::Vector2f(x0 + tileDim_ * (i+1),y0 + tileDim_ * (j+1));
            g_VertexArr[tile_idx + 3].position = sf::Vector2f(x0 + tileDim_ * i,y0 + tileDim_ * (j+1));                
            //Setting up textures.
            g_VertexArr[tile_idx + 0].texCoords = sf::Vector2f(texW * tu,texW * tv);
            g_VertexArr[tile_idx + 1].texCoords = sf::Vector2f(texW * (tu+1),texW * tv);
            g_VertexArr[tile_idx + 2].texCoords = sf::Vector2f(texW * (tu+1),texW * (tv+1));
            g_VertexArr[tile_idx + 3].texCoords = sf::Vector2f(texW * tu,texW * (tv+1));
        }
    }
}

void Render_Map::update() {
    std::pair<int,int> map_x0y0 = tile_map_->Getx0y0();
    int map_tile_dim = tile_map_->GetTileDim();
    if (x0y0_.first != map_x0y0.first || x0y0_.second != map_x0y0.second || tileDim_ != map_tile_dim) {
        x0y0_ = map_x0y0;
        tileDim_ = map_tile_dim;
    }
    draw_map();
}

std::weak_ptr<Tile_Map> Render_Map::get_tile_map()
{
    return tile_map_;
}

void Render_Map::set_tile_map(std::shared_ptr<Tile_Map>& tile_map)
{
    tile_map_ = tile_map;
}
