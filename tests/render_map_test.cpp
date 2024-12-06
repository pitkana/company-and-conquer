#include "iostream"
#include "map_builder.hpp"
#include "SFML/Graphics.hpp"
#include "render_map.hpp"


//Also contains tests for Tile_Map class.
//Use WASD to move map and FG to zoom in and out.
//Clicking tile will print the coordinates of that tile.
void render_map_test() {
    Map_Builder builder = Map_Builder();
    std::vector<std::vector<char>> terrain_vec = builder.read_map_file(TESTMAP_PATH);
    size_t height = terrain_vec.size();
    size_t width = terrain_vec[0].size();
    std::shared_ptr<Game> game = std::make_shared<Game>(Game(height,width));
    builder.load(terrain_vec,game->get_map());
    int window_width = 500;
    int window_height = 600;
    std::shared_ptr<Tile_Map> tile_map = std::make_shared<Tile_Map>(Tile_Map(game,100));
    Render_Map render_game(tile_map);

    if (!render_game.load(TEXTURE_PATH)) {
        return;
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(window_height, window_width), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                //std::pair<int,int> matrix_pos = tile_map->get_map_coords(mousePos.x,mousePos.y);
                //std::cout << "Current pixel pos: [" << mousePos.x << "," << mousePos.y << "]" << std::endl;
                //std::cout << "Current tile pos: [" << matrix_pos.first << "," << matrix_pos.second << "]" << std::endl; 
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        
        float moveSpeed = 3;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            tile_map->move((-1)*moveSpeed,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            tile_map->move(0,(-1)*moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            tile_map->move(moveSpeed,0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            tile_map->move(0,moveSpeed);
        }
        //Update all components
        render_game.update();
        // draw everything here...
        // window.draw(...);
        // end the current frame
        window.draw(render_game);
        window.display();
    }
}