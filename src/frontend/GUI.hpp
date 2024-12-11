#pragma once

#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>
#include <limits>

#include "coordinates.hpp"
#include "game_manager.hpp"



struct RectButtonGroup {
    bool isActive = false;
    std::vector<RectButton> buttons;

    virtual inline void clear_buttons() {
        buttons.clear();
    }
};

struct InventoryButtonGroup : public RectButtonGroup {

    int deactivateButtonIdx = -1;

    inline void clear_buttons() override {
        buttons.clear();
        deactivateButtonIdx = -1;
    }

    inline void clear_deactivate_button() {
        if (deactivateButtonIdx == -1) return;
        buttons.erase(buttons.begin() + deactivateButtonIdx);
        deactivateButtonIdx = -1;
    }

    inline bool deactivate_button_exists() const {
        return deactivateButtonIdx != -1;
    }

};


class Game;
class Map;
class Item;
class Unit;

class GUI : public sf::Drawable {
public:
    GUI();
    GUI(std::shared_ptr<Game_Manager> game_manager);

    void initialize();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    // Return true if a button was pressed and the event should be consumed
    bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

    void click_on_coords(size_t y, size_t x);

private:
    bool selected_unit_in_active_team() const;

    void initialize_main_buttons();
    void update_inventory();
    void initialize_movement();

    void draw_button_group(sf::RenderTarget& target, const RectButtonGroup& group) const;

    std::vector<RectButton*> get_all_buttons();

private:
    std::shared_ptr<Game_Manager> game_manager_;
    Map* map_;

    bool selected_unit_changed_ = false;

    std::shared_ptr<const Item> active_item;

    std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

    RectButtonGroup main_buttons_;
    RectButtonGroup unit_buttons_;
    InventoryButtonGroup inventory_buttons_;
};
