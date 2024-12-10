#include "window_to_render.hpp"

Window_To_Render::Window_To_Render() { }

void Window_To_Render::add_drawable( const std::shared_ptr<sf::Drawable> a_drawable )
{
    drawables_.push_back( a_drawable );
}