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
         * @brief A setter used for if a Unit is clicked or not, if the player does not click a Unit,
         * then this to <false> with a parameter, if Unit is clicked, set to <true>
         * 
         */
        void display_inv( const bool display_inv );

        bool load(const std::string& texture_path);

        void update() override;
        

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
            float width = (padding + 150) * 4;   // the calculation comes from 4 / 6, where 4 is the amount of items
            float height = 130;

            float scale_x = width / texture_.getSize().x;
            float scale_y = height / texture_.getSize().y;

            background_.setTexture( texture_, true );
            background_.setScale( scale_x, scale_y );

            sf::Vector2f pos = {static_cast<float>((window_width_ / 6)), static_cast<float>(window_height_ - (window_height_ / 6))};

            background_.setPosition( pos );


            //sf::Uint8* pixels = new sf::Uint8[width * height * 4](0xff);
            
            //background_.setColor( sf::Color(255, 255, 255) );
        }


        sf::Sprite background_;
        sf::Texture texture_;
        std::vector<std::unique_ptr<sf::Sprite>> items_list_;
        size_t window_width_ = 0;
        size_t window_height_ = 0;
        bool display_inv_ = true;
};

#endif
