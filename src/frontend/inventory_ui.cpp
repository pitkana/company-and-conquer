#include "inventory_ui.hpp"

Inventory_UI::Inventory_UI()
{
    items_list_.reserve( unit_consts.inventory_size );
}

Inventory_UI::Inventory_UI(const std::shared_ptr<Game>& game) : game_(game)
{
    items_list_.reserve( unit_consts.inventory_size );
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
    sf::Vector2f pos = { 2 * (game_->get_map().width() / 8), game_->get_map().height() - (game_->get_map().height() / 8)};

    background_.setPosition( pos );

    sf::Texture texture;
    texture.create(100, 400);
    sf::Uint8* pixels = new sf::Uint8[100 * 400 * 4](0xff);
    background_.setTexture( texture );
    background_.setColor( sf::Color(255, 255, 255) );
}

std::unique_ptr<sf::Sprite> Inventory_UI::render_item(const Item& an_item, size_t position_idx)
{
    std::unique_ptr<sf::Sprite> item = std::make_unique<sf::Sprite>();
    sf::Vector2f pos = {( 2 + position_idx) *(game_->get_map().width() / 8) + padding, game_->get_map().height() - (game_->get_map().height() / 8) + padding};

    item->setPosition( pos );

    sf::Texture texture;
    texture.create(100, 100);
    sf::Uint8* pixels = new sf::Uint8[100 * 100 * 4](0xff);
    texture.update(pixels);

    item->setTexture( texture );


    return std::move(item);
}