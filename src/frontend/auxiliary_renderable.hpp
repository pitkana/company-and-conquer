#ifndef AUXILIARY_RENDERABLE
#define AUXILIARY_RENDERABLE

#include <memory>
#include "SFML/Graphics.hpp"


/**
 * @brief Use this for inheritance instead of straight up using the sfml classes so that
 * window_to_render can call update() override on all of them
 * 
 */
class Auxiliary_renderable : public sf::Drawable, public sf::Transformable
{
    public:
        virtual void update() = 0;

    protected: 
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};



#endif