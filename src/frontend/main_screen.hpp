#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <sfmlbutton.hpp>

class Renderer;

class MainScreen {
public:
    MainScreen(Renderer& renderer, size_t window_width, size_t window_height);

    void start(sf::RenderWindow& window);

private:
    std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();
    sf::Text title_;

    size_t window_width_;
    size_t window_height_;
    Renderer& renderer_;
};
