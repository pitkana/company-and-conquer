#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include "main_screen.hpp"
#include "renderer.hpp"
#include "sfmlbutton.hpp"

MainScreen::MainScreen(Renderer& renderer, size_t window_width, size_t window_height):
    renderer_(renderer), window_width_(window_width), window_height_(window_height) 
{
    if (!font_->loadFromFile(FONT_PATH)) {
        std::cout << "File could not be loaded" << std::endl;
        return;
    }

    title_.setFont(*font_);
    title_.setFillColor(sf::Color::White);
    title_.setCharacterSize(50);
    title_.setString("Company & Conquer");

    sf::Vector2f title_size = title_.getLocalBounds().getSize();
    title_.setOrigin(-((float)window_width / 2 - title_size.x / 2), -((float)window_height / 6));

}


void MainScreen::start(sf::RenderWindow& window) {

    RectButton load_file_button(*font_, true, {(float)window_width_ / 2 - 130, 4 * (float) window_height_ / 6});
    load_file_button.setButtonLabel(30, "\n Load Scenario \n");
    load_file_button.set_activation_function([this]() {
        this->renderer_.initialize_scenario();
    });

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            load_file_button.getButtonStatus(window, event);
            if (load_file_button.isPressed) {
                load_file_button.activate();
            }

            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        window.clear();

        load_file_button.draw(window);
        window.draw(title_);

        window.display();
    }
}
