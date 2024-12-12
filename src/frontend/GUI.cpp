#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

#include "coordinates.hpp"
#include "GUI.hpp"
#include "game.hpp"
#include "unit.hpp"


GUI::GUI() {}

GUI::GUI(std::shared_ptr<Game_Manager> game_manager):
    game_manager_(std::move(game_manager)), map_(&game_manager_->get_map()) {}

void GUI::initialize() {
    if (!font_->loadFromFile(GUI_FONT_PATH)) {
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
    update_inventory();
    // Set it to false after update so dont keep updating
    selected_unit_changed_ = false;
}

bool GUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event) {

    for (RectButton* button : get_all_buttons()) {
        button->getButtonStatus(window, event);
        if (button->isPressed) {

            if (button->isActive) {
                button->activate();
            }
            return true;
        } 
    }

    return false;
}

void GUI::click_on_coords(size_t y, size_t x) {
    coordinates<size_t> clicked_coords(x, y);

    // if (use_active_item_on_coords(clicked_coords)) return;
    if (active_item != nullptr) {
        //If action succeeded, deselect the active item
        if (game_manager_->enqueue_item_action(clicked_coords, active_item.get()))
            active_item = nullptr;
        return;
    }

    // If clicked on own team's unit, select that before trying to move on their square
    if (game_manager_->select_unit_on_coords(clicked_coords)) {
        active_item = nullptr;
        selected_unit_changed_ = true;
        return;
    }

    // At last try to move the selected unit
    game_manager_->enqueue_movement_action(clicked_coords);
}

void GUI::initialize_main_buttons() {

    float curr_x = 50;
    RectButton next_unit_button(*font_, true, {curr_x, 650});
    next_unit_button.setButtonLabel(20, " Next unit ");
    next_unit_button.set_activation_function([this]() {
        this->game_manager_->cycle_units(700, 700);
        this->selected_unit_changed_ = true;
    });

    curr_x += next_unit_button.button.getSize().x + 20;

    RectButton end_turn_button(*font_, true, {curr_x, 650});
    end_turn_button.setButtonLabel(20, " End turn ");
    end_turn_button.set_activation_function([this]() {
        this->game_manager_->next_turn();
    });

    curr_x += end_turn_button.button.getSize().x + 20;

    RectButton deselect_unit_button(*font_, true, {curr_x, 650});
    deselect_unit_button.setButtonLabel(20, " Deselect unit ");
    deselect_unit_button.set_activation_function([this]() {
        this->game_manager_->deselect_unit();
    });

    main_buttons_.buttons.push_back(std::move(next_unit_button));
    main_buttons_.buttons.push_back(std::move(end_turn_button));
    main_buttons_.buttons.push_back(std::move(deselect_unit_button));


    main_buttons_.isActive = true;
}

void GUI::update_inventory() {
    // Don't show inventory of inactive team's units
    if (!game_manager_->selected_valid_unit()) {
        inventory_buttons_.clear_buttons();
        inventory_buttons_.isActive = false;
        return;
    }


    if (!selected_unit_changed_) {
        // If no active item 
        if (active_item == nullptr) {
            inventory_buttons_.clear_deactivate_button();
            return;
        }

        // If deselect button already exists, return
        if (inventory_buttons_.deactivate_button_exists()) return;

        RectButton button(*font_, true, {30, 460});
        button.setButtonLabel(20, "Deselect item");
        button.set_activation_function([this]() {
            this->active_item = nullptr;
        });

        inventory_buttons_.buttons.push_back(std::move(button));
        inventory_buttons_.deactivateButtonIdx = inventory_buttons_.buttons.size() - 1;

        return;
    }

    inventory_buttons_.clear_buttons();

    float curr_x = 30;
    const std::vector<std::shared_ptr<const Item>>& inventory = game_manager_->selected_unit_ptr()->get_inventory();

    for (unsigned int i = 0; i < unit_consts.inventory_size; i++) {
        RectButton button(*font_, true, {curr_x, 500});
        if (i < inventory.size()) {
            const std::shared_ptr<const Item>& item = inventory[i];
            button.setButtonLabel(20, inventory[i]->get_name());
            button.set_activation_function([this, item]() {
                this->active_item = item;
            });

        } else {
            button.setButtonLabel(20, "No item");
            button.toggle_button_disabled();
        }

        curr_x += button.button.getSize().x + 20;
        inventory_buttons_.buttons.push_back(std::move(button));
    }

    if (Building* building = map_->get_building(game_manager_->selected_unit_coords()).get();
        building != nullptr && building->is_ready()) 
    {
        RectButton button(*font_, true, {curr_x, 500});
        button.setButtonLabel(20, building->get_name());
        button.set_activation_function([this, building]() {
            this->active_item = building->get_item();
        });

        inventory_buttons_.buttons.push_back(std::move(button));
    }

    inventory_buttons_.isActive = true;
}

void GUI::draw_button_group(sf::RenderTarget& target, const RectButtonGroup& group) const {
    for (const RectButton& button : group.buttons) {
        button.draw(target);
    }
}

template <typename T>
void add_ptrs_to_vec(std::vector<T*>& dest, std::vector<T>& source) {
    dest.reserve(dest.size() + source.size());
    for (T& t : source) {
        dest.push_back(&t);
    }
}

std::vector<RectButton*> GUI::get_all_buttons() {
    std::vector<RectButton*> buttons;

    add_ptrs_to_vec(buttons, main_buttons_.buttons);
    add_ptrs_to_vec(buttons, inventory_buttons_.buttons);
    add_ptrs_to_vec(buttons, unit_buttons_.buttons);

    return buttons;
}
