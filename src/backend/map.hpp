#ifndef MAP
#define MAP
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_set>
#include <queue>
#include <utility>
#include <limits>


#include "terrain.hpp"
#include "matrix.hpp"



/**
 * @brief Main class that will hold every other object of the game in the backend
 * 
 */ 
class Map
{
    private:
        size_t width_ = 0;
        size_t height_ = 0;

        /*
        * We will create a board into this container ( NOTE: you can specify a custom board size ).
        * The size_t in the Terrain template is used to define the template type for coordinates,
        * so basically the maximum coordinate value. 
        */
        Matrix< std::shared_ptr< Terrain<size_t> >> all_terrains_;


        
        // we define the directions from the helper tools that we'll use in directions handling
        std::vector< Helper::Directions > directions_ = { 
            Helper::Directions::North, 
            Helper::Directions::East, 
            Helper::Directions::South, 
            Helper::Directions::West 
        };


    public:
        /**
         * @brief Construct a new Map object
         * 
         * @param width width of the map
         * @param height the height of the map
         */
        Map( const size_t width, const size_t height ) : all_terrains_( width, height )
        {
            this->create_board();
        }


        /**
         * @brief Construct a new Map object, it uses the initialiser list to initialise the board and add the Terrains
         * 
         * @param size amount of Terrains
         */
        Map( const size_t size ) : all_terrains_(size)
        {
            this->create_board();
        }



        // add the new Terrains into the board
        constexpr inline void create_board() noexcept
        {

            // with this nested loop we create all the Terrains
            for ( size_t i = 0; i < this->all_terrains_.width(); i++ ) {
                for ( size_t j = 0; j < this->all_terrains_.height(); j++ ) {

                    // here we use normal initialisation because if we use std::make_shared the objects
                    // wont be deleted until all the weak pointers go out of scope.
                    // Because I use std::weak_ptr's in Terrain, I cannot use std::make_shared
                    all_terrains_(i, j) = std::shared_ptr< Terrain<size_t> >( new Terrain<size_t>() );

                }
            }

            return;
        }

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent Terrain to return
         * @param direction given direction
         * @return std::shared_ptr< Terrain<size_t> >
         */
        std::shared_ptr<Terrain<size_t>> get_neighbor( const coordinates<size_t>& location, Helper::Directions direction )
        {
            coordinates<int64_t> new_location;

            std::shared_ptr< Terrain<size_t> > possible_location = nullptr;

            switch ( direction ) {
                case Helper::Directions::North:
                    if ( location.y > 0 ) {
                        possible_location = this->all_terrains_( location.x, location.y - 1 );
                    }
                    break;

                case Helper::Directions::East:
                    if ( location.x < this->all_terrains_.width() - 1) {
                        possible_location = this->all_terrains_( location.x + 1, location.y );
                    }
                    break;

                case Helper::Directions::South:
                    if ( location.y < this->all_terrains_.width() - 1 ) {
                        possible_location = this->all_terrains_( location.x, location.y + 1 );
                    }
                    break;

                case Helper::Directions::West:
                    if ( location.x > 0 ) {
                        possible_location = this->all_terrains_( location.x - 1, location.y );
                    }
                    break;
            }

            return possible_location;
        }


        /**
         * @brief Get the neighbors of a given location from all the main directions
         * 
         * @param location location of whose adjacent Terrains to return
         * @return std::vector< std::shared_ptr<Terrain<size_t> >> 
         */
        std::vector< std::shared_ptr<Terrain<size_t> >> get_neighbors( const coordinates<size_t>& location )
        {

            std::vector< std::shared_ptr<Terrain<size_t> >> possible_locations;

            std::shared_ptr<Terrain<size_t>> a_neighbor;

            for ( Helper::Directions a_direction : directions_ ) {
                a_neighbor = get_neighbor( location, a_direction );
                if ( a_neighbor ) {
                    possible_locations.push_back( a_neighbor );
                }
            }

            return possible_locations;
        }

        /**
         * @brief a nice-to-have method for checking if the direction we want to go to is valid.
         * Implemented here so that code in other parts is shorter.
         * @param location the current location from where we want to move
         * @param direction the direction we want to go to
         * @return <true> there is a tile in the given direction
         * @return <false> if there is not a tile 
         */
        constexpr inline bool valid_direction( const coordinates<size_t>& location, const Helper::Directions direction )
        {
            switch ( direction ) {
                case Helper::Directions::North:
                    if ( location.y > 0 ) {
                        // we do a nested if-statement because ordering of evaluations is not quaranteed in older c++ versions
                        // and we might be out of range when trying to access a tile
                        if ( all_terrains_( location.x, location.y - 1 )->can_move_to() ) {
                            return true;
                        }
                        
                    }
                    break;

                case Helper::Directions::East:
                    if ( location.x < this->all_terrains_.width() - 1) {
                        if ( all_terrains_( location.x + 1, location.y )->can_move_to() ) {
                            return true;
                        }
                    }
                    break;

                case Helper::Directions::South:
                    if ( location.y < this->all_terrains_.height() - 1 ) {
                        if ( all_terrains_( location.x, location.y + 1 )->can_move_to() ) {
                            return true;
                        }
                    }
                    break;

                case Helper::Directions::West:
                    if ( location.x > 0 ) {
                        if ( all_terrains_( location.x - 1, location.y)->can_move_to() ) {
                            return true;
                        }
                    }
                    break;
            }
            

            return false;
        }


        // converts the window coordinates given as coordinates<int64_t> into a Terrains coordinates<int64_t>, this new coordinates<int64_t> can then be used
        // to get the corresponding Terrain
        coordinates<int64_t> convert_pos( const int& x, const int& y, const int64_t& screen_width, const int64_t& screen_height, bool use_clamp = true ) noexcept
        {
            int square_width = screen_width/this->all_terrains_.width();
            int square_height = screen_height/this->all_terrains_.height();

            int x1 = x/square_width;
            int y1 = y/square_height;

            if ( use_clamp ) {
                x1 = Helper::clamp<size_t>(x1, 0, this->all_terrains_.width());
                y1 = Helper::clamp<size_t>(y1, 0, this->all_terrains_.height());
            }

            

            return coordinates<int64_t>{x1, y1};
        }

        /**
         * @brief get all the possible tiles that the unit can move to from the current location
         * 
         * @param location Original the units current location
         * @param movement_range the amount that the unit can traverse
         * @return std::vector< coordinates< size_t > > all the coordinates of the tiles that the unit con go to
         */
        
        std::vector< coordinates< size_t > > possible_tiles_to_move_to( const coordinates<size_t>& location, const uint8_t movement_range )
        {   
            // cant use unordered_set with coordinates without making a hash function so I used a vector
            std::vector<bool> is_processed( width_ * height_ );

            // this will contain the distance and predecessor of each vertex as: <distance, location of predecessor>
            Matrix< std::pair<size_t, coordinates<size_t>> > vertex_attributes(width_, height_, std::make_pair( std::numeric_limits<size_t>::max(), coordinates<size_t>{0, 0} ));
            vertex_attributes( location.x, location.y ) = std::make_pair( 0, location );


            auto Relax = [&vertex_attributes]( coordinates<size_t>& curr, coordinates<size_t>& a_neighbour, size_t weight ) -> void
            {
                if ( vertex_attributes( curr.x, curr.y ).first + weight < vertex_attributes( a_neighbour.x, a_neighbour.y ).first ) {
                    vertex_attributes( a_neighbour.x, a_neighbour.y ) = std::make_pair( vertex_attributes( curr.x, curr.y ).first + weight, curr );
                }
            };

            
            std::pair<size_t, coordinates<size_t>> curr;
            coordinates<size_t> aux; // well use this in the following loop

            // very interesting template constructor for std::priority_queue, we need it to make it possible to use std::pair in it
            // it basically orders the pairs by the first element into a min-heap
            std::priority_queue< std::pair<size_t, coordinates<size_t> >, std::vector<std::pair<size_t, coordinates<size_t> >> , std::less<std::pair<size_t, coordinates<size_t> >> > distances;

            while ( !(distances.empty()) ) {
                curr = distances.top();
                distances.pop();

                if ( !(is_processed[ curr.second.x * width_  + curr.second.y ]) ) {

                    // the tile is only connected to 4 other tiles in the main directions
                    for ( const Helper::Directions& a_direction : directions_ ) {

                        // check if the direction is valid before doing the relaxation of path
                        // created this <valid_direction> method to not put all the if-statements into one clutter
                        if ( valid_direction( location, a_direction ) ) {

                            // for different directions we have a different increment in the indexing
                            switch( a_direction ) {
                                case Helper::Directions::North:
                                    if ( !(is_processed[ curr.second.x * width_  + (curr.second.y - 1) ]) ) {
                                        
                                        aux = {curr.second.x, curr.second.y - 1};
                                        Relax( curr.second, aux, all_terrains_( curr.second.x, curr.second.y - 1 )->movement_cost() );

                                        distances.push( std::make_pair( vertex_attributes( aux.x, aux.y ).first, aux ) );
                                    }
                                    break;

                                case Helper::Directions::East:
                                    if ( !(is_processed[ (curr.second.x + 1) * width_  + curr.second.y ]) ) {

                                        aux = {curr.second.x + 1, curr.second.y};
                                        Relax( curr.second, aux, all_terrains_( curr.second.x, curr.second.y - 1 )->movement_cost() );

                                        distances.push( std::make_pair( vertex_attributes( aux.x, aux.y ).first, aux ) );
                                    }
                                    break;

                                case Helper::Directions::South:
                                    if ( !(is_processed[ curr.second.x * width_  + (curr.second.y + 1) ]) ) {

                                        aux = {curr.second.x, curr.second.y + 1};
                                        Relax( curr.second, aux, all_terrains_( curr.second.x, curr.second.y - 1 )->movement_cost() );

                                        distances.push( std::make_pair( vertex_attributes( aux.x, aux.y ).first, aux ) );
                                    }
                                    break;

                                case Helper::Directions::West:
                                    if ( !(is_processed[ (curr.second.x - 1) * width_  + curr.second.y ]) ) {

                                        aux = {curr.second.x - 1, curr.second.y};
                                        Relax( curr.second, aux, all_terrains_( curr.second.x, curr.second.y - 1 )->movement_cost() );

                                        distances.push( std::make_pair( vertex_attributes( aux.x, aux.y ).first, aux ) );
                                    }
                                    break;
                            }
                        }
                    }
                }

                is_processed[ curr.second.x * width_  + curr.second.y ] = true;
            }
            

            
            std::vector< coordinates<size_t> > tiles_that_are_close_enough;

            // copy the tile into the return container only if their distance 
            // is equal or less than the given <movement_range>
            // I didn't use <std::copy_if> because the original vector has
            // std::pair's so the simple for-loop is more efficient and much clearer
            for ( std::pair<size_t, coordinates<size_t> >& a_tile : vertex_attributes ) {
                if ( a_tile.first <= movement_range ) {
                    tiles_that_are_close_enough.push_back( a_tile.second );
                }
            }

            return tiles_that_are_close_enough;
        }


};


#endif
