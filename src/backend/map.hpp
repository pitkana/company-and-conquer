#ifndef MAP
#define MAP
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <random>

#include "square.hpp"


/**
 * @brief Main class that will hold every other object of the game in the backend
 * 
 */
class Map
{
    private:
        int32_t amount_of_players = 0;

        /*
        * We will create a board into this container ( NOTE: you can specify a custom board size ).
        * The size_t in the Square template is used to define the template type for helper::coordinates,
        * so basically the maximum coordinate value. 
        */
        std::vector< std::vector< std::shared_ptr< Square<size_t> > > > all_squares;

        
        // we define the directions from the helper tools that we'll use in wave function collapse
        std::vector< helper::Directions > directions = { 
            helper::Directions::North, 
            helper::Directions::East, 
            helper::Directions::South, 
            helper::Directions::West 
            };


    public:
        /**
         * @brief Construct a new Map object, it uses the initialiser list to initialise the board and add the Squares
         * 
         * @param size amount of squares
         */
        Map( const size_t& size ) : all_squares(size, std::vector< std::shared_ptr<Square<size_t>> >(size) )
        {
            this->create_board();
        }



        // add the new Squares into the board
        constexpr inline void create_board() noexcept
        {

            // with this nested loop we create all the squares
            for ( size_t i = 0; i < this->all_squares.size(); i++ ) {
                for ( size_t j = 0; j < this->all_squares.size(); j++ ) {

                    // here we use normal initialisation because if we use std::make_shared the objects
                    // wont be deleted until all the weak pointers go out of scope.
                    // Because I use std::weak_ptr's in square, I cannot use std::make_shared
                    all_squares[i][j] = std::shared_ptr<Square<size_t>>( new Square<size_t>({0, 0}, { }) );

                }
            }

            return;
        }

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent square to return
         * @param direction given direction
         * @return std::vector< std::shared_ptr<Square<size_t> >> 
         */
        std::vector< std::shared_ptr<Square<size_t> >> get_neighbor(helper::coordinates<size_t>& location, helper::Directions direction)
        {
            helper::coordinates<int64_t> new_location;

            std::vector< std::shared_ptr<Square<size_t> >> possible_locations;

            switch ( direction ) {
                case helper::Directions::North:
                    if ( location.y > 0 ) {
                        possible_locations.push_back( this->all_squares[location.x][location.y - 1] );
                    }

                case helper::Directions::East:
                    if ( location.x < this->all_squares.size() - 1) {
                        possible_locations.push_back( this->all_squares[location.x + 1][location.y] );
                    }

                case helper::Directions::South:
                    if ( location.y < this->all_squares.size() - 1 ) {
                        possible_locations.push_back( this->all_squares[location.x][location.y + 1] );
                    }

                case helper::Directions::West:
                    if ( location.x > 0 ) {
                        possible_locations.push_back( this->all_squares[location.x - 1][location.y] );
                    }
            }

            return possible_locations;
        }

};



#endif