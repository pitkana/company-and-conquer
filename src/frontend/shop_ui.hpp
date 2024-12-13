#ifndef SHOP_UI_HPP
#define SHOP_UI_HPP

#include "shop.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <../../libs/SFMLButton/include/sfmlbutton.hpp>

#include "renderer.hpp"

class Renderer;

class ShopUI : public Auxiliary_renderable {
    private:
        Shop& shop_;
        Renderer& renderer_;

        // containers for different types of buttons
        std::vector<RectButton> catalogue_buttons_;
        std::vector<RectButton> owned_buttons_;
        std::vector<RectButton> unit_buttons_;  // shows the units names in the buttons
        std::vector<RectButton> unit_owned_buttons_;  // shows the items that are owned by some unit
        std::vector<RectButton> game_button_;
        sf::Text budget_text_;  // the text for displaying how much money you have left after buying some items

        // methods that are used inside update() for updating the graphics for different buttons
        // and the budget text
        void update_catalogue();
        void update_owned();
        void update_units();
        void update_unit_inventory();
        void load_game_button();
        void update_budget();

        Unit* active_unit_ = nullptr;
        std::shared_ptr<const Item> active_item_;
        Team* team_ = nullptr;

        // font that will be used for the buttons and the budget text
        std::unique_ptr<sf::Font> font_ = std::make_unique<sf::Font>();

        bool catalogue_loaded_ = false;
        bool units_loaded_ = false;
        bool new_items_ = false;
        bool unit_changed_ = false;
        bool game_button_loaded_ = false;

        int budget_ = 0;

    public:

        void initialize();

        /**
         * @brief Construct a new Shop UI object
         * 
         * @param shop The shop object from which the different units and items will be rendered
         * @param renderer   a <Renderer> object that conains all the other renderables and the UI window
         */
        ShopUI(Shop& shop, Renderer& renderer) : shop_(shop), renderer_(renderer), budget_(shop.get_budget()) {}

        /**
         * @brief Used for handling button clicks
         * 
         * @param window The window in which the event occurred
         * @param event the <sf::Event>
         * @return true if a button was clicked,
         * @return false if the event was something else than a button click
         */
        bool execute_button_actions(sf::RenderWindow& window, sf::Event& event);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        void update();

};



#endif //SHOP_UI_HPP
