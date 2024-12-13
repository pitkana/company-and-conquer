#include "rendering_engine.hpp"
#include "renderer.hpp"
#include "game_logs.hpp"


Rendering_Engine::Rendering_Engine(std::shared_ptr<Game>& game, size_t width, size_t height) : game_(game) { }

void Rendering_Engine::render(size_t window_width, size_t window_height, sf::RenderWindow& window, Renderer& renderer, const std::shared_ptr<Window_To_Render>& renderables)
{
    tile_map_ = renderer.get_tile_map();
    r_aux_ = renderer.get_r_aux();
    manager_ = std::make_shared<Game_Manager>(game_, tile_map_);

    gui_ = GUI(manager_, window_width, window_height);
    gui_.initialize();

    std::shared_ptr<Game_Logs>& logs = renderer.get_logs();

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
            r_aux_->show_unit_highlight(manager_->selected_unit_coords());
            r_aux_->show_cursor_highlight(mousePos.x, mousePos.y);
            r_aux_->update_movement_range(manager_->selected_unit_possible_movements());
            coordinates<size_t> target_coord = tile_map_->get_map_coords(mousePos.x,mousePos.y);
            r_aux_->show_text = true;
            r_aux_->show_cursor_text(mousePos.x,mousePos.y,manager_->get_action_info(target_coord,gui_.get_active_item()));
            //Only draw the movement range if selected unit has not yet moved
            if (!manager_->selected_unit_ptr()->has_moved)
                r_aux_->update_movement_range(manager_->selected_unit_possible_movements());
            r_aux_->show_cursor_text(mousePos.x,mousePos.y,manager_->get_action_info(target_coord,gui_.get_active_item()));

            //Only draw the movement range if selected unit has not yet moved
            if (!manager_->selected_unit_ptr()->has_moved)
                r_aux_->update_movement_range(manager_->selected_unit_possible_movements());

        } else {
            r_aux_->clear_movement_range_rects();
            r_aux_->clear_movement_range_rects();
            r_aux_->hide_unit_highlight();
            r_aux_->hide_cursor_highlight();
            r_aux_->show_text = false;
            r_aux_->clear_cursor_text();
        }



        std::string output = game_->get_output();
        if (output.size() > 0) {
            std::cout << output << std::endl;
            logs->add_logs(output);
            game_->clear_output();
        }
        if (logs->show_logs) {
            r_aux_->show_logs(logs->get_logs());
        } else {
            r_aux_->clear_logs();
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
}

void Rendering_Engine::events(sf::RenderWindow& target, sf::Event event, Renderer& renderer) {
    // If a button was pressed (aka func returns true) return, which means event is consumed by the button
    if (gui_.execute_button_actions(target, event)) return;

    switch(event.type) {
        // "close requested" event: we close the window
        case(sf::Event::Closed): {
            target.close();
            break;
        }

        case (sf::Event::MouseButtonReleased): {
            if (event.mouseButton.button != sf::Mouse::Left)
                break;

            sf::Vector2i mousePos = sf::Mouse::getPosition(target);
            // Check if the mouse is inside of the map
            if (tile_map_->is_inside_map_pixel(mousePos.x,mousePos.y))
            {
                coordinates<size_t> matrix_pos = tile_map_->get_map_coords(mousePos.x,mousePos.y);
                gui_.click_on_coords(matrix_pos.y, matrix_pos.x);
            }

            break;
        }

        case (sf::Event::KeyReleased): {
            switch (event.key.code) {
                case (sf::Keyboard::Escape): {
                    gui_.deselect_unit();
                    break;
                }

                case (sf::Keyboard::Space): {
                    gui_.next_turn();
                    break;
                }

                case (sf::Keyboard::F): {
                    tile_map_->fog_of_war = !tile_map_->fog_of_war;
                    break;
                }

                case (sf::Keyboard::X): {
                    gui_.undo_action();
                    break;
                }


                case (sf::Keyboard::B): {
                    renderer.get_logs()->show_logs = !renderer.get_logs()->show_logs;
                    break;
                }

                case (sf::Keyboard::Up): {
                    renderer.get_logs()->change_start(-1);
                    break;
                }

                case (sf::Keyboard::Down): {
                    renderer.get_logs()->change_start(1);
                    break;
                }

                default:
                    break;
            }

            break;
        }
    }
}
