#ifndef MAP
#define MAP
#include <vector>
#include <memory>
#include <cstdint>
#include <functional>
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
        const std::shared_ptr< Terrain> window_ = std::make_shared<Terrain>('O', false, true, false, false);

        std::unordered_map<char, std::shared_ptr<Terrain> > all_tile_types_ = { 
            {'.', this->background_ }, 
            {'#', this->wall_ }, 
            {'-', this->swamp_ },
            {'O', this->window_}
        };


        /*
        * We will create a board into this container ( NOTE: you can specify a custom board size ).
        * The size_t in the Terrain template is used to define the template type for coordinates,
        * so basically the maximum coordinate value. 
        */
        Matrix< std::shared_ptr< Terrain >> all_terrains_;
        //Raw pointer since the map doesn't have ownership of units
        Matrix< Unit* > all_units_;
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
        constexpr inline size_t width() const 
        {
            return all_terrains_.width();
        }

        [[nodiscard]]
        constexpr inline size_t height() const 
        {
            return all_terrains_.height();
        }

        void update_terrain(char terrain, size_t y, size_t x);

        std::shared_ptr<Terrain> get_terrain(size_t y, size_t x);

        void update_terrain(char terrain, const coordinates<size_t>& coords);

        std::shared_ptr<Terrain> get_terrain(const coordinates<size_t>& coords);


        bool has_building(size_t y, size_t x) const;
        bool has_building(const coordinates<size_t>& coords) const;

        //Adds building to the given coordinates if the terrain doesn't already have a building and can be built on
        // Returns true if worked, false if not
        bool add_building(std::shared_ptr<Building> building, size_t y, size_t x);
        bool add_building(std::shared_ptr<Building> building, const coordinates<size_t>& coords);

        bool remove_building(size_t y, size_t x);
        bool remove_building(const coordinates<size_t>& coords);

        std::shared_ptr<Building> get_building(size_t y, size_t x);
        std::shared_ptr<Building> get_building(const coordinates<size_t>& coords);

        bool has_weapon_building(size_t y, size_t x);
        bool has_weapon_building(const coordinates<size_t>& coords);

        bool has_healing_building(size_t y, size_t x);
        bool has_healing_building(const coordinates<size_t>& coords);

        bool can_build_on(size_t y, size_t x) const;
        bool can_build_on(const coordinates<size_t>& coords) const;

        bool can_move_to_terrain(size_t y, size_t x) const;
        bool can_move_to_terrain(const coordinates<size_t>& coords) const;

        bool can_move_to_coords(size_t y, size_t x) const;
        bool can_move_to_coords(const coordinates<size_t> coords) const;

        bool has_unit(size_t y, size_t x) const;
        bool has_unit(const coordinates<size_t>& coords) const;

        Unit* get_unit(size_t y, size_t x);
        Unit* get_unit(const coordinates<size_t>& coords);

        coordinates<size_t> get_unit_location(Unit* unit_ptr) const;

        bool add_unit(size_t y, size_t x, Unit* unit);
        bool add_unit(const coordinates<size_t>& coords, Unit* unit);

        /**
         * @brief Sets the ptr on the specified coordinates to null.
         *
         * @return bool, true if there was a unit there, false if not
         */
        bool remove_unit(size_t y, size_t x);
        bool remove_unit(const coordinates<size_t>& coords);

        /**
         * @brief Moves unit from origin coordinates to destination coordinates
         *
         * @param origin_y
         * @param origin_x
         * @param dest_y
         * @param dest_x
         * @return bool True if unit could be moved (was a unit at origin, was not a unit at destination, destination terrain can be moved to), false otherwise
         */
        bool move_unit(size_t origin_y, size_t origin_x, size_t dest_y, size_t dest_x);
        bool move_unit(const coordinates<size_t>& origin, const coordinates<size_t>& dest);

        


        // add the new Terrains into the board
        constexpr inline void create_board() noexcept;

        /**
         * @brief Get the neighbor of a given location from a specified direction
         * 
         * @param location location of whose adjacent Terrain to return
         * @param direction given direction
         * @return std::shared_ptr<Terrain>
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
         * @brief Uses Andres circle drawing algorithm to get the 
         * location of the furthest tile that the unit can see
         * @param location the location of the unit
         * @param visibility_range the distance to which the unit can see
         * @return std::vector< coordinates<size_t> > 
         */
        std::vector< coordinates<size_t> > max_visible_locations( const coordinates<size_t>& location, const uint32_t visibility_range );



        std::vector<coordinates<size_t>> get_aoe_affected_coords(const coordinates<size_t>& location, const uint32_t range);

        /**
         * @brief Checks which coordinates around the player in specified range are 'visible' (not blocked by coordinates that return false from predicate function)
         * 
         * @param location location from which the check is done
         * @param range The distance to which the unit can see
         * @param predicate function that takes coordinates and returns false if it stops LoS
         * @return std::vector< coordinates<size_t> > the tiles that are within LoS
         */
        std::vector< coordinates<size_t> > line_of_sight_check( const coordinates<size_t>& location, const uint32_t range, const std::function<bool(int64_t y, int64_t x)>& predicate);
        

        /**
         * @brief Used for the fog of war feature
         * 
         * @param location the units location
         * @param visibility_range The distance to which the unit can see
         * @return std::vector< coordinates<size_t> > the tiles that the unit sees
         */
        std::vector< coordinates<size_t> > tiles_unit_sees( const coordinates<size_t>& location, const uint32_t visibility_range );

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

        coordinates<size_t> get_closest_accessible_tile(const coordinates<size_t>& location);

        coordinates<size_t> fastest_movement_to_target(const coordinates<size_t>& location, coordinates<size_t> target, uint8_t movement_range);
        
        void print_map() const;

        void print_units() const;

        void print_buildings() const;


};


#endif
