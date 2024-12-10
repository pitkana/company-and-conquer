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

        Inventory_UI(const std::shared_ptr<Game>& game);

        // the padding of the item images from the sides of the inventory rectangle and from each other
        // the value is in pixels
        static const size_t padding = 10;
        /**
         * @brief Used to update which inventory to render, the std::span creates a
         * non-owning container for the std::vector in which the items are stored in the Unit.
         * This way passing the whole container as a parameter is efficient.
         */
        void update_inventory(std::span<std::shared_ptr<const Item>> items);

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

            if ( display_inv_ ) {
                target.draw(background_);

                for ( const std::unique_ptr<sf::Sprite>& spr : items_list_ ) 
                {
                    target.draw(*spr, states);
                }
            } 

            
        }


        sf::Sprite background_;
        std::vector<std::unique_ptr<sf::Sprite>> items_list_;
        std::shared_ptr<Game> game_;
        bool display_inv_ = false;
};

#endif