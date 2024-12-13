#ifndef SHOP_UI_HPP
#define SHOP_UI_HPP

#include "shop.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>

#include "renderer.hpp"

class Renderer;

class ShopUI : public sf::Drawable {
    private:
        Shop& shop_;
        Renderer& renderer_;

        std::vector<RectButton> catalogue_buttons_;
        std::vector<RectButton> owned_buttons_;
        std::vector<RectButton> unit_buttons_;
        std::vector<RectButton> unit_owned_buttons_;
        std::vector<RectButton> game_button_;

        void update_catalogue();
        void update_owned();
        void update_units();
        void update_unit_inventory();
        void load_game_button();

        Unit* active_unit_ = nullptr;
        std::shared_ptr<const Item> active_item_;
        Team* team_ = nullptr;

        std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

        bool catalogue_loaded_ = false;
        bool units_loaded_ = false;
        bool new_items_ = false;
        bool unit_changed_ = false;
        bool game_button_loaded_ = false;

    public:

        void initialize();

        ShopUI(Shop& shop, Renderer& renderer) : shop_(shop), renderer_(renderer) {}

        bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

};



#endif //SHOP_UI_HPP
