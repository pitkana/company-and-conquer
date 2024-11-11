#ifndef MAP
#define MAP
#include <vector>
#include <list>
#include <memory>
#include <cstdint>
#include <unordered_set>
#include <queue>
#include <utility>
#include <limits>
#include <iostream>


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
        Matrix< std::shared_ptr< Terrain >> all_terrains_;


        
        // we define the directions from the helper tools that we'll use in directions handling
        std::vector< Helper::Directions > directions_ = { 
            Helper::Directions::North, 
            Helper::Directions::East, 
            Helper::Directions::South, 
            Helper::Directions::West 
        };

        std::vector< coordinates<int32_t> > directions_vectors_ = { 
            {0, -1}, 
            {1, 0}, 
            {0, 1}, 
            {-1, 0} 
        };


    public:
        /**
         * @brief Construct a new Map object
         * 
         * @param width width of the map
         * @param height the height of the map
         */
        Map( const size_t width, const size_t height ) : width_(width), height_(height), all_terrains_( width, height )
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

        void update_terrain(Terrain& terrain, size_t y, size_t x) {
            all_terrains_(y, x) = std::shared_ptr<Terrain>(&terrain);
        }

        std::shared_ptr<Terrain> get_terrain(size_t y, size_t x) {
            return all_terrains_(y, x);
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
                    all_terrains_(i, j) = std::shared_ptr< Terrain >( new Terrain('.') );

                }
            }

            return;
        }

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent Terrain to return
         * @param direction given direction
         * @return std::shared_ptr< Terrain >
         */
        std::shared_ptr<Terrain> get_neighbor( const coordinates<size_t>& location, const Helper::Directions direction )
        {
            coordinates<int64_t> new_location;

            std::shared_ptr< Terrain > possible_location = nullptr;

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
         * @return std::vector< std::shared_ptr<Terrain> > 
         */
        std::vector< std::shared_ptr<Terrain> > get_neighbors( const coordinates<size_t>& location )
        {

            std::vector< std::shared_ptr<Terrain >> possible_locations;

            std::shared_ptr<Terrain> a_neighbor;

            for ( Helper::Directions a_direction : directions_ ) {
                a_neighbor = get_neighbor( location, a_direction );
                if ( a_neighbor ) {
                    possible_locations.push_back( a_neighbor );
                }
            }

            return possible_locations;
        }

        /**
         * @brief Get the neighbouring coordinates of a given location from all the main directions
         * 
         * @param location location of whose adjacent Terrains to return
         * @return std::vector< coordinates<size_t>> 
         */
        std::vector< coordinates<size_t> > get_neighbouring_coordinates( const coordinates<size_t>& location )
        {

            std::vector< coordinates<size_t> > possible_locations;

            std::shared_ptr<Terrain> a_neighbor;
// get the coordinates vectors of every direction
            for ( coordinates<int32_t> a_direction : directions_vectors_ ) {
                
                if ( valid_direction( location, a_direction ) ) {
                    // because we already checked if the direction is valid, 
                    // we can just make the sum of the location and direction
                    possible_locations.push_back( location + a_direction );
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
        inline bool valid_direction( const coordinates<size_t>& location, const coordinates<int32_t>& direction )
        {
            // have to cast so we can check if the valu goes to below 0
            coordinates<int64_t> aux = {static_cast<int64_t>(location.x) + direction.x, static_cast<int64_t>(location.y) + direction.y};

            if ( aux.x < 0 || aux.x >= this->all_terrains_.width() ) {
                return false;
            }

            else if ( aux.y < 0 || aux.y >= this->all_terrains_.height() ) {
                return false;
            }

            return true;
        }



        /**
         * @brief get all the possible tiles that the unit can move to from the current location
         * 
         * @param location Original the units current location
         * @param movement_range the amount that the unit can traverse
         * @return std::vector< coordinates< size_t > > all the coordinates of the tiles that the unit con go to
         */
        std::vector< coordinates<size_t> > possible_tiles_to_move_to( const coordinates<size_t>& location, const uint8_t movement_range )
        {   
            // cant use unordered_set with coordinates without making a hash function so I used a vector
            std::vector<bool> is_processed( width_, height_ );

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

                // check if we've already computed the current vertex
                if ( !(is_processed[ curr.second.x * width_ + curr.second.y ]) ) {

                    // the tile is only connected to 4 other tiles in the main directions
                    for ( const coordinates<int32_t>& a_direction : directions_vectors_ ) {

                        // check if the direction is valid before doing the relaxation of path
                        // created this <valid_direction> method to not put all the if-statements into one clutter
                        if ( valid_direction( curr.second, a_direction ) ) {

                            // for different directions we have a different increment in the indexing
                            aux = curr.second + a_direction;

                            // check if we've already processed the tile
                            if ( !(is_processed[ aux.x * width_ + aux.y ] )) {   
                                Relax( curr.second, aux, all_terrains_( aux.x, aux.y )->movement_cost() );

                                distances.push( std::make_pair( vertex_attributes( aux.x, aux.y ).first, aux ) );
                            }
                        }
                    }

                    is_processed[ curr.second.x * width_ + curr.second.y ] = true;
                }

                
            }
            

            
            std::vector< coordinates<size_t> > tiles_that_are_close_enough;

            // add the tile's coordinates into the return container only if their distance 
            // is equal or less than the given <movement_range>
            // I didn't use <std::copy_if> because the original vector has
            // std::pair's so the simple for-loop is more efficient and much clearer
            for ( size_t width = 0; width < vertex_attributes.width(); width++ ) {
                for ( size_t height = 0; height < vertex_attributes.height(); height++ ) {
                    if ( vertex_attributes(width, height).first <= movement_range ) {
                        tiles_that_are_close_enough.push_back( coordinates<size_t>{ width, height } );
                    }
                }
            }

            return tiles_that_are_close_enough;
        }


        std::vector< coordinates< size_t > > possible_tiles_to_move_to3( const coordinates<size_t>& location, uint8_t movement_range ) {
            struct Vertex {
                size_t cooldown;
                const coordinates<size_t>& coords;
            };

            Matrix<int> visited(width_, height_, false);

            std::vector<coordinates<size_t>> result;

            std::queue<Vertex> vertex_queue;
            vertex_queue.push({0, location});
            
            std::list<Vertex> waiting_vertices;
            uint8_t movements_left = movement_range;
            while (movements_left > 0) {
                int current_size = vertex_queue.size();
            
                for (int i = 0; i < current_size; i++) {
                    const Vertex& current_vertex = vertex_queue.front();
                    std::vector<coordinates<size_t>> neighbours = get_neighbouring_coordinates(current_vertex.coords);
                    for (const auto& neighbour : neighbours) {
                        // bool& is_visited = visited(neighbour.y, neighbour.x);
                        const std::shared_ptr<Terrain>& neighbour_terrain = all_terrains_[neighbour];
                        if (visited(neighbour.y, neighbour.x)|| !neighbour_terrain->can_move_to()) continue;
                        visited(neighbour.y, neighbour.x) = true;

                        if (neighbour_terrain->movement_cost() > 1) {
                            waiting_vertices.push_back({neighbour_terrain->movement_cost(), neighbour});
                        } else {
                            vertex_queue.push({0, neighbour});
                            result.push_back(neighbour);
                        }
                    }
                    vertex_queue.pop();
                }
                
                auto it = waiting_vertices.begin();
                while (it != waiting_vertices.end()) {
                    auto waiting_vertex_it = it++;
                    Vertex& waiting_vertex = *waiting_vertex_it;
                    if (--waiting_vertex.cooldown <= 0) {
                        vertex_queue.push(waiting_vertex);
                        result.push_back(waiting_vertex.coords);
                        waiting_vertices.erase(waiting_vertex_it);
                    }
                }
                movements_left--;
            }

            return result;
        }
        
        void print_map() const {
            for (size_t y = 0; y < height_; ++y) {
                for (size_t x = 0; x < width_; ++x) {
                    std::cout << all_terrains_(y, x)->get_repr();
                }
                std::cout << '\n';
            }

            std::cout << std::endl;
        }


};


#endif
