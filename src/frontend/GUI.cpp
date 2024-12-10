#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

#include "GUI.hpp"
#include "game.hpp"


GUI::GUI(std::shared_ptr<Game> game):
    game_(game) {}

void GUI::initialize() {
    if (!font_.loadFromFile("fonts/NotoSans-Bold.ttf")) {
        assert(false && "Loading font failed");
    }

    {
        RectButton button(font_, true, sf::Vector2f(100, 100));
        button.setButtonLabel(24, "Moi!!!!!!!!!!!!!!!");
        button.set_activation_function([](const std::shared_ptr<Game>& game, size_t y, size_t x) {
            std::cout << "hello" << std::endl;
        });
        always_active_buttons_.buttons.push_back(std::move(button));
    }

    always_active_buttons_.isActive = true;
}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const RectButton& button : always_active_buttons_.buttons) {
        button.draw(target);
    }

    if (unit_buttons_.isActive) {
        for (const RectButton& button : unit_buttons_.buttons) {
            button.draw(target);
        }
    }
}


void GUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event, size_t map_y, size_t map_x) {
    for (RectButton& button : always_active_buttons_.buttons) {
        button.getButtonStatus(window, event);
        if (button.isPressed) {
            button.activate(game_, map_y, map_x);
        }
    }
}
