#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

#include "GUI.hpp"
#include "game.hpp"
#include "unit.hpp"


GUI::GUI(std::shared_ptr<Game> game):
    game_(game), map_(&game->get_map()) {}

void GUI::initialize() {
    if (!font_.loadFromFile("fonts/NotoSans-Bold.ttf")) {
        assert(false && "Loading font failed");
    }

    initialize_main_buttons();

}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    draw_button_group(target, main_buttons_);

    if (unit_buttons_.isActive) {
        draw_button_group(target, unit_buttons_);
    }

    if (inventory_buttons_.isActive) {
        draw_button_group(target, inventory_buttons_);
    }
}

void GUI::update() {
    if (!map_->are_valid_coords(active_coords))
        return;

    if (map_->has_unit(active_coords)){
        // initialize_movement();
        initialize_inventory();
    } else {
        inventory_buttons_.isActive = false;
        unit_buttons_.isActive = false;
    }
}

void GUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event, size_t map_y, size_t map_x) {
    for (RectButton& button : main_buttons_.buttons) {
        button.getButtonStatus(window, event);
        if (button.isPressed) {
            button.activate(game_, map_y, map_x);
        }
    }
}

void GUI::set_active_coords(size_t y, size_t x) {
    active_coords = {x, y};
}

void GUI::initialize_main_buttons() {
    RectButton button(font_, true, sf::Vector2f(100, 100));
    button.setButtonLabel(24, "Moi!!!!!!!!!!!!!!!");
    button.set_activation_function([](const std::shared_ptr<Game>& game, size_t y, size_t x) {
        std::cout << "hello" << std::endl;
    });
    main_buttons_.buttons.push_back(std::move(button));

    main_buttons_.isActive = true;
}

void GUI::initialize_inventory() {
    inventory_buttons_.buttons.clear();

    float curr_x = 30;
    Unit* active_unit = map_->get_unit(active_coords);
    const std::vector<std::shared_ptr<const Item>>& inventory = active_unit->get_inventory();

    for (unsigned int i = 0; i < unit_consts.inventory_size; i++) {
        RectButton button(font_, true, {curr_x, 500});
        if (i < inventory.size()) {
            button.setButtonLabel(20, inventory[i]->get_name());
        } else {
            button.setButtonLabel(20, "No item");
        }

        curr_x += button.button.getSize().x + 20;
        inventory_buttons_.buttons.push_back(std::move(button));
    }

    inventory_buttons_.isActive = true;
}

void GUI::draw_button_group(sf::RenderTarget& target, const RectButtonGroup& group) const {
    for (const RectButton& button : group.buttons) {
        button.draw(target);
    }
}
