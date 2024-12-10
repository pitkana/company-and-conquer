#pragma once

#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>

#include "coordinates.hpp"



struct RectButtonGroup {
    bool isActive = false;
    std::vector<RectButton> buttons;
};


class Game;
class Map;

class GUI : public sf::Drawable {
public:
    GUI() {};
    GUI(std::shared_ptr<Game> game);

    void initialize();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    void execute_button_actions(sf::RenderWindow& window, sf::Event& event, size_t map_y, size_t map_x);

    void set_active_coords(size_t y, size_t x);

private:
    void initialize_main_buttons();
    void initialize_inventory();
    void initialize_movement();

    void draw_button_group(sf::RenderTarget& target, const RectButtonGroup& group) const;

private:
    std::shared_ptr<Game> game_;
    Map* map_;

    coordinates<size_t> active_coords;
    sf::Font font_;

    RectButtonGroup main_buttons_;
    RectButtonGroup unit_buttons_;
    RectButtonGroup inventory_buttons_;
};
