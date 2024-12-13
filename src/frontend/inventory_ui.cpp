#include "inventory_ui.hpp"



Inventory_UI::Inventory_UI(const size_t width, const size_t height) : window_width_(width), window_height_(height)
{
    items_list_.reserve( unit_consts.inventory_size );

    update_background();
}



bool Inventory_UI::load(const std::string& texture_path)
{
    if (!texture_.loadFromFile(texture_path)) {
        return false;
    }

    return true;
}

void Inventory_UI::update()
{
    update_background();
}



void Inventory_UI::display_inv( bool display_inv )
{
    display_inv_ = display_inv;
}


