#pragma once

#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>
#include <limits>

#include "coordinates.hpp"
#include "auxiliary_renderable.hpp"
#include "inventory_ui.hpp"
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

class GUI : public Auxiliary_renderable
{
public:
    static const size_t padding = 10;


    GUI() {};
    GUI(std::shared_ptr<Game_Manager> manager, size_t width, size_t height);

    void initialize();

    void update() override;
    // Return true if a button was pressed and the event should be consumed
    bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

    void click_on_coords(size_t y, size_t x);

    const Item* get_active_item() const;

    void deselect_unit();
    void undo_action();
    void next_turn();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

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
    sf::Vector2f active_item_pos_;

    std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

    // the inventory backround
    std::shared_ptr<Inventory_UI> r_inv_;

    size_t width_ = 0;
    size_t height_ = 0;
    

    RectButtonGroup main_buttons_;
    RectButtonGroup unit_buttons_;
    InventoryButtonGroup inventory_buttons_;
};
