#include "rendering_engine.hpp"
#include "renderer.hpp"



Rendering_Engine::Rendering_Engine(std::shared_ptr<Game>& game, const std::string& texture_path) : 
    game_(game), text_path_(texture_path), gui_(game) {
    gui_.initialize();
}



void Rendering_Engine::render(size_t window_width, size_t window_height, sf::RenderWindow& window, Render_Map& r_map, Tile_Map& tile_map, Renderer& renderer)
{
    
    if (!r_map.load(text_path_)) {
        return;
    }

    //sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Game");

    sf::Font myfont;


    // run the program as long as the window is open
    while (window.isOpen())
    {
        //Performing all events here.
        sf::Event event;
        while (window.pollEvent(event))
        {
            events(r_map, window, event);
        }

        window.clear(sf::Color::Black);
        key_inputs(r_map, renderer);

        //Every render target needs to be updated after changes.
        //r_map.update();
        //Every render target will be drawn separately.
        window.draw(r_map); //Draw map.
        window.draw(gui_);

        window.display();
    }
}

void Rendering_Engine::key_inputs(Render_Map& r_map, Renderer& renderer) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        r_map.move(-move_speed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        r_map.move(0,-move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        r_map.move(move_speed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        r_map.move(0,move_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        r_map.zoom(1, renderer.width(), renderer.height());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        r_map.zoom(-1, renderer.width(), renderer.height());
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        /*
        Map_Builder builder_ = Map_Builder{};
        
        std::vector<std::vector<char>> terrain_vec = builder_.read_map_file(TESTMAP_PATH1);

        builder_.load(terrain_vec, tile_map.GetMap());

        if (!r_map.load(text_path_)) {
            return;
        }
        */

        renderer.initialise_level(1);
        // the below should be implemented in Renderer
        if (!r_map.load(text_path_)) {
            return;
        }
        
    }
}

void Rendering_Engine::events(const Render_Map& render_map, sf::RenderWindow& target, sf::Event event) {
    // "close requested" event: we close the window
    switch (event.type) {
        case (sf::Event::Closed): {
            target.close();
            break;
        }
        case (sf::Event::MouseButtonReleased): {
            sf::Vector2i mousePos = sf::Mouse::getPosition(target);
            std::pair<int,int> matrix_pos = render_map.get_map_coords(mousePos.x,mousePos.y);

            std::cout << "Current pixel pos: [" << mousePos.x << "," << mousePos.y << "]" << std::endl;
            std::cout << "Current tile pos: [" << matrix_pos.first << "," << matrix_pos.second << "]" << std::endl; 
            break;
        }

        default:
            break;
    }
}
