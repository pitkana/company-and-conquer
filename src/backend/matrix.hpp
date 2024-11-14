#pragma once

#include <vector>
#include <cstdint>
#include <cassert>

#include "coordinates.hpp"



/**
 * @brief class definition for a Matrix class that contains a std::vector
 * as the underlying container. The class is more used as a grid based container 
 * and not used to make mathematical matrix operations on its data.
 */
template<typename T>
class Matrix
{
    private: 
        std::vector<T> data_;
        size_t width_ = 0;
        size_t height_ = 0;


    public:
        // initialise a n x n matrix
        Matrix( size_t n ) noexcept : data_(n * n), width_(n), height_(n) { }

        // initialise a n x m matrix
        Matrix( size_t height, size_t width ) noexcept : data_(height * width), width_(width), height_(height) { }


        // initialise a n x n matrix with the <value> at every cell
        Matrix( size_t n, T value ) noexcept : data_(n * n, value), width_(n), height_(n) { }


        // initialise a n x m matrix with the <value> initialised at every cell
        Matrix( size_t height, size_t width, T value ) noexcept : data_(height * width, value), width_(width), height_(height) { }

        [[nodiscard]]
        constexpr size_t size() const noexcept
        {
            return width_ * height_;
        }

        [[nodiscard]]
        constexpr size_t width() const noexcept
        {
            return width_;
        }

        [[nodiscard]]
        constexpr size_t height() const noexcept
        {
            return height_;
        }


        // works the same way as accessing with [i][j] as std::vector<std::vector<T>>
        [[nodiscard]]
        constexpr T& operator () ( const size_t y, const size_t x ) noexcept
        {
            //assert(y < height_ && x < width_);
            return data_[ y * width_ + x ];
        }

        // works the same way as accessing with [i][j] as std::vector<std::vector<T>>
        [[nodiscard]]
        constexpr const T& operator () ( const size_t y, const size_t x ) const noexcept
        {
            //assert(y < height_ && x < width_);
            return data_[ y * width_ + x ];
        }

        // this is a simpler way of accessing [y][x] by using a set of coordinates
        template<typename D>
        [[nodiscard]]
        constexpr inline T& operator [] ( const coordinates<D>& a_coordinates ) noexcept
        {
            //assert(a_coordinates.y >= 0 && a_coordinates.y < height_ && a_coordinates.x >= 0 && a_coordinates.x < width_);
            return data_[ a_coordinates.y * width_ + a_coordinates.x ];
        }

        template<typename D>
        [[nodiscard]]
        constexpr inline const T& operator [] ( const coordinates<D>& a_coordinates ) const noexcept
        {
            //assert(a_coordinates.y >= 0 && a_coordinates.y < height_ && a_coordinates.x >= 0 && a_coordinates.x < width_);
            return data_[ a_coordinates.y * width_ + a_coordinates.x ];
        }

        template<typename D>
        [[nodiscard]]
        constexpr inline T& operator () ( const coordinates<D>& a_coordinates ) noexcept
        {
            //assert(a_coordinates.y >= 0 && a_coordinates.y < height_ && a_coordinates.x >= 0 && a_coordinates.x < width_);
            return data_[ a_coordinates.y * width_ + a_coordinates.x ];
        }

        template<typename D>
        [[nodiscard]]
        constexpr inline const T& operator () ( const coordinates<D>& a_coordinates ) const noexcept
        {
            //assert(a_coordinates.y >= 0 && a_coordinates.y < height_ && a_coordinates.x >= 0 && a_coordinates.x < width_);
            return data_[ a_coordinates.y * width_ + a_coordinates.x ];
        }

        std::vector<T>::iterator begin() { return data_.begin(); }

        std::vector<T>::iterator end() { return data_.end(); }

        std::vector<T>::const_iterator cbegin() const { return data_.cbegin(); } 

        std::vector<T>::const_iterator cend() const { return data_.cend(); } 

};
