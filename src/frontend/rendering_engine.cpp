#include "rendering_engine.hpp"
#include "renderer.hpp"



Rendering_Engine::Rendering_Engine(std::shared_ptr<Game>& game) : game_(game) {}

void Rendering_Engine::render(size_t window_width, size_t window_height, sf::RenderWindow& window, Renderer& renderer, const std::shared_ptr<Window_To_Render>& renderables)
{
    r_map_ = renderer.get_r_map();
    tile_map_ = renderer.get_tile_map();
    r_aux_ = renderer.get_r_aux();
    manager_ = std::make_shared<Game_Manager>(game_, tile_map_);

    gui_ = GUI(manager_);
    gui_.initialize();

    // run the program as long as the window is open
    while (window.isOpen())
    {
        //Performing all events here.
        sf::Event event;
        while (window.pollEvent(event))
        {
            events(window, event, renderer);
        }

        window.clear(sf::Color::Black);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        handle_continuous_inputs(1, 1, renderer, mousePos);

        if (manager_->selected_valid_unit()) {
            r_aux_->draw_unit_highlight(manager_->selected_unit_coords());
            r_aux_->draw_cursor_highlight(mousePos.x,mousePos.y);
            coordinates<size_t> target_coord = tile_map_->get_map_coords(mousePos.x,mousePos.y);
            r_aux_->show_text = true;
            r_aux_->draw_text(mousePos.x,mousePos.y,manager_->get_action_info(target_coord,nullptr));
        } else {
            r_aux_->hide_unit_highlight();
            r_aux_->hide_cursor_highlight();

            r_aux_->show_text = false;
            r_aux_->clear_text();
        }

        std::string output = game_->get_output();
        if (output.size() > 0) {
            std::cout << output << std::endl;
            game_->clear_output();
        }

        //Every render target needs to be updated after changes.
        renderables->update();  // update every renderable
        gui_.update();

        //Every render target will be drawn separately.
        window.draw(*renderables);  // draw the renderables
        window.draw(gui_);

        window.display();
    }
}

Game& Rendering_Engine::get_game() const {
    return *game_;
}

std::shared_ptr<Game>& Rendering_Engine::get_game() 
{
    return game_;
}


/**
 * sf::Transformable apparently has a move method. Maybe we should use that.  
 */
void Rendering_Engine::handle_continuous_inputs(float moveSpeed, float zoom, Renderer& renderer, const sf::Vector2i& mouse_pos) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        tile_map_->move(-moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        tile_map_->move(0,-moveSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        tile_map_->move(moveSpeed,0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        tile_map_->move(0,moveSpeed);
    }

    if (mouse_pos.x >= renderer.width() * (1 - screen_area_to_move_screen_)) {
        tile_map_->move(-moveSpeed, 0);
    }
    if (mouse_pos.x <= renderer.width() * screen_area_to_move_screen_) {
        tile_map_->move(moveSpeed, 0);
    }

    if (mouse_pos.y >= renderer.width() * (1 - screen_area_to_move_screen_)) {
        tile_map_->move(0, -moveSpeed);
    }
    if (mouse_pos.y <= renderer.height() * screen_area_to_move_screen_) {
        tile_map_->move(0, moveSpeed);
    }
}

void Rendering_Engine::events(sf::RenderWindow& target, sf::Event event, Renderer& renderer) {
    // If a button was pressed (aka func returns true) return, which means event is consumed by the button
    if (gui_.execute_button_actions(target, event)) return;


    // "close requested" event: we close the window
    if (event.type == sf::Event::Closed)
        target.close();
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(target);
        std::cout << "Current pixel pos: [" << mousePos.x << "," << mousePos.y << "]" << std::endl;
        if (tile_map_->is_inside_map_pixel(mousePos.x,mousePos.y))
        {


            coordinates<size_t> matrix_pos = tile_map_->get_map_coords(mousePos.x,mousePos.y);
            std::pair<int,int> tile_pixel_pos = tile_map_->get_tile_coords(matrix_pos);
            std::cout << "Current tile pixel pos: [" << tile_pixel_pos.first << "," << tile_pixel_pos.second << "]" << std::endl;
            std::cout << "Current tile coords:" << matrix_pos << std::endl;
            std::cout << "Terrain: " << tile_map_->GetMap().get_terrain(matrix_pos)->get_repr() << std::endl;

            gui_.click_on_coords(matrix_pos.y, matrix_pos.x);

        }
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
        manager_->deselect_unit();
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        manager_->next_turn();
    }
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F) {
        tile_map_->fog_of_war = !tile_map_->fog_of_war;
    }

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::X) {
        manager_->undo_action();
    }

    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::R) {
        renderer.initialise_level(1);

        // have to update these pointers right after initialising a new level so we
        // don't try to call the old objects and get segfault
        r_map_ = renderer.get_r_map();
        tile_map_ = renderer.get_tile_map();
        r_aux_ = renderer.get_r_aux();

        manager_ = std::make_shared<Game_Manager>(game_, tile_map_);
        gui_ = GUI(manager_);
        gui_.initialize();
    }
}
