#ifndef MAP
#define MAP
#include <vector>
#include <memory>
#include <cstdint>
#include <unordered_map>


#include "unit.hpp"
#include "terrain.hpp"
#include "matrix.hpp"
#include "timer.hpp"
#include "building.hpp"



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


        /*
        * We will create a board into this container ( NOTE: you can specify a custom board size ).
        * The size_t in the Terrain template is used to define the template type for coordinates,
        * so basically the maximum coordinate value. 
        */
        Matrix< std::shared_ptr< Terrain >> all_terrains_;
        Matrix< std::shared_ptr< Unit >> all_units_;
        Matrix< std::shared_ptr< Building >> all_buildings_;


        
        // we define the directions from the Helper tools that we'll use in directions handling
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
        Map( const size_t width, const size_t height );


        /**
         * @brief Construct a new Map object, it uses the initialiser list to initialise the board and add the Terrains
         * 
         * @param size amount of Terrains
         */
        Map( const size_t size );

        [[nodiscard]]
        constexpr inline size_t width() const;

        [[nodiscard]]
        constexpr inline size_t height() const;

        void update_terrain(char terrain, size_t y, size_t x);

        std::shared_ptr<Terrain> get_terrain(size_t y, size_t x);

        void update_terrain(char terrain, const coordinates<size_t>& coords);

        std::shared_ptr<Terrain> get_terrain(const coordinates<size_t>& coords);


        bool has_building(const coordinates<size_t>& coords) const;
        //Adds building to the given coordinates if the terrain doesn't already have a building and can be built on
        // Returns true if worked, false if not
        bool add_building(std::shared_ptr<Building> building, const coordinates<size_t>& coords);
        std::shared_ptr<Building> get_building(const coordinates<size_t>& coords);

        bool has_building(size_t y, size_t x) const;
        //Adds building to the given coordinates if the terrain doesn't already have a building and can be built on
        // Returns true if worked, false if not
        bool add_building(std::shared_ptr<Building> building, size_t y, size_t x);
        std::shared_ptr<Building> get_building(size_t y, size_t x);

        // add the new Terrains into the board
        constexpr inline void create_board() noexcept;

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent Terrain to return
         * @param direction given direction
         * @return std::shared_ptr< Terrain >
         */
        std::shared_ptr<Terrain> get_neighbor( const coordinates<size_t>& location, const Helper::Directions direction );


        /**
         * @brief Get the neighbors of a given location from all the main directions
         * 
         * @param location location of whose adjacent Terrains to return
         * @return std::vector< std::shared_ptr<Terrain> > 
         */
        std::vector< std::shared_ptr<Terrain> > get_neighbors( const coordinates<size_t>& location );

        /**
         * @brief Get the neighbouring coordinates of a given location from all the main directions
         * 
         * @param location location of whose adjacent Terrains to return
         * @return std::vector< coordinates<size_t>> 
         */
        std::vector< coordinates<size_t> > get_neighbouring_coordinates( const coordinates<size_t>& location );

        /**
         * @brief a nice-to-have method for checking if the direction we want to go to is valid.
         * Implemented here so that code in other parts is shorter.
         * @param location the current location from where we want to move
         * @param direction the direction we want to go to
         * @return <true> there is a tile in the given direction
         * @return <false> if there is not a tile 
         */
        inline bool valid_direction( const coordinates<size_t>& location, const coordinates<int32_t>& direction );



        /**
         * @brief get all the possible tiles that the unit can move to from the current location
         * 
         * @param location Original the units current location
         * @param movement_range the amount that the unit can traverse
         * @return std::vector< coordinates< size_t > > all the coordinates of the tiles that the unit con go to
         */
        std::vector< coordinates<size_t> > possible_tiles_to_move_to( const coordinates<size_t>& location, const uint8_t movement_range );


        // Breadth-first-search based algorithm on finding movement tiles
        std::vector< coordinates< size_t > > possible_tiles_to_move_to3( const coordinates<size_t>& location, uint8_t movement_range );
        
        void print_map() const;


};


#endif
