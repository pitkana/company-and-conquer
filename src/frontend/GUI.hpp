#pragma once

#include <memory>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>



struct RectButtonGroup {
    bool isActive = false;
    std::vector<RectButton> buttons;
};


class Game;

class GUI : public sf::Drawable {
public:
    GUI() {};
    GUI(std::shared_ptr<Game> game);

    void initialize();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw2(sf::RenderWindow& window);
    void execute_button_actions(sf::RenderWindow& window, sf::Event& event, size_t map_y, size_t map_x);


private:
    std::shared_ptr<Game> game_;

    sf::Font font_;

    RectButtonGroup always_active_buttons_;
    RectButtonGroup unit_buttons_;
};
