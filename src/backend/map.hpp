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
#include <bitset>
#include <unordered_map>


#include "terrain.hpp"
#include "matrix.hpp"
#include "timer.hpp"



/**
 * @brief Main class that will hold every other object of the game in the backend
 * 
 */ 
class Map
{
    private:
        // base tiles
        // here we use normal initialisation because if we use std::make_shared the objects
        // wont be deleted until all the weak pointers go out of scope.
        // Because I use std::weak_ptr's in Terrain, I cannot use std::make_shared
        const std::shared_ptr< Terrain > background_ = std::shared_ptr< Terrain>( new Terrain('.') );
        const std::shared_ptr< Terrain > wall_ = std::shared_ptr< Terrain>( new Terrain('#', false, false, false, false) );
        const std::shared_ptr< Terrain > swamp_ = std::shared_ptr< Terrain>( new Terrain('-', 3) );

        std::unordered_map<char, std::shared_ptr< Terrain >> all_tile_types_ = { 
            {'.', this->background_ }, 
            {'#', this->wall_ }, 
            {'-', this->swamp_ }
        };

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

        void update_terrain(char terrain, size_t y, size_t x)
        {
            // thought about using switch statement, but IMO
            // it can become tedious if we keep adding 
            // different terrains so I used unordered_map for now.
            if ( all_tile_types_.count( terrain ) ) {
                all_terrains_(y, x) = all_tile_types_[terrain];
            }
            else {
                all_terrains_(y, x) = background_;
            }
            
        }

        std::shared_ptr<Terrain> get_terrain(size_t y, size_t x) 
        {
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
                    all_terrains_(i, j) = this->background_;

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
            possible_locations.reserve(4);

            // get the coordinates vectors of every direction
            for ( const coordinates<int32_t>& a_direction : directions_vectors_ ) {
                
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
            const coordinates<int64_t> aux = {static_cast<int64_t>(location.x) + direction.x, static_cast<int64_t>(location.y) + direction.y};

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
            Timer timer;
            // this will contain the distance and predecessor of a vertex as: <distance, location of predecessor>
            struct a_vertex
            {
                size_t first;
                coordinates<size_t> second;

                inline bool operator < ( const a_vertex& a ) const noexcept
                {
                    return first < a.first;
                }
                inline bool operator > ( const a_vertex& a ) const noexcept
                {
                    return first > a.first;
                }
            };
            // cant use unordered_set with coordinates without making a hash function so I used a vector.
            std::vector<bool> is_processed( width_ * height_, false ); 

            // this will contain the distance and predecessor of each vertex as: <distance, location of predecessor>
            Matrix< a_vertex > vertex_attributes(width_, height_, { std::numeric_limits<size_t>::max(), coordinates<size_t>{0, 0} });
            vertex_attributes( location.y, location.x ) = { 0, location };


            auto Relax = [&vertex_attributes, this]( const a_vertex& curr, const coordinates<size_t>& a_neighbour, size_t weight ) -> void
            {
                if ( ( curr.first + weight < vertex_attributes( a_neighbour.y, a_neighbour.x ).first ) && (this->all_terrains_[a_neighbour]->can_move_to()) ) {
                    //vertex_attributes( a_neighbour.y, a_neighbour.x ) = { vertex_attributes( curr.y, curr.x ).first + weight, curr };
                    vertex_attributes( a_neighbour.y, a_neighbour.x ).first = curr.first + weight;
                    vertex_attributes( a_neighbour.y, a_neighbour.x ).second = curr.second;
                }
            };

            
            a_vertex curr;
            coordinates<size_t> aux; // well use this in the following loop

            // very interesting template constructor for std::priority_queue, we need it to make it possible to use std::pair in it
            // it basically orders the pairs by the first element into a min-heap
            std::priority_queue< a_vertex, std::vector<a_vertex>, std::greater<a_vertex> > distances;
            distances.push( vertex_attributes( location.y, location.x ) );

            std::vector< coordinates<size_t> > tiles_that_are_close_enough;

            while ( !(distances.empty()) ) {
                curr = distances.top();
                distances.pop();

                // if the top value (so the one with the smallest weight ) is more than movement range,
                // then we know that there cant be other tiles that the player can go to and
                // we stop running
                if ( curr.first > movement_range ) {
                    break;
                }

                // add the tile's coordinates into the return container only if their distance 
                // is equal or less than the given <movement_range>
                tiles_that_are_close_enough.emplace_back( curr.second.x, curr.second.y );
                

                // check if we've already computed the current vertex
                if ( !(is_processed[ curr.second.y * width_ + curr.second.x ]) ) {

                    // the tile is only connected to 4 other tiles in the main directions
                    for ( const coordinates<int32_t>& a_direction : directions_vectors_ ) {

                        // check if the direction is valid before doing the relaxation of path
                        // created this <valid_direction> method to not put all the if-statements into one clutter
                        if ( valid_direction( curr.second, a_direction ) ) {

                            // for different directions we have a different increment in the indexing
                            aux = curr.second + a_direction;

                            // check if we've already processed the tile
                            if ( !(is_processed[ aux.y * height_ + aux.x ] )) {   
                                Relax( curr, aux, all_terrains_( aux.y, aux.x )->movement_cost() );

                                distances.emplace( vertex_attributes( aux.y, aux.x ).first, aux );
                            }
                        }
                    }

                    is_processed[ curr.second.y * width_ + curr.second.x ] = true;
                }

                
            }
            

            return tiles_that_are_close_enough;
        }


        // Breadth-first-search based algorithm on finding movement tiles
        std::vector< coordinates< size_t > > possible_tiles_to_move_to3( const coordinates<size_t>& location, uint8_t movement_range ) {
            Timer timer;
            struct Vertex {
                size_t cooldown;
                coordinates<size_t> coords;
            };

            std::vector<bool> visited(width_ * height_, false);
            //initialize result vector with the given location
            std::vector<coordinates<size_t>> result(1, location);

            // Queue of vertices that will be visited in the search.
            std::deque<Vertex> vertex_queue;
            // List of vertices that have a movement cost greater than 1 and thus won't be searched until that many movements have been used
            std::list<Vertex> waiting_vertices;

            // Add starting location to queue and mark it as visited
            vertex_queue.emplace_back(0, location);
            visited[location.y * width_ + location.x] = true;
            uint8_t movements_left = movement_range;

            while (movements_left > 0) {
                // Store how many tiles to visit in this movement
                int current_size = vertex_queue.size();
            
                for (int i = 0; i < current_size; i++) {
                    const Vertex& current_vertex = vertex_queue.front();

                    std::vector<coordinates<size_t>> neighbours = get_neighbouring_coordinates(current_vertex.coords);
                    for (const auto& neighbour : neighbours) {
                        // Get this neighbour's terrain, check if the neighbour was already visited or if you can move to it.
                        // If visited or can't move, skip it, otherwise visit it and mark it as visited
                        const std::shared_ptr<Terrain>& neighbour_terrain = all_terrains_[neighbour];
                        if (visited[neighbour.y * width_ + neighbour.x]|| !neighbour_terrain->can_move_to()) continue;
                        visited[neighbour.y * width_ + neighbour.x] = true;

                        // If it costs only 1 movement action to move into this tile, add it to the queue straight away.
                        // Otherwise add it to waiting_vertices
                        if (neighbour_terrain->movement_cost() > 1) {
                            waiting_vertices.emplace_back(neighbour_terrain->movement_cost(), neighbour);
                        } else {
                            vertex_queue.emplace_back(0, neighbour);
                            result.push_back(neighbour);
                        }
                    }
                    vertex_queue.pop_front();
                }
                
                // Loop through waiting vertices, decrease each vertex's cooldown and if it reaches 0, remove it from waiting_vertices
                // and add it to the queue of vertices to visit on the next movement
                auto it = waiting_vertices.begin();
                while (it != waiting_vertices.end()) {
                    auto waiting_vertex_it = it++;
                    Vertex& waiting_vertex = *waiting_vertex_it;
                    if (--waiting_vertex.cooldown <= 0) {
                        result.push_back(waiting_vertex.coords);
                        vertex_queue.push_back(std::move(waiting_vertex));
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
