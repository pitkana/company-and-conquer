#ifndef WINDOW_TO_RENDER
#define WINDOW_TO_RENDER


#include <memory>
#include "SFML/Graphics.hpp"

/**
 * @brief Our Window object that we will pass to the sf::RenderWindow to be rendered,
 * this will store all the other renderables
 * 
 */
class Window_To_Render : public sf::Drawable, public sf::Transformable
{
    public: 
        Window_To_Render();

        /**
         * @brief add a drawable that will be drawn onto the screen
         * 
         * @param a_drawable 
         */
        void add_drawable( const std::shared_ptr<sf::Drawable> a_drawable );

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            for ( const std::shared_ptr<sf::Drawable>& a_drawable : drawables_ ) {
                target.draw( *a_drawable, states );
            }

        }

        std::vector<std::shared_ptr<sf::Drawable>> drawables_;

};


#endif