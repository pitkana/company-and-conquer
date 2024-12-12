#ifndef SHOP_UI_HPP
#define SHOP_UI_HPP

#include "shop.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>

class ShopUI : public sf::Drawable {
    private:
        Shop& shop_;

        std::vector<RectButton> catalogue_buttons_;
        std::vector<RectButton> owned_buttons_;
        std::vector<RectButton> unit_buttons_;
        std::vector<RectButton> unit_owned_buttons_;

        void update_catalogue();
        void update_owned();
        void update_units();
        void update_unit_inventory();

        Unit* active_unit_ = nullptr;
        std::shared_ptr<const Item> active_item_;

        std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

        bool catalogue_loaded_ = false;
        bool units_loaded_ = false;
        bool new_items_ = false;
        bool unit_changed_ = false;



    public:

        void initialize();

        ShopUI(Shop& shop) : shop_(shop) {}

        bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

};



#endif //SHOP_UI_HPP
