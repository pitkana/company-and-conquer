#include "render_map.hpp"

bool Render_Map::load(const std::string& tiles, int tileSize, Map map) {
    if (!g_texture.loadFromFile(tiles)) {
        return false;
    }

    mapWidth = map.width();
    mapHeight = map.height();
    g_VertexArr.setPrimitiveType(sf::Quads);
    g_VertexArr.resize(4 * mapWidth * mapHeight);

    int scale = tileSize / g_texture.getSize().y;
    
    widthHeight = sf::Vector2f(g_texture.getSize().y*scale,g_texture.getSize().y*scale);
    x0y0 = sf::Vector2i(0,0);

    float w = widthHeight.x;
    float h = widthHeight.y;

    int texW = g_texture.getSize().y;

    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {

            int32_t tile = map.get_terrain(j,i)->texture();

            int tu = tile % (g_texture.getSize().x / g_texture.getSize().y);
            int tv = tile / (g_texture.getSize().x / g_texture.getSize().y);

            g_VertexArr[(i*mapHeight+j)*4 + 0].position = sf::Vector2f(w * i,h * j);
            g_VertexArr[(i*mapHeight+j)*4 + 1].position = sf::Vector2f(w * (i+1),h * j);
            g_VertexArr[(i*mapHeight+j)*4 + 2].position = sf::Vector2f(w * (i+1),h * (j+1));
            g_VertexArr[(i*mapHeight+j)*4 + 3].position = sf::Vector2f(w * i,h * (j+1));                

            g_VertexArr[(i*mapHeight+j)*4 + 0].texCoords = sf::Vector2f(texW * tu,texW * tv);
            g_VertexArr[(i*mapHeight+j)*4 + 1].texCoords = sf::Vector2f(texW * (tu+1),texW * tv);
            g_VertexArr[(i*mapHeight+j)*4 + 2].texCoords = sf::Vector2f(texW * (tu+1),texW * (tv+1));
            g_VertexArr[(i*mapHeight+j)*4 + 3].texCoords = sf::Vector2f(texW * tu,texW * (tv+1));

        }
    }
    return true;
}

void Render_Map::moveTiles(float x, float y) {
    x0y0.x = x0y0.x + x;
    x0y0.y = x0y0.y + y;
    int l = g_VertexArr.getVertexCount();
    for (int i = 0; i < l; i++) {
        sf::Vertex& v = g_VertexArr[i];
        v.position.x = v.position.x + x;
        v.position.y = v.position.y + y;
    }
}

void Render_Map::zoom(float z) {
    widthHeight.x = widthHeight.x + z;
    widthHeight.y = widthHeight.y + z;
    x0y0.x = x0y0.x - z;
    x0y0.y = x0y0.y - z;
    float w = widthHeight.x;
    float h = widthHeight.y;
    int x0 = x0y0.x;
    int y0 = x0y0.y;
    for (int i = 0; i < mapWidth; i++) {
        for (int j = 0; j < mapHeight; j++) {
            g_VertexArr[(i*mapHeight+j)*4 + 0].position = sf::Vector2f(x0 + w * i,y0 + h * j);
            g_VertexArr[(i*mapHeight+j)*4 + 1].position = sf::Vector2f(x0 + w * (i+1),y0 + h * j);
            g_VertexArr[(i*mapHeight+j)*4 + 2].position = sf::Vector2f(x0 + w * (i+1),y0 + h * (j+1));
            g_VertexArr[(i*mapHeight+j)*4 + 3].position = sf::Vector2f(x0 + w * i,y0 + h * (j+1));
        }
    }
}

