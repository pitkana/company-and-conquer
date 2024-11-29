#include <memory>

#include "aoe_test.hpp"
#include "map.hpp"
#include "coordinates.hpp"

static std::unordered_map<char, Terrain> terrains;
static size_t width = 20;
static size_t height = 10;


void change_terrain2(Map& a_map) {
  std::cout << "Enter coordinates in form 'x y'" << std::endl;
  int x, y;
  std::cin >> x >> y;
  if (std::cin.fail()) {return;}

  char terrain_type;
  std::cout << "Enter terrain character representative (. or # or - or O)" << std::endl;
  std::cin >> terrain_type;
  if (std::cin.fail()) {return;}

  switch (terrain_type) {
    case '.':
      a_map.update_terrain(terrain_type, y, x);
      return;
    case '#':
      a_map.update_terrain(terrain_type, y, x);
      return;
    case '-':
      a_map.update_terrain(terrain_type, y, x);
      return;
    case 'O':
      a_map.update_terrain(terrain_type, y, x);
      return;
    default:
      return;
  }
}

void print_aoe(Map& map) {
  while (true) {
    std::cout << "Enter coordinates in form 'x y'" << std::endl;
    size_t x, y;
    std::cin >> x >> y;
    if (std::cin.fail()) {std::cin.clear();return;}

    std::vector<coordinates<size_t>> affected_coords = map.get_aoe_affected_coords({x, y}, 5);
    std::cout << affected_coords.size() << std::endl;

    for (size_t y = 0; y < height; ++y) {
      for (size_t x = 0; x < width; ++x) {
        if (std::find_if(affected_coords.begin(), affected_coords.end(), [x, y](const coordinates<size_t>& coords) {
          return (coords.x == x && coords.y == y);
        }) != affected_coords.end()) {
          std::cout << '@';
        } else {
          std::cout << map.get_terrain(y, x)->get_repr();
        }
      }
      std::cout << '\n';
    }
    std::cout << std::endl;
    return;
  }
}


int aoe_test() {

  Map map(width, height);

  Terrain background('.');
  Terrain wall = Terrain('#', false, false, false, false);
  Terrain swamp('-', 3);
  terrains[background.get_repr()] = background;
  terrains[wall.get_repr()] = wall;
  terrains[swamp.get_repr()] = swamp;

  while (true) {
    std::cout << "A to change terrain, M to calculate movement, Q to quit" << std::endl;
    char input;
    std::cin >> input;
    if (std::cin.fail()) {std::cin.clear(); continue;}

    switch (input) {
      case 'A':
        change_terrain2(map);
        map.print_map();
        break;
      case 'M':
        print_aoe(map);
        break;
      case 'Q':
        return EXIT_SUCCESS;
      default:
        std::cin.clear();
    }
  }
}

