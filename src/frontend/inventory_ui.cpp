#include "inventory_ui.hpp"





Inventory_UI::Inventory_UI(const size_t width, const size_t height) : window_width_(width), window_height_(height)
{
    items_list_.reserve( unit_consts.inventory_size );

    update_background();
}



void Inventory_UI::update_inventory(std::span<std::shared_ptr<const Item>> items)
{
    items_list_.clear();

    size_t idx = 0;
    for ( std::shared_ptr<const Item>& an_item : items ) {
        items_list_.push_back( render_item(*an_item, idx) );
        idx++;
    }
}

void Inventory_UI::update()
{
    update_background();
}



void Inventory_UI::display_inv( bool display_inv )
{
    display_inv_ = display_inv;
}

std::unique_ptr<sf::Sprite> Inventory_UI::render_item(const Item& an_item, size_t position_idx)
{
    size_t width = window_width_ / 2 + padding * 4;
    size_t height = window_height_ - (window_height_ / 8) + padding * 2;

    std::unique_ptr<sf::Sprite> item = std::make_unique<sf::Sprite>();
    sf::Vector2f pos = {( 2 + position_idx) *(window_width_ / 8) + padding, window_height_ - (window_height_ / 8) + padding};

    item->setPosition( pos );

    sf::Texture texture;
    texture.create(width, height);
    sf::Uint8* pixels = new sf::Uint8[width * height * 4](0xff);
    texture.update(pixels);

    item->setTexture( texture );


    return std::move(item);
}