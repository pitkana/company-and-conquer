#ifndef MAP
#define MAP
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <random>

#include "square.hpp"
#include "matrix.hpp"



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
        * The size_t in the Square template is used to define the template type for coordinates,
        * so basically the maximum coordinate value. 
        */
        Matrix< std::shared_ptr< Square<size_t> >> all_squares_;

        
        // we define the directions from the helper tools that we'll use in directions handling
        std::vector< Helper::Directions > directions_ = { 
            Helper::Directions::North, 
            Helper::Directions::East, 
            Helper::Directions::South, 
            Helper::Directions::West 
        };


    public:
        /**
         * @brief Construct a new Map object, it uses the initialiser list to initialise the board and add the Squares
         * 
         * @param size amount of squares
         */
        Map( const size_t size ) : all_squares_(size)
        {
            this->create_board();
        }



        // add the new Squares into the board
        constexpr inline void create_board() noexcept
        {

            // with this nested loop we create all the squares
            for ( size_t i = 0; i < this->all_squares_.width(); i++ ) {
                for ( size_t j = 0; j < this->all_squares_.height(); j++ ) {

                    // here we use normal initialisation because if we use std::make_shared the objects
                    // wont be deleted until all the weak pointers go out of scope.
                    // Because I use std::weak_ptr's in square, I cannot use std::make_shared
                    all_squares_(i, j) = std::shared_ptr< Square<size_t> >( new Square<size_t>(coordinates<size_t>{0, 0}) );

                }
            }

            return;
        }

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent square to return
         * @param direction given direction
         * @return std::shared_ptr< Square<size_t> >
         */
        std::shared_ptr<Square<size_t>> get_neighbor( const coordinates<size_t>& location, Helper::Directions direction )
        {
            coordinates<int64_t> new_location;

            std::shared_ptr< Square<size_t> > possible_location = nullptr;

            switch ( direction ) {
                case Helper::Directions::North:
                    if ( location.y > 0 ) {
                        possible_location = all_squares_( location.x, location.y - 1 );
                    }
                    break;

                case Helper::Directions::East:
                    if ( location.x < this->all_squares_.width() - 1) {
                        possible_location = this->all_squares_( location.x + 1, location.y );
                    }
                    break;

                case Helper::Directions::South:
                    if ( location.y < this->all_squares_.width() - 1 ) {
                        possible_location = this->all_squares_( location.x, location.y + 1 );
                    }
                    break;

                case Helper::Directions::West:
                    if ( location.x > 0 ) {
                        possible_location = this->all_squares_( location.x - 1, location.y );
                    }
                    break;
            }

            return possible_location;
        }


        /**
         * @brief Get the neighbors of a given location from all the main directions
         * 
         * @param location location of whose adjacent squares to return
         * @return std::vector< std::shared_ptr<Square<size_t> >> 
         */
        std::vector< std::shared_ptr<Square<size_t> >> get_neighbors( const coordinates<size_t>& location )
        {

            std::vector< std::shared_ptr<Square<size_t> >> possible_locations;

            std::shared_ptr<Square<size_t>> a_neighbor;

            for ( Helper::Directions a_direction : directions_ ) {
                a_neighbor = get_neighbor( location, a_direction );
                if ( a_neighbor ) {
                    possible_locations.push_back( a_neighbor );
                }
            }

            return possible_locations;
        }


        // converts the window coordinates given as coordinates<int64_t> into a squares coordinates<int64_t>, this new coordinates<int64_t> can then be used
        // to get the corresponding square
        coordinates<int64_t> convert_pos( const int& x, const int& y, const int64_t& screen_width, const int64_t& screen_height, bool use_clamp = true ) noexcept
        {
            int square_width = screen_width/this->all_squares_.width();
            int square_height = screen_height/this->all_squares_.height();

            int x1 = x/square_width;
            int y1 = y/square_height;

            if ( use_clamp ) {
                x1 = Helper::clamp<size_t>(x1, 0, this->all_squares_.width());
                y1 = Helper::clamp<size_t>(y1, 0, this->all_squares_.height());
            }

            

            return coordinates<int64_t>{x1, y1};
        }
};



#endif
