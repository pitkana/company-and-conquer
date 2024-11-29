#ifndef MAP_BUILDER
#define MAP_BUILDER

#include "map.hpp"
#include "vector"
#include "fstream"
#include "exception"
#include "algorithm"


/**
 * Some exceptions that used to kill the program if an invalid map file is used.
*/
class Invalid_Map_Exception : public std::exception {
    virtual const char* what() const noexcept {
        return "Lines in map file are no the same length.";
    }
};

class Empty_Map_Exception : public std::exception {
    virtual const char* what() const noexcept {
        return "Read map is empty.";
    }
};

/**
 * Map_Builder class is used to read a file and use its data to construct a valid Map object.
 */
class Map_Builder {
    public:
        //Helper method for reading files.
        //TODO: Add exceptions.
        //This method used be private.
        std::vector<std::vector<char>> read_map_file(const std::string& path) {
            std::vector<std::vector<char>> out;
            std::ifstream is(path);
            std::string s;
            while (getline(is,s)) {
               std::vector<char> line; 
                for (auto it : s) {
                    line.push_back(it);
                }
                out.push_back(line);
            }
            is.close();
            if (out.size() == 0) {
                throw Empty_Map_Exception();
            }
            return out;
        }

        /**
         * @brief Reads a files and constructs a valid Map object.
         * 
         * @param map_path A path to file that contains the format for desired map.
         * 
         * @returns A pointer to the newly created Map.
         */
        Map load(const std::string& map_path) {
            std::vector<std::vector<char>> terrain_vec = read_map_file(map_path);
            int height = terrain_vec.size();
            if (height == 0) {
                throw Invalid_Map_Exception();
            }

            int width = terrain_vec[0].size();
            bool map_is_rectangle = std::all_of(terrain_vec.begin(),terrain_vec.end(),[width](std::vector<char> line) {
                int s = line.size();
                return s == width;
            });
            if (!map_is_rectangle) {
                throw Invalid_Map_Exception();
            }

            Map out = Map(width,height);

            /** 
             * Inefficient way of updating terrains because the Map is already initialized with
             * default terrain in the constructor. However, the runtime change after optimization
             * will be trivial since our map sizes will be relatively small.
            */
            for (int w = 0; w < height; w++) {
                for (int h = 0; h < width; h++) {
                    out.update_terrain(terrain_vec[w][h],w,h);
                }
            }
            return out;
        }

        //Overloaded load method will be used in Game class constructor.
        void load(std::vector<std::vector<char>> terrain_vec, Map& map) {
            size_t height = terrain_vec.size();
            if (height == 0) {
                throw Invalid_Map_Exception();
            }
            size_t width = terrain_vec[0].size();
            bool map_is_rectangle = std::all_of(terrain_vec.begin(),terrain_vec.end(),[width](std::vector<char> line) {
                size_t s = line.size();
                return s == width;
            });
            if (!map_is_rectangle || map.height() != height || map.width() != width) {
                throw Invalid_Map_Exception();
            }
            /** 
             * Inefficient way of updating terrains because the Map is already initialized with
             * default terrain in the constructor. However, the runtime change after optimization
             * will be trivial since our map sizes will be relatively small.
            */
            for (size_t w = 0; w < height; w++) {
                for (size_t h = 0; h < width; h++) {
                    map.update_terrain(terrain_vec[w][h], w, h);
                }
            }
        }
};

#endif
