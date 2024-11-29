#include "window_to_render.hpp"

void Window_To_Render::add_drawable( std::shared_ptr<sf::Drawable>& a_drawable )
{
    drawables_.push_back( a_drawable );
}