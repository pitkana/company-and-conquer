#include "rendering_engine.hpp"
#include "renderer.hpp"



Rendering_Engine::Rendering_Engine(std::shared_ptr<Game>& game, const std::string& map_texture_path, const std::string& unit_texture_path, const std::string& buildings_texture_path, const std::string& aux_texture_path) : game_(game), map_text_path_(map_texture_path), unit_text_path_(unit_texture_path), building_text_path_(buildings_texture_path), aux_text_path_(aux_texture_path) { }

void Rendering_Engine::render(size_t window_width, size_t window_height, sf::RenderWindow& window, Render_Map& r_map, Tile_Map& tile_map, Render_Units& r_units, Render_Buildings& r_buildings, Render_Aux& r_aux_, Renderer& renderer)
{
    
    if (!r_map.load(map_text_path_)) {
        return;
    }
    if (!r_units.load(unit_text_path_)) {
        return;
    }

    if (!r_buildings.load(building_text_path_)) {
        return;
    }

    if (!r_aux_.load(aux_text_path_)) {
        return;
    }

    Game_Manager manager(tile_map.GetGame());

    game_->init_game();

    // run the program as long as the window is open
    while (window.isOpen())
    {
        //Performing all events here.
        sf::Event event;
        while (window.pollEvent(event))
        {
            events(tile_map, window, event, manager);
        }
        window.clear(sf::Color::Black);
        key_inputs(tile_map, 1, 1, r_map, renderer);

        if (manager.action_ontheway()) {
            r_aux_.draw_unit_highlight(manager.get_priority_coords());
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (tile_map.is_inside_map_pixel(mousePos.x,mousePos.y)) {
                coordinates<size_t> matrix_pos = tile_map.get_map_coords(mousePos.x,mousePos.y);
                r_aux_.draw_cursor_highlight(matrix_pos);
            }
        } else {
            r_aux_.hide_unit_highlight();
            r_aux_.hide_cursor_highlight();
        }

        std::string output = game_->get_output();
        if (output.size() > 0) {
            std::cout << output << std::endl;
            game_->clear_output();
        }

        //Every render target needs to be updated after changes.
        r_map.update();
        r_units.update();
        r_buildings.update();
        r_aux_.update();
        //Every render target will be drawn separately.
        window.draw(r_map); //Draw map.
        window.draw(r_units);
        window.draw(r_buildings);
        window.draw(r_aux_);
        window.display();
        //std::cout << game_->get_output() << std::endl;
        //game_->clear_output();
    }
}

Game& Rendering_Engine::get_game() const {
    return *game_;
}


/**
 * sf::Transformable apparently has a move method. Maybe we should use that.  
 */
void Rendering_Engine::key_inputs(Tile_Map& tile_map, float moveSpeed, float zoom, Render_Map& r_map, Renderer& renderer) {
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
/*
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        r_map.zoom(1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        r_map.zoom(-1);
    }
*/
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
        if (!r_map.load(map_text_path_)) {
            return;
        }
        
    }
}

void Rendering_Engine::events(Tile_Map& tile_map, sf::RenderWindow& target, sf::Event event, Game_Manager& manager) {
    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
        target.close();
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(target);
        std::cout << "Current pixel pos: [" << mousePos.x << "," << mousePos.y << "]" << std::endl;
        if (tile_map.is_inside_map_pixel(mousePos.x,mousePos.y))
        {
            coordinates<size_t> matrix_pos = tile_map.get_map_coords(mousePos.x,mousePos.y);
            std::pair<int,int> tile_pixel_pos = tile_map.get_tile_coords(matrix_pos);
            std::cout << "Current tile pixel pos: [" << tile_pixel_pos.first << "," << tile_pixel_pos.second << "]" << std::endl;
            std::cout << "Current tile coords:" << matrix_pos << std::endl;
            std::cout << "Terrain: " << tile_map.GetMap().get_terrain(matrix_pos)->get_repr() << std::endl;
            if (manager.action_ontheway()) {
                manager.enqueue_movement_action(matrix_pos);
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(target);
        if (tile_map.is_inside_map_pixel(mousePos.x,mousePos.y))
        {
            coordinates<size_t> mouse_coords = tile_map.get_map_coords(mousePos.x,mousePos.y);
            Map& map = tile_map.GetMap();
            if (map.has_unit(mouse_coords)) {
                Unit* target_unit = map.get_unit(mouse_coords);
                if (manager.init_priority(mouse_coords)) {
                    std::cout << "Init priority!" << std::endl;
                } else {
                    std::cout << "Failed!" << std::endl;
                }
            }
            //tile_map.center_at(matrix_pos,700,700);
        }
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
        manager.terminate_action();
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        game_->next_turn();
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F) {
        tile_map.fog_of_war = !tile_map.fog_of_war;
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Z) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(target);
        if (tile_map.is_inside_map_pixel(mousePos.x,mousePos.y) && manager.action_ontheway()) {
            coordinates<size_t> mouse_coords = tile_map.get_map_coords(mousePos.x,mousePos.y);
            if (manager.enqueue_item_action(mouse_coords)) {
                std::cout << "Item action success!" << std::endl;
            } else {
                std::cout << "Item action failed!" << std::endl;
            }
        }
    }
}