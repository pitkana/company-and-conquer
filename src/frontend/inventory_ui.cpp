#include <memory>
#include <span>
#include "SFML/Graphics.hpp"

#include "item.hpp"
#include "matrix.hpp"

// Used for rendering the inventory of a specific Unit
class Inventory_UI : public sf::Drawable
{
    public:
        /**
         * @brief Used to update which inventory to render, the std::span creates a
         * non-owning container for the std::vector in which the items are stored in the Unit.
         * This way passing the whole container as a parameter is efficient.
         */
        void update_inventory(std::span<std::shared_ptr<const Item>>);

        /**
         * @brief Used for defining the renderable for a specific item, the renderable will be
         * passed onto the sf::VertexArray
         * 
         * @param an_item 
         * @return sf::Vector2f 
         */
        sf::Vector2f render_item(const Item& an_item);


    private:
        sf::VertexArray g_VertexArr;
};