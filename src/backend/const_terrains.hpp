#ifndef SRC_BACKEND_CONST_TERRAINS_HPP_
#define SRC_BACKEND_CONST_TERRAINS_HPP_

#include <algorithm>
#include <memory>
#include <unordered_map>

#include "texture_idx.hpp"


class Terrain;

namespace ConstTerrain {

static const std::shared_ptr<const Terrain> background = std::make_shared<const Terrain>('.', true, true, true, true, 1);
static const std::shared_ptr<const Terrain> wall = std::make_shared<const Terrain>('#', false, false, false, false);
static const std::shared_ptr<const Terrain> mud = std::make_shared<const Terrain>('-', true, true, true, true, 3);
static const std::shared_ptr<const Terrain> water = std::make_shared<const Terrain>('~', true, true, false, false);
static const std::shared_ptr<const Terrain> tree = std::make_shared<const Terrain>('P', false, false, false, false);

static const std::unordered_map<char, std::shared_ptr<const Terrain>> char_to_terrain = 
    {
        {'.', background}, 
        {'#', wall}, 
        {'-', mud}, 
        {'~', water}, 
        {'P', tree}
    };

inline std::shared_ptr<const Terrain> get_terrain_from_char(char repr) {
    const auto it = char_to_terrain.find(repr);

    if (it == char_to_terrain.cend())
        return nullptr;

    return (*it).second;
}

static const std::unordered_map<char, int> char_to_texture_idx = 
    {
        {'.', TextureIdx::background_terrain}, 
        {'#', TextureIdx::wall_terrain}, 
        {'-', TextureIdx::mud_terrain}, 
        {'~', TextureIdx::water_terrain}, 
        {'P', TextureIdx::tree_terrain}
    };

inline int get_texture_idx_from_char(char repr) {
    const auto it = char_to_texture_idx.find(repr);
    if (it == char_to_texture_idx.cend())
        return 0;

    return (*it).second;
}


}
#endif
