#ifndef SQUARE
#define SQUARE

#include <optional>
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <algorithm>
#include <iterator>

#include "helper_tools.hpp"


template<typename T>
class Square
{
    private:
        helper::coordinates<T> position;


    public:

        Square() { }

        /**
         * @brief Construct a new Square object
         * 
         * @param coordinate The coordinates of the square
         * @param all_tiles all the possible starting tiles that we'll filter to find the right one
         */
        Square( const helper::coordinates<T>& coordinate ) : position( coordinate ) { }
        

        /**
         * @brief Construct a new Square object, this constructor uses separate x and y 
         * coordinate values in the constructor, while the other one uses helper::coordinates<T>
         * @param x the x -coordinate
         * @param y the y -coordinates
         * @param all_tiles all the possible starting tiles that we'll filter to find the right one
         */
        Square( const T& x, const T& y ) : position(x, y) { }

};

#endif