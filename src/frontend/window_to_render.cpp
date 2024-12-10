#include "window_to_render.hpp"

Window_To_Render::Window_To_Render() { }

void Window_To_Render::add_drawable( const std::shared_ptr<Auxiliary_renderable> a_drawable )
{
    drawables_.push_back( a_drawable );
}


void Window_To_Render::update()
{
    for ( std::shared_ptr<Auxiliary_renderable>& a_drawable : drawables_ ) {
        a_drawable->update();
    }   
}


void Window_To_Render::clear()
{
    drawables_.clear();
}