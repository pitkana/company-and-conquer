#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>

#include "coordinates.hpp"
#include "GUI.hpp"
#include "game.hpp"
#include "unit.hpp"


GUI::GUI(std::shared_ptr<Game> game):
    game_(game), map_(&game->get_map()) {}

void GUI::initialize() {
    if (!font_->loadFromFile("fonts/NotoSans-Bold.ttf")) {
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
        initialize_inventory();
    } else {
        inventory_buttons_.isActive = false;
        unit_buttons_.isActive = false;
    }

    // Set it to false after update so dont keep updating
    active_coords_changed = false;
}

bool GUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event) {

    for (RectButton* button : get_all_buttons()) {
        button->getButtonStatus(window, event);
        if (button->isPressed) {

            if (button->isActive) {
                button->activate(game_, active_coords.y, active_coords.x);
            }
            return true;
        } 
    }

    return false;
}

void GUI::click_on_coords(size_t y, size_t x) {
    coordinates<size_t> clicked_coords(x, y);
    if (use_active_item_on_coords(clicked_coords)) return;

    if (active_coords == clicked_coords){
        active_coords_changed = false;
        return;
    }

    if (enqueue_movement(clicked_coords)) return;

    active_unit_ptr = map_->get_unit(clicked_coords);

    // Don't select unit that isn't controllable
    if (!selected_unit_in_active_team()){
        active_unit_ptr = nullptr;
    }
    active_coords = std::move(clicked_coords);
    active_coords_changed = true;
}

bool GUI::selected_unit_in_active_team() const {
    if (active_unit_ptr == nullptr) return false;
    return game_->get_unit_team_id(active_unit_ptr->get_id()) == game_->get_active_team()->get_id();
}

void GUI::initialize_main_buttons() {
    main_buttons_.isActive = true;
}

void GUI::initialize_inventory() {
    // Don't show inventory of inactive team's units
    if (!selected_unit_in_active_team()) {
        inventory_buttons_.clear_buttons();
        return;
    }


    if (!active_coords_changed) {
        // If no active item 
        if (active_item == nullptr) {
            inventory_buttons_.clear_deactivate_button();
            return;
        }

        // If deselect button already exists, return
        if (inventory_buttons_.deactivate_button_exists()) return;

        RectButton button(*font_, true, {30, 460});
        button.setButtonLabel(20, "Deselect item");
        button.set_activation_function([this](const std::shared_ptr<Game>& game, size_t y, size_t x) {
            this->active_item = nullptr;
        });

        inventory_buttons_.buttons.push_back(std::move(button));
        inventory_buttons_.deactivateButtonIdx = inventory_buttons_.buttons.size() - 1;

        return;
    }

    inventory_buttons_.clear_buttons();

    float curr_x = 30;
    const std::vector<std::shared_ptr<const Item>>& inventory = active_unit_ptr->get_inventory();

    for (unsigned int i = 0; i < unit_consts.inventory_size; i++) {
        RectButton button(*font_, true, {curr_x, 500});
        if (i < inventory.size()) {
            const std::shared_ptr<const Item>& item = inventory[i];
            button.setButtonLabel(20, inventory[i]->get_name());
            button.set_activation_function([this, item](const std::shared_ptr<Game>& game, size_t y, size_t x) {
                this->active_item = item;
            });

        } else {
            button.setButtonLabel(20, "No item");
            button.toggle_button_disabled();
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

bool GUI::use_active_item_on_coords(const coordinates<size_t>& coords) {
    if (!selected_unit_in_active_team() ||
        active_item == nullptr ||
        active_unit_ptr->has_added_action) return false;

    std::vector<coordinates<size_t>> coords_can_shoot_on = map_->tiles_can_shoot_on(active_coords, unit_consts.visual_range);
    if (auto it = std::find(coords_can_shoot_on.begin(), coords_can_shoot_on.end(), coords);
        it == coords_can_shoot_on.end()) return false;

    Team* active_team = game_->get_active_team();
    std::shared_ptr<Action> action = active_item->get_action(coords, *active_unit_ptr);

    //After successful action, deactivate item
    active_item = nullptr;
    return game_->add_action(std::move(action), game_->get_active_team()->get_id());
}

bool GUI::enqueue_movement(const coordinates<size_t>& coords) {
    if (!selected_unit_in_active_team() ||
        active_unit_ptr->has_moved) return false;

    std::vector<coordinates<size_t>> coords_can_move_to = map_->possible_tiles_to_move_to3(active_coords, unit_consts.move_range);
    if (auto it = std::find(coords_can_move_to.begin(), coords_can_move_to.end(), coords);
        it == coords_can_move_to.end()) return false;

    Team* active_team = game_->get_active_team();
    std::shared_ptr<Action> movement = std::make_shared<MovementAction>(active_coords, coords, *active_unit_ptr);
    game_->add_action(std::move(movement), active_team->get_id());

    return true;
}
