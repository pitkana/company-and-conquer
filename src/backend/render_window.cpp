#include "render_window.hpp"

Render_Window::Render_Window(Game* game, const std::string& texture_path) {
    game_ = game;
    text_path_ = texture_path;
}

void Render_Window::spawn_window(int window_width, int window_height) {

    Tile_Map tile_map(*game_,100);
    Render_Map r_map(tile_map);
    if (!r_map.load(text_path_)) {
        return;
    }
    // create the window
    sf::RenderWindow window(sf::VideoMode(window_height, window_width), "Game");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        //Performing all events here.
        sf::Event event;
        while (window.pollEvent(event))
        {
            events(tile_map,window,event);
        }

        window.clear(sf::Color::Black);
        key_inputs(tile_map,3,1);

        //Every render target needs to be updated after changes.
        r_map.update();
        //Every render target will be drawn separately.
        window.draw(r_map); //Draw map.
        window.display();
    }
}

void Render_Window::key_inputs(Tile_Map& tile_map, float moveSpeed, float zoom) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        tile_map.move((-1)*moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        tile_map.move(0,(-1)*moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        tile_map.move(moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        tile_map.move(0,moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        tile_map.zoom(1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        tile_map.zoom(-1);
    }
}

void Render_Window::events(Tile_Map& tile_map, sf::RenderWindow& target, sf::Event event) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
        target.close();
    if (event.type == sf::Event::MouseButtonReleased) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(target);
        std::pair<int,int> matrix_pos = tile_map.get_map_coords(mousePos.x,mousePos.y);
        std::cout << "Current pixel pos: [" << mousePos.x << "," << mousePos.y << "]" << std::endl;
        std::cout << "Current tile pos: [" << matrix_pos.first << "," << matrix_pos.second << "]" << std::endl; 
    }
}