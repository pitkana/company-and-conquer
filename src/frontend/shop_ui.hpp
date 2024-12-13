#ifndef SHOP_UI_HPP
#define SHOP_UI_HPP

#include "shop.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
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
        sf::Text budget_text_;

        // Update methods for button groups
        void update_catalogue();
        void update_owned();
        void update_units();
        void update_unit_inventory();
        void load_game_button();
        void update_budget();

        Unit* active_unit_ = nullptr;
        std::shared_ptr<const Item> active_item_;
        Team* team_ = nullptr;

        std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

        bool catalogue_loaded_ = false;
        bool units_loaded_ = false;
        bool new_items_ = false;
        bool unit_changed_ = false;
        bool game_button_loaded_ = false;

        int budget_ = 0;

    public:

        void initialize();

        ShopUI(Shop shop, Renderer& renderer) : shop_(shop), renderer_(renderer), budget_(shop.get_budget()) {}

        /**
         * Event handler for button actions
         * @param window the window the event is contained in
         * @param event The current event
         * @return Whether a button action was executed
         */
        bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

};



#endif //SHOP_UI_HPP
