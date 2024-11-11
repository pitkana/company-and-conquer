#pragma once

#include <optional>
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <algorithm>
#include <iterator>

#include "helper_tools.hpp"
#include "coordinates.hpp"



class Terrain
{
    private:
        // indeces for accessing <terrain_properties_>
        static const size_t can_shoot_through_ = 0;
        static const size_t can_see_through_ = 1;
        static const size_t can_walk_through = 2;
        static const size_t can_build_in = 3;


        int32_t texture_idx_ = 0;
        size_t movement_cost_ = 1;
        std::vector<bool> terrain_properties_; // special version of std::vector that should be implemented as a bitset
        char character_repr_;



    public:

        Terrain(char repr = '.') : terrain_properties_(4, true), character_repr_(repr) { }

        Terrain(char repr, bool can_shoot, bool can_see, bool can_walk, bool can_build) : terrain_properties_(4, true), character_repr_(repr) { 
            terrain_properties_[can_shoot_through_] = can_shoot;
            terrain_properties_[can_see_through_] = can_see;
            terrain_properties_[can_walk_through] = can_walk;
            terrain_properties_[can_build_in] = can_build;
        }


        constexpr int32_t texture() { return texture_idx_; }

        constexpr size_t movement_cost() { return movement_cost_; }

        constexpr bool can_move_to() 
        {
            return ( terrain_properties_[can_walk_through] );
        }

        char get_repr() const {return character_repr_;}

        

};
