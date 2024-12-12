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
        std::vector<RectButton> action_buttons_;

        void update_catalogue();
        void update_owned();
        void update_unit_inventory();


    public:
        ShopUI(Shop& shop) : shop_(shop) {}

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

};



#endif //SHOP_UI_HPP
