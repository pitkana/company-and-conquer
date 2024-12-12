#ifndef INVENTORY_UI
#define INVENTORY_UI


#include <memory>
#include <span>
#include "SFML/Graphics.hpp"

#include "item.hpp"
#include "unit.hpp"
#include "matrix.hpp"
#include "game.hpp"
#include "auxiliary_renderable.hpp"

// Used for rendering the inventory of a specific Unit
class Inventory_UI : public Auxiliary_renderable
{
    public:

        Inventory_UI(const size_t width, const size_t height);

        // the padding of the item images from the sides of the inventory rectangle and from each other
        // the value is in pixels
        static const size_t padding = 10;
        /**
         * @brief Used to update which inventory to render, the std::span creates a
         * non-owning container for the std::vector in which the items are stored in the Unit.
         * This way passing the whole container as a parameter is efficient.
         */
        void update_inventory(const std::span<const std::shared_ptr<const Item>> items);

        /**
         * @brief A setter used for if a Unit is clicked or not, if the player does not click a Unit,
         * then this to <false> with a parameter, if Unit is clicked, set to <true>
         * 
         */
        void display_inv( const bool display_inv );

        void update() override;
        
        /**
         * @brief Used for defining the renderable for a specific item, the renderable will be
         * passed onto the sf::VertexArray
         * 
         * @param an_item 
         * @return sf::Vector2f 
         */
        std::unique_ptr<sf::Sprite> render_item(const Item& an_item, size_t position_idx);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(background_);

            for ( const std::unique_ptr<sf::Sprite>& spr : items_list_ ) 
            {
                target.draw(*spr, states);
            }
            
        }

        /**
         * @brief Used for updating the background_ member, written as own function so it doesnt have
         * to be explicitly written into multiple methods
         * 
         */
        void update_background()
        {
            size_t width = window_width_ / 2 + padding * 4;
            size_t height = window_height_ - (window_height_ / 8) + padding * 2;

            sf::Vector2f pos = { 2 * (window_width_ / 8) - padding * 2, window_height_ - (window_height_ / 8)};

            background_.setPosition( pos );

            sf::Texture texture;
            texture.create(width, height);
            //sf::Uint8* pixels = new sf::Uint8[width * height * 4](0xff);
            background_.setTexture( texture, true );
            background_.setColor( sf::Color(255, 255, 255) );
        }


        sf::Sprite background_;
        std::vector<std::unique_ptr<sf::Sprite>> items_list_;
        size_t window_width_ = 0;
        size_t window_height_ = 0;
        bool display_inv_ = true;
};

#endif
