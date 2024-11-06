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


template<typename T>
class Terrain
{
    private:
        static const uint16_t can_shoot_through_ = 0;
        static const uint16_t can_see_through_ = 1;
        static const uint16_t can_walk_through = 2;
        static const uint16_t can_build_in = 3;


        int32_t texture_idx_ = 0;
        int32_t movement_cost_ = 1;
        std::vector<bool> square_properties_;



    public:

        Square() : square_properties_(4) { }


        // returns true if this square contains a piece, and false if the square's empty
        constexpr inline bool has_piece() noexcept
        {
            // we call the bool() operator of std::shared_ptr
            return (this->container) ? true : false;
        }


        // check whether 2 given Squares have the same coordinates<int64_t>,
        // if they have, then this operator considers them the same Square
        constexpr inline bool operator == ( Square a_square ) noexcept
        {
            return { this->position.x == a_square.coordinates().x && this->position.y == a_square.coordinates().y };
        }

        constexpr int32_t texture() { return texture_idx_; }

        

};
