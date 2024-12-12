#include "shop_ui.hpp"

#include <cassert>

void ShopUI::initialize() {
    if (!font_->loadFromFile(GUI_FONT_PATH)) {
        assert(false && "Loading font failed");
    }

}

void ShopUI::update_catalogue()
{
    catalogue_loaded_ = true;
    catalogue_buttons_.clear();

    float curr_x = 30;

    for (auto item : shop_.get_catalogue())
    {
        RectButton button = RectButton(*font_, true, {curr_x, 30});

        std::string label = item.first->get_name() + ": " + std::to_string(item.second);

        button.setButtonLabel(20,  label);
        button.set_activation_function([this, item]()
        {
            this->new_items_ = true;
            this->shop_.buy_item(item.first);
        });

        catalogue_buttons_.push_back(button);

        curr_x += button.button.getSize().x + 20;
    }
}

void ShopUI::update_owned()
{
    owned_buttons_.clear();

    float curr_x = 30;

    for (auto item : shop_.get_owned())
    {
        RectButton button = RectButton(*font_, true, {curr_x, 80});

        std::string label = item->get_name();

        button.setButtonLabel(20,  label);

        button.set_activation_function([this, item]()
        {
            this->active_item_ = item;
            this->units_loaded_ = false;
        });

        owned_buttons_.push_back(button);

        curr_x += button.button.getSize().x + 20;
    }


}

void ShopUI::update_units()
{
    units_loaded_ = true;
    unit_buttons_.clear();

    float curr_x = 30;

    for (auto& unit : shop_.get_units())
    {
        RectButton button = RectButton(*font_, true, {curr_x, 130});

        Unit* unit_pointer = &unit;
        std::string label = unit.get_name();
        button.setButtonLabel(20,  label);
        button.set_activation_function([this, unit_pointer]()
        {
            this->unit_changed_ = true;
            this->active_unit_ = unit_pointer;
            if (active_item_ != nullptr)
            {
                this->shop_.assign_to_unit(active_item_, unit_pointer);
                this->active_item_ = nullptr;
            }
        });
        unit_buttons_.push_back(button);
        curr_x += button.button.getSize().x + 20;
    }
    RectButton refund_button = RectButton(*font_, true, {curr_x, 130});
    refund_button.setButtonLabel(20, "Refund");
    refund_button.set_activation_function([this]()
    {
        if (active_item_ != nullptr)
        {
            shop_.refund_item(active_item_);
            this->active_item_ = nullptr;
        }
    });
    unit_buttons_.push_back(refund_button);
}

void ShopUI::update_unit_inventory()
{
    unit_owned_buttons_.clear();

    if (active_unit_ != nullptr)
    {
        float curr_x = 30;

        for (auto item : active_unit_->get_inventory())
        {
            RectButton button = RectButton(*font_, true, {curr_x, 180});

            std::string label = item->get_name();

            button.setButtonLabel(20,  label);

            button.set_activation_function([this, item]()
            {
                this->unit_changed_ = true;
                this->shop_.retrieve_from_unit(item, active_unit_);
            });

            unit_owned_buttons_.push_back(button);

            curr_x += button.button.getSize().x + 20;
        }
    }
}

void ShopUI::load_game_button()
{
    game_button_loaded_ = true;
    RectButton button = RectButton(*font_, true, {30, 230});
    button.setButtonLabel(20, "Start Game");
    button.set_activation_function([this]()
    {
        Team team = shop_.form_team();
        renderer_.ready_game();
    });
    game_button_.push_back(button);
}

void ShopUI::update()
{
    if (!catalogue_loaded_)
    {
        update_catalogue();
    }
    if (new_items_)
    {
        update_owned();
    }
    if (!units_loaded_)
    {
        update_units();
    }
    if (unit_changed_)
    {
        update_unit_inventory();
    }
    if (!game_button_loaded_)
    {
        load_game_button();
    }
}

void ShopUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto button : catalogue_buttons_)
    {
        button.draw(target);
    }
    for (auto button : owned_buttons_)
    {
        button.draw(target);
    }
    for (auto button : unit_buttons_)
    {
        button.draw(target);
    }
    for (auto button : unit_owned_buttons_)
    {
        button.draw(target);
    }
    for (auto button : game_button_)
    {
        button.draw(target);
    }
}

bool ShopUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event) {

    for (auto button : catalogue_buttons_) {
        button.getButtonStatus(window, event);
        if (button.isPressed) {

            button.activate();
            return true;
        }
    }

    for (auto button : owned_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed) {
            button.activate();
            return true;
        }
    }

    for (auto button : unit_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed)
        {
            button.activate();
            return true;
        }
    }

    for (auto button : unit_owned_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed)
        {
            button.activate();
            return true;
        }
    }

    for (auto button : game_button_) {
        button.getButtonStatus(window, event);
        if (button.isPressed) {
            button.activate();
            return true;
        }
    }

    return false;
}