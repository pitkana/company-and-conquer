#include <memory>
#include <algorithm>
#include <queue>
#include <utility>
#include <set>
#include <vector>
#include <limits>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <list>
#include <cmath>

#include "map.hpp"
#include "helper_tools.hpp"

Map::Map( const size_t width, const size_t height ) : all_terrains_( height, width ), all_units_(height, width), all_buildings_(height, width)
{
    this->create_board();
}


Map::Map( const size_t size ) : all_terrains_(size), all_units_(size), all_buildings_(size)
{
    this->create_board();
}


void Map::update_terrain(char terrain, size_t y, size_t x)
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



void Map::update_terrain(char terrain, const coordinates<size_t>& coords) {
    update_terrain(terrain, coords.y, coords.x);
}




std::shared_ptr<Terrain> Map::get_terrain(size_t y, size_t x) 
{
    return all_terrains_(y, x);
}


std::shared_ptr<Terrain> Map::get_terrain(const coordinates<size_t>& coords) {
    return get_terrain(coords.y, coords.x);
}


bool Map::has_building(size_t y, size_t x) const {
    return all_buildings_(y, x) != nullptr;
}


bool Map::has_building(const coordinates<size_t> &coords) const {
    return has_building(coords.y, coords.x);
}


bool Map::add_building(std::shared_ptr<Building> building, size_t y, size_t x) {
    if (!has_building(y, x) && get_terrain(y, x)->can_build_on()) {
        all_buildings_(y, x) = building;
        return true;
    }

    return false;
}


bool Map::add_building(std::shared_ptr<Building> building, const coordinates<size_t> &coords) {
    return add_building(building, coords.y, coords.x);
}

bool Map::remove_building(size_t y, size_t x) {
    if (has_building(y, x)) {
        all_buildings_(y, x) = nullptr;
        return true;
    }

    return false;
}

bool Map::remove_building(const coordinates<size_t> &coords) {
    return remove_building(coords.y, coords.x);
}

std::shared_ptr<Building> Map::get_building(size_t y, size_t x) {
    return all_buildings_(y, x);
}


std::shared_ptr<Building> Map::get_building(const coordinates<size_t> &coords) {
    return get_building(coords.y, coords.x);
}

bool Map::has_weapon_building(size_t y, size_t x) {
    const std::shared_ptr<Building>& building = get_building(y, x);
    return building != nullptr && building->get_item()->is_weapon();
}

bool Map::has_weapon_building(const coordinates<size_t>& coords) {
    return has_weapon_building(coords.y, coords.x);
}

bool Map::has_healing_building(size_t y, size_t x) {
    const std::shared_ptr<Building>& building = get_building(y, x);
    return building != nullptr && building->get_item()->is_healing_item();
}

bool Map::has_healing_building(const coordinates<size_t>& coords) {
    return has_healing_building(coords.y, coords.x);
}

bool Map::can_build_on(size_t y, size_t x) const {
    return all_terrains_(y, x)->can_build_on();
}


bool Map::can_build_on(const coordinates<size_t> &coords) const {
    return can_build_on(coords.y, coords.x);
}

bool Map::can_move_to_terrain(size_t y, size_t x) const {
    return all_terrains_(y, x)->can_move_to();
}
bool Map::can_move_to_terrain(const coordinates<size_t> &coords) const {
    return can_move_to_terrain(coords.y, coords.x);
}

bool Map::can_move_to_coords(size_t y, size_t x) const {
    return can_move_to_terrain(y, x) && !has_unit(y, x);
}
bool Map::can_move_to_coords(const coordinates<size_t> coords) const {
    return can_move_to_coords(coords.y, coords.x);
}

bool Map::has_unit(size_t y, size_t x) const {
    return all_units_(y, x) != nullptr;
}
bool Map::has_unit(const coordinates<size_t>& coords) const {
    return has_unit(coords.y, coords.x);
}


bool Map::add_unit(size_t y, size_t x, Unit* unit) {
    if (has_unit(y, x) || !can_move_to_terrain(y, x)) {
        return false;
    }

    all_units_(y, x) = unit;
    return true;
}

bool Map::add_unit(const coordinates<size_t>& coords, Unit* unit) {
    return add_unit(coords.y, coords.x, unit);
}

Unit* Map::get_unit(size_t y, size_t x) {
    return all_units_(y, x);
}

Unit* Map::get_unit(const coordinates<size_t>& coords) {
    return get_unit(coords.y, coords.x);
}

coordinates<size_t> Map::get_unit_location(Unit *unit_ptr) const {
    assert(unit_ptr != nullptr);

    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            if (all_units_(y, x) == unit_ptr) {
                return {x, y};
            }
        }
    }

    assert(false && "The specified unit does not exist in this map");
    return {0, 0};
}

coordinates<size_t> Map::get_building_location(std::shared_ptr<Building> building_ptr) const {
    assert(building_ptr != nullptr);

    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            if (all_buildings_(y, x) == building_ptr) {
                return {x, y};
            }
        }
    }

    assert(false && "The specified building does not exist in this map");
    return {0, 0};
}

bool Map::remove_unit(size_t y, size_t x){
    if (has_unit(y, x)) {
        all_units_(y, x) = nullptr;
        return true;
    }
    return false;
}

bool Map::remove_unit(const coordinates<size_t>& coords){
    return remove_unit(coords.y, coords.x);
}

bool Map::move_unit(size_t origin_y, size_t origin_x, size_t dest_y, size_t dest_x) {
    // Can only move a unit if a unit exists at origin and there is not unit at destination
    // and the destination coordinates can be moved to
    if (!(has_unit(origin_y, origin_x) && !has_unit(dest_y, dest_x) && can_move_to_terrain(dest_y, dest_x))) {
        return false;
    }

    //Move unit to destination and remove from origin
    Unit* origin_unit = get_unit(origin_y, origin_x);
    all_units_(dest_y, dest_x) = origin_unit;
    remove_unit(origin_y, origin_x);

    return true;
}

bool Map::move_unit(const coordinates<size_t>& origin, const coordinates<size_t>& dest) {
    return move_unit(origin.y, origin.x, dest.y, dest.x);
}


constexpr inline void Map::create_board() noexcept
{
    // with this nested loop we create all the Terrains
    for ( size_t y = 0; y < height(); y++ ) {
        for ( size_t x = 0; x < width(); x++ ) {

            // here we use normal initialisation because if we use std::make_shared the objects
            // wont be deleted until all the weak pointers go out of scope.
            // Because I use std::weak_ptr's in Terrain, I cannot use std::make_shared
            all_terrains_(y, x) = this->background_;

        }
    }

    return;
}


std::shared_ptr<Terrain> Map::get_neighbor( const coordinates<size_t>& location, const Helper::Directions direction )
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
            if ( location.x < width() - 1) {
                possible_location = this->all_terrains_( location.x + 1, location.y );
            }
            break;

        case Helper::Directions::South:
            if ( location.y < height() - 1 ) {
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


std::vector< std::shared_ptr<Terrain> > Map::get_neighbors( const coordinates<size_t>& location )
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




std::vector< coordinates<size_t> > Map::max_visible_locations( const coordinates<size_t>& location, const uint32_t visibility_range )
{
    // use the andres algorithm for creating the circle of the maximum distance the 
    // unit can see
    std::vector< coordinates<size_t> > visible_tiles;

    size_t x = location.x;
    size_t y = location.y; 
    uint32_t d = visibility_range - 1;
    uint32_t a = visibility_range - 1;
    uint32_t b = 0;

    while ( a >= b ) {
        visible_tiles.emplace_back( x + b, y + a );
        visible_tiles.emplace_back( x + a, y + b );
        visible_tiles.emplace_back( x - b, y + a );
        visible_tiles.emplace_back( x - a, y + b );
        visible_tiles.emplace_back( x + b, y - a );
        visible_tiles.emplace_back( x + a, y - b );
        visible_tiles.emplace_back( x - b, y - a );
        visible_tiles.emplace_back( x - a, y - b );

        if ( d >= 2*b ) {
            d = d - 2 * b - 1;
            b = b + 1;
        }
        else if ( d < 2 * ( visibility_range - a ) ) {
            d = d + 2 * a - 1;
            a = a - 1;
        }
        else {
            d = d + 2 * ( a - b - 1 );
            a = a - 1;
            b = b + 1;
        }
    }

    return visible_tiles;
}


std::vector< coordinates<size_t> > Map::tiles_unit_sees( const coordinates<size_t>& location, const uint32_t visibility_range )
{
    return line_of_sight_check(location, visibility_range, [this](int64_t y, int64_t x) -> bool {
        return this->get_terrain(y, x)->can_see_through();
    });
}

std::vector<coordinates<size_t>> Map::get_aoe_affected_coords(const coordinates<size_t>& location, const uint32_t range) {
    return line_of_sight_check(location, range + 1, [this](int64_t y, int64_t x) -> bool {
        return this->get_terrain(y, x)->can_shoot_through();
    });
}

std::vector< coordinates<size_t> > Map::line_of_sight_check( const coordinates<size_t>& location, const uint32_t range, const std::function<bool(int64_t y, int64_t x)>& predicate) {
    std::vector< coordinates<size_t> > max_range_coords = max_visible_locations( location, range );

    // Set to avoid duplicate coordinates and size is small enough that insert time is okay
    std::set< coordinates<size_t> > visible_coords = { {location.x, location.y} }; // contains the starting location

    // this utilises Bresentham's line drawing algorithm which is 
    // modified to work on all directions
    int64_t x0 = location.x;
    int64_t y0 = location.y;
    int64_t x1 = 0;
    int64_t y1 = 0;

    int64_t dx = 0;
    int64_t sx = 0;
    int64_t dy = 0;
    int64_t sy = 0;
    int64_t error = 0;
    int64_t e2 = 0;

    for ( const coordinates<size_t>& a_side : max_range_coords ) {
        x0 = location.x;
        y0 = location.y;

        x1 = a_side.x;
        y1 = a_side.y;
        
        dx = std::abs( x1 - x0 );
        sx = ( x0 < x1 ) ? 1 : -1;
        dy = (-1) * std::abs( y1 - y0 );
        sy = ( y0 < y1 ) ? 1 : -1;
        error = dx + dy;

        while ( true ) {

            if ( x0 == x1 && y0 == y1 ) {
                break;
            }
            
            

            e2 = 2 * error;

            if ( e2 >= dy ) {
                error += dy;
                x0 += sx;
            }
            if ( e2 <= dx ) {
                error += dx;
                y0 += sy;
            }


            // check that the tile that we're trying to look for is not out of range,
            // this can happen if the <location + visibility_range> is more than the map width or length or less than 0.
            if ( y0 < 0 || y0 >= height() || x0 < 0 || x0 >= width() ) {
                break;
            }
            // check that you can see through the tile, if not, then we stop looping since we also cant see the following tiles
            if (predicate(y0, x0) == false) {
                break;
            }
            // this is for the special case that if there's a corner in which the absolute corner tile is visible but the ones next to it are not,
            // we have to check for this case in both y and x axis.
            // if ( get_terrain( Helper::clamp( y0 - sy, 0, height() - 1 ), x0)->can_see_through() == false 
            //     && get_terrain( y0, Helper::clamp( x0 - sx, 0, width() - 1) )->can_see_through() == false) {
            //     break;
            // }
            if (predicate(Helper::clamp(y0 - sy, 0, height() - 1), x0) == false &&
                predicate(y0, Helper::clamp(x0 - sx, 0, width() - 1)) == false) {
                break;
            }
            
            visible_coords.emplace( x0, y0 );
        }
    }

    //return result as a vector using vectors constructor that takes iterators
    return {visible_coords.begin(), visible_coords.end()};

}

std::vector< coordinates<size_t> > Map::get_neighbouring_coordinates( const coordinates<size_t>& location )
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


inline bool Map::valid_direction( const coordinates<size_t>& location, const coordinates<int32_t>& direction )
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



std::vector< coordinates<size_t> > Map::possible_tiles_to_move_to( const coordinates<size_t>& location, const uint8_t movement_range )
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
    std::vector<bool> is_processed( width() * height(), false ); 

    // this will contain the distance and predecessor of each vertex as: <distance, location of predecessor>
    Matrix< a_vertex > vertex_attributes(height(), width(), { std::numeric_limits<size_t>::max(), coordinates<size_t>{0, 0} });
    vertex_attributes( location.y, location.x ) = { 0, location };


    auto Relax = [&vertex_attributes, this]( const a_vertex& curr, const coordinates<size_t>& a_neighbour, size_t weight ) -> void
    {
        if ( ( curr.first + weight < vertex_attributes( a_neighbour.y, a_neighbour.x ).first ) && (can_move_to_coords(a_neighbour)) ) {
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
        if ( !(is_processed[ curr.second.y * width() + curr.second.x ]) ) {

            // the tile is only connected to 4 other tiles in the main directions
            for ( const coordinates<int32_t>& a_direction : directions_vectors_ ) {

                // check if the direction is valid before doing the relaxation of path
                // created this <valid_direction> method to not put all the if-statements into one clutter
                if ( valid_direction( curr.second, a_direction ) ) {

                    // for different directions we have a different increment in the indexing
                    aux = curr.second + a_direction;

                    // check if we've already processed the tile
                    if ( !(is_processed[ aux.y * width() + aux.x ] )) {   
                        Relax( curr, aux, all_terrains_( aux.y, aux.x )->movement_cost() );

                        distances.emplace( vertex_attributes( aux.y, aux.x ).first, aux );
                    }
                }
            }

            is_processed[ curr.second.y * width() + curr.second.x ] = true;
        }

        
    }
    

    return tiles_that_are_close_enough;
}


std::vector< coordinates< size_t > > Map::possible_tiles_to_move_to3( const coordinates<size_t>& location, uint8_t movement_range ) {
    Timer timer;
    struct Vertex {
        size_t cooldown;
        coordinates<size_t> coords;
    };

    std::vector<bool> visited(width() * height(), false);
    //initialize result vector with the given location
    std::vector<coordinates<size_t>> result(1, location);

    // Queue of vertices that will be visited in the search.
    std::deque<Vertex> vertex_queue;
    // List of vertices that have a movement cost greater than 1 and thus won't be searched until that many movements have been used
    std::list<Vertex> waiting_vertices;

    // Add starting location to queue and mark it as visited
    vertex_queue.emplace_back(0, location);
    visited[location.y * width() + location.x] = true;
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
                if (visited[neighbour.y * width() + neighbour.x]|| !can_move_to_coords(neighbour)) continue;
                visited[neighbour.y * width() + neighbour.x] = true;

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

coordinates<size_t> Map::get_closest_accessible_tile(const coordinates<size_t>& location) {
    std::deque<coordinates<size_t>> q;
    q.push_back(location);
    std::vector<bool> visited(width() * height(), false);

    while (!q.empty()) {
        coordinates<size_t> current = q.front();
        q.pop_front();

        if (can_move_to_coords(current))
            return current;

        for (const coordinates<size_t>& neighbour : get_neighbouring_coordinates(current)) {
            q.push_back(neighbour);
        }

    }

    assert(false && "There is not a single tile on the map that you can move to");

    return {0, 0};
}

coordinates<size_t> Map::fastest_movement_to_target(const coordinates<size_t>& location, coordinates<size_t> target, uint8_t movement_range) {
    assert(can_move_to_terrain(target) && "Target coordinates cannot be moved to");
    Timer timer;

    // If there is somebody on the target tile, get closest other tile
    if (has_unit(target)) {
        target = get_closest_accessible_tile(target);
    }

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
    std::vector<bool> is_processed( width() * height(), false ); 

    // this will contain the distance and predecessor of each vertex as: <distance, location of predecessor>
    Matrix< a_vertex > vertex_attributes(height(), width(), { std::numeric_limits<size_t>::max(), coordinates<size_t>{0, 0} });
    vertex_attributes( location.y, location.x ) = { 0, location };


    auto Relax = [&vertex_attributes, this]( const a_vertex& curr, const coordinates<size_t>& a_neighbour, size_t weight ) -> void
    {
        if ( ( curr.first + weight < vertex_attributes( a_neighbour.y, a_neighbour.x ).first ) && (can_move_to_coords(a_neighbour)) ) {
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


        // add the tile's coordinates into the return container only if their distance 
        // is equal or less than the given <movement_range>
        tiles_that_are_close_enough.emplace_back( curr.second.x, curr.second.y );
        

        // check if we've already computed the current vertex
        if ( !(is_processed[ curr.second.y * width() + curr.second.x ]) ) {

            // the tile is only connected to 4 other tiles in the main directions
            for ( const coordinates<int32_t>& a_direction : directions_vectors_ ) {

                // check if the direction is valid before doing the relaxation of path
                // created this <valid_direction> method to not put all the if-statements into one clutter
                if ( valid_direction( curr.second, a_direction ) ) {

                    // for different directions we have a different increment in the indexing
                    aux = curr.second + a_direction;

                    // check if we've already processed the tile
                    if ( !(is_processed[ aux.y * width() + aux.x ] )) {   
                        Relax( curr, aux, all_terrains_( aux.y, aux.x )->movement_cost() );

                        distances.emplace( vertex_attributes( aux.y, aux.x ).first, aux );
                        if (target == aux) {
                            std::vector<coordinates<size_t>> path;
                            coordinates<size_t> current = aux;

                            while (current != location) {
                                path.push_back(current);
                                current = vertex_attributes(current).second;
                            }

                            size_t i = path.size();
                            int range_left = movement_range;
                            while (i > 0) {
                                i--;
                                range_left -= all_terrains_(path[i])->movement_cost();

                                // If no move range to move or this is the last coordinate in the path, return
                                if (range_left <= 0 || i == 0) {
                                    return path[i];
                                }

                            }
                        }
                    }
                }
            }

            is_processed[ curr.second.y * width() + curr.second.x ] = true;
        }

        
    }

    assert(false && "this probably shouldnt be reached ever");
    return {0, 0};

}


void Map::print_map() const {
    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            std::cout << all_terrains_(y, x)->get_repr();
        }
        std::cout << '\n';
    }

    std::cout << std::endl;
}

void Map::print_units() const {
    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            if (has_unit(y, x)) {
                std::cout << '@';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }

    std::cout << std::endl;

}

void Map::print_buildings() const {
    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
             if (has_building(y, x)) {
                 std::cout << '@';
             } else {
                 std::cout << '.';
             }
         }
        std::cout << '\n';
    }
    std::cout << std::endl;

}
