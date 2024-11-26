#include "render_window.hpp"

Render_Window::Render_Window(Game* game, const std::string& texture_path) {
    game_ = game;
    text_path_ = texture_path;
}

void Render_Window::spawn_window(int window_width, int window_height) {

    if (!r_map_.load(text_path_,100,game_->get_map())) {
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
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        key_inputs(3,1);
        // draw everything here...
        // window.draw(...);
        // end the current frame
        window.draw(r_map_); //Draw map.
        window.display();
    }
}

void Render_Window::key_inputs(float moveSpeed, float zoom) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        r_map_.moveTiles((-1)*moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        r_map_.moveTiles(0,(-1)*moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        r_map_.moveTiles(moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        r_map_.moveTiles(0,moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        r_map_.zoom(zoom);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        r_map_.zoom(-1*zoom);
    }
}