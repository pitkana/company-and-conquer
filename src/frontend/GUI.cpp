#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <span>

#include "coordinates.hpp"
#include "GUI.hpp"
#include "game.hpp"
#include "unit.hpp"


GUI::GUI(std::shared_ptr<Game_Manager> manager, size_t width, size_t height):
    game_manager_(manager), width_(width), height_(height), map_(&game_manager_->get_map())
{
    r_inv_ = std::make_shared<Inventory_UI>( width, height_ ); // render_window_->getSize().x, render_window_->getSize().y );
}

void GUI::initialize() {
    inventory_buttons_start_pos_ = { static_cast<float>((width_ / 6) + r_inv_->padding / 2), static_cast<float>(height_ - (height_ / 6 - r_inv_->padding / 2))};
    main_buttons_start_pos_ = inventory_buttons_start_pos_ + main_buttons_pos_relative_to_inv_buttons_;

    if (!font_->loadFromFile(GUI_FONT_PATH)) {
        assert(false && "Loading font failed");
    }

    if ( !r_inv_->load( INVENTORY_TEXTURE_PATH ) ) {
        assert(false && "Loading inventory image failed");
    }

    initialize_main_buttons();

}

void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    draw_button_group(target, main_buttons_);

    if (inventory_buttons_.isActive) {
        target.draw( *r_inv_ );
        draw_button_group(target, inventory_buttons_);
    }
}

void GUI::update() {
    update_inventory();
    r_inv_->update();

    // Set this to false so that the GUI is not updated unnecessarily
    selected_unit_changed_ = false;
}

bool GUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event) {

    // Check if hovering a button and update variable accordingly
    is_hovering_a_button_ = false;

    for (RectButton* button : get_all_buttons()) {
        button->getButtonStatus(window, event);

        if (!button->isHover) continue;

        is_hovering_a_button_ = true;
        // If the button is pressed but not active, still consume the event aka return true
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

void GUI::deselect_unit() {
    game_manager_->deselect_unit();
    active_item = nullptr;
}

void GUI::undo_action() {
    game_manager_->undo_action();
    active_item = nullptr;
}

void GUI::next_turn() {
    game_manager_->next_turn();
    active_item = nullptr;
}

void GUI::initialize_main_buttons() {
    sf::Vector2f pos = main_buttons_start_pos_;

    RectButton next_unit_button(*font_, true, pos);
    next_unit_button.setButtonLabel(20, " Next unit ");
    next_unit_button.set_activation_function([this]() {
        this->game_manager_->cycle_units(width_, height_);
        this->selected_unit_changed_ = true;
        active_item = nullptr;
    });

    pos.x += next_unit_button.button.getSize().x + r_inv_->padding;

    RectButton end_turn_button(*font_, true, pos);
    end_turn_button.setButtonLabel(20, " End turn ");
    end_turn_button.set_activation_function([this]() {
        this->game_manager_->next_turn();
        active_item = nullptr;
    });
    pos.x += end_turn_button.button.getSize().x + r_inv_->padding;

    RectButton undo_action_button(*font_, true, pos);
    undo_action_button.setButtonLabel(20, " Undo action ");
    undo_action_button.set_activation_function([this]() {
        this->undo_action();
    });
    pos.x += undo_action_button.button.getSize().x + r_inv_->padding;

    RectButton deselect_unit_button(*font_, true, pos);
    deselect_unit_button.setButtonLabel(20, " Deselect unit ");
    deselect_unit_button.set_activation_function([this]() {
        this->game_manager_->deselect_unit();
        active_item = nullptr;
    });
    pos.x += deselect_unit_button.button.getSize().x + r_inv_->padding;

    RectButton toggle_logs_button(*font_, true, pos);
    toggle_logs_button.setButtonLabel(20, " Toggle logs ");
    toggle_logs_button.set_activation_function([this]() {
        this->are_logs_active = !this->are_logs_active;
    });
    pos.x += toggle_logs_button.button.getSize().x + r_inv_->padding;

    main_buttons_.buttons.push_back(std::move(next_unit_button));
    main_buttons_.buttons.push_back(std::move(end_turn_button));
    main_buttons_.buttons.push_back(std::move(undo_action_button));
    main_buttons_.buttons.push_back(std::move(deselect_unit_button));
    main_buttons_.buttons.push_back(std::move(toggle_logs_button));


    main_buttons_.isActive = true;
}

void GUI::update_inventory() {
    // Don't show inventory of inactive team's units
    if (!game_manager_->selected_valid_unit()) {
        inventory_buttons_.clear_buttons();
        inventory_buttons_.isActive = false;
        return;
    }

    // we calculate some sizes for the buttons depending on the window size
    float button_width = 150;
    float button_height = 130;

    // also calculate the relative position of the first item buttom
    sf::Vector2f pos = inventory_buttons_start_pos_;

    if (!selected_unit_changed_) {
        // If no active item 
        if (active_item == nullptr) {
            inventory_buttons_.clear_deactivate_button();
            return;
        }

        // If active item has changed, redraw the deactivate button on top of currently active item, if not, return
        if (!active_item_changed_)
            return;

        active_item_changed_ = false;
        inventory_buttons_.clear_deactivate_button();

        RectButton button(*font_, true, {active_item_pos_.x, active_item_pos_.y - 4 * r_inv_->padding});
        button.setButtonLabel(20, " Deselect item ");
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


    // Draw the buttons for items in the inventory
    for (unsigned int i = 0; i < unit_consts.inventory_size; i++) {
        RectButton button(*font_, {button_width, button_height}, pos);
        if (i < inventory.size()) {
            const std::shared_ptr<const Item>& item = inventory[i];
            button.setButtonLabel(20, inventory[i]->get_name());
            button.set_activation_function([this, item, pos]() {
                this->active_item = item;
                this->active_item_pos_ = pos;
                this->active_item_changed_ = true;
            });

        } else { // this means no item in that slot
            button.setButtonLabel(20, "No item");
            button.toggle_button_disabled();
        }

        pos.x += button.button.getSize().x + r_inv_->padding;
        inventory_buttons_.buttons.push_back(std::move(button));
    }


    // If there is a fully built building under the unit, add a button so that it can be used as well
    if (Building* building = map_->get_building(game_manager_->selected_unit_coords()).get();
        building != nullptr && building->is_ready()) 
    {
        RectButton button(*font_, {button_width, button_height}, pos);
        button.setButtonLabel(20, building->get_name());
        button.set_activation_function([this, building, pos]() {
            this->active_item = building->get_item();
            this->active_item_pos_ = pos;
            this->active_item_changed_ = true;
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

    return buttons;
}

const Item* GUI::get_active_item() const { return active_item.get(); }
