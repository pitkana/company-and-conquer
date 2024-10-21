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
        std::optional< std::string > tile = std::nullopt;

        std::array< std::optional< char >, 8 > boundary;

        std::vector< std::string > plausible_tiles;

        helper::coordinates<T> position;


    public:

        Square() { }

        /**
         * @brief Construct a new Square object
         * 
         * @param coordinate The coordinates of the square
         * @param all_tiles all the possible starting tiles that we'll filter to find the right one
         */
        Square(const helper::coordinates<T>& coordinate, const std::vector< std::string >& all_tiles) : position( coordinate ), plausible_tiles( all_tiles ) { }
        

        /**
         * @brief Construct a new Square object, this constructor uses separate x and y 
         * coordinate values in the constructor, while the other one uses helper::coordinates<T>
         * @param x the x -coordinate
         * @param y the y -coordinates
         * @param all_tiles all the possible starting tiles that we'll filter to find the right one
         */
        Square(const T& x, const T& y, const std::vector< std::string >& all_tiles) : position(x, y), plausible_tiles( all_tiles ) { }


        /**
         * @brief Fixes a single element in the boundary and removes incompatible tiles.
         * 
         * @param boudary_index index of the element on the boundary
         * @param value value to be set in the boundary
         */
        void fix( const size_t& boudary_index, const char& value )
        {
            if ( this->boundary[boudary_index].has_value() ) {
                this->boundary[boudary_index] = value;
                
                // we create a temporary vector for the new possible tiles
                std::vector< std::string > aux;

                // we filter out the tiles that are not possible anymore, on the second line is the lambda function for filtering
                // basically the lambda function checks if the character of the possible tile at index: <boudary_index> is the same
                // as the given <value> -character
                std::copy_if( 
                    this->plausible_tiles.begin(), this->plausible_tiles.end(), std::back_inserter(aux), 
                    [boudary_index, value](std::string i)-> bool {return i[boudary_index] == value;} 
                );

                // assign the filtered vector back to the main one
                this->plausible_tiles = aux;
            }

            return;
        }

};

#endif