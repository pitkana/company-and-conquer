#include <string>
#include <cstdint>

#include "shop_ui.hpp"

#include <cassert>

void ShopUI::initialize() {
    if (!font_->loadFromFile(GUI_FONT_PATH)) {
        assert(false && "Loading font failed");
    }
    budget_text_.setFont( *font_ );
    budget_text_.setFillColor( sf::Color::White );
    budget_text_.setCharacterSize(20);
    budget_text_.setPosition(30, 0);
}


void ShopUI::update_catalogue()
{
    game_button_updated_ = true;
    catalogue_buttons_.clear();

    float curr_x = 30;
    float starting_y0 = starting_y_;

    uint8_t count = 0;

    for (auto item : shop_.get_catalogue())
    {
        RectButton button = RectButton(*font_, true, {curr_x, starting_y0});

        std::string label = item.first->get_name() + ": " + std::to_string(item.second);

        button.setButtonLabel(20,  label);
        button.set_activation_function([this, item]()
        {
            this->game_button_updated_ = true;
            this->shop_.buy_item(item.first);
        });

        catalogue_buttons_.push_back(button);

        curr_x += button.button.getSize().x + 20;
        count++;

        if ( count > 5 ) {
            starting_y0 += 50;
            count = 0;
            curr_x = 30;
            starting_y_ = starting_y0 + 50;
        }
    }
    
}

void ShopUI::update_owned()
{
    owned_buttons_.clear();

    float curr_x = 30;
    float starting_y0 = starting_y_;

    uint8_t count = 0;

    for (auto item : shop_.get_owned())
    {
        RectButton button = RectButton(*font_, true, {curr_x, starting_y0});

        std::string label = item->get_name();

        button.setButtonLabel(20,  label);

        
        if (item == active_item_) {
            button.button.setFillColor(sf::Color::Magenta);
            button.toggle_updating_color();
        }
        
        

        button.set_activation_function([this, item]()
        {
            this->active_item_ = item;
            this->game_button_updated_ = true;
        });

        owned_buttons_.push_back(button);

        curr_x += button.button.getSize().x + 20;
        count++;

        if ( count > 5 ) {
            starting_y0 += 50;
            count = 0;
            curr_x = 30;
            starting_y_ = starting_y0 + 50;
        }
    }

    // starting_y_ = starting_y0 + 50;

    RectButton deselect = RectButton(*font_, true, {curr_x, starting_y0});
    deselect.setButtonLabel(20, "Deselect");
    deselect.set_activation_function([this]() {
        this->game_button_updated_ = true;
        this->active_item_ = nullptr;
    });
    owned_buttons_.push_back(deselect);
}


void ShopUI::update_units()
{
    game_button_updated_ = true;
    unit_buttons_.clear();

    float curr_x = 30;
    float starting_y0 = starting_y_;

    uint8_t count = 0;

    for (auto& unit : shop_.get_units())
    {
        RectButton button = RectButton(*font_, true, {curr_x, starting_y0});

        Unit* unit_pointer = &unit;
        std::string label = unit.get_name();
        button.setButtonLabel(20,  label);

        
        if (unit_pointer == active_unit_) {
            button.button.setFillColor(sf::Color::Magenta);
            button.toggle_updating_color();
        }

        button.set_activation_function([this, unit_pointer]()
        {
            this->game_button_updated_ = true;
            this->active_unit_ = unit_pointer;
            if (active_item_ != nullptr)
            {
                this->shop_.assign_to_unit(active_item_, unit_pointer);
                this->active_item_ = nullptr;
            }
        });
        unit_buttons_.push_back(button);
        curr_x += button.button.getSize().x + 20;
        count++;

        if ( count > 5 ) {
            starting_y0 += 50;
            count = 0;
            curr_x = 30;
            starting_y_ = starting_y0 + 50;
        }
    }

    // starting_y_ = starting_y0 + 50;


    RectButton refund_button = RectButton(*font_, true, {curr_x, starting_y0});
    refund_button.setButtonLabel(20, "Refund");
    refund_button.set_activation_function([this]()
    {
        this->game_button_updated_ = true;
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
        float starting_y0 = starting_y_;

        uint8_t count = 0;

        for (auto item : active_unit_->get_inventory())
        {
            RectButton button = RectButton(*font_, true, {curr_x, starting_y0});

            std::string label = item->get_name();

            button.setButtonLabel(20,  label);

            button.set_activation_function([this, item]()
            {
                this->game_button_updated_ = true;
                this->shop_.retrieve_from_unit(item, active_unit_);
            });

            unit_owned_buttons_.push_back(button);

            curr_x += button.button.getSize().x + 20;
            count++;

            if ( count > 5 ) {
                starting_y0 += 50;
                count = 0;
                curr_x = 30;
                starting_y_ = starting_y0 + 50;
            }
        }
        
    }
}


void ShopUI::update_budget()
{
    std::string aux = "budget left: ";
    budget_ = shop_.get_budget();
    budget_text_.setString( aux + std::to_string(budget_) );
}


void ShopUI::load_game_button()
{
    game_button_updated_ = true;
    std::unique_ptr<RectButton> button = std::make_unique<RectButton>(*font_, true, sf::Vector2f{30, starting_y_});

    button->setButtonLabel(20, "Start Game");
    button->set_activation_function([this]()
    {
        Team team = shop_.form_team();
        renderer_.ready_game();
    });

    game_button_ = std::move(button);
}

void ShopUI::update()
{
    if ( game_button_updated_ ) {
        update_catalogue();
        starting_y_ += 50;

        update_owned();
        starting_y_ += 50;

        update_units();
        starting_y_ += 50;

        update_unit_inventory();
        starting_y_ += 50;

        load_game_button();
        update_budget();
    }
    
    game_button_updated_ = false;

    //reset the starting y position for the buttons again at the original position
    starting_y_ = 30;
}

void ShopUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const RectButton& button : catalogue_buttons_)
    {
        button.draw(target);
    }
    for (const RectButton& button : owned_buttons_)
    {
        button.draw(target);
    }
    for (const RectButton& button : unit_buttons_)
    {
        button.draw(target);
    }
    for (const RectButton& button : unit_owned_buttons_)
    {
        button.draw(target);
    }

    game_button_->draw(target);
    
    target.draw(budget_text_);
}

bool ShopUI::execute_button_actions(sf::RenderWindow& window, sf::Event& event) {

    for (RectButton& button : catalogue_buttons_) {
        button.getButtonStatus(window, event);
        if (button.isPressed) {

            button.activate();
            return true;
        }
    }

    for (RectButton& button : owned_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed) {
            button.activate();
            return true;
        }
    }

    for (RectButton& button : unit_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed)
        {
            button.activate();
            return true;
        }
    }

    for (RectButton& button : unit_owned_buttons_)
    {
        button.getButtonStatus(window, event);
        if (button.isPressed)
        {
            button.activate();
            return true;
        }
    }

    
    if ( game_button_ ) {
        game_button_->getButtonStatus(window, event);
        if (game_button_->isPressed) {
            game_button_->activate();
            return true;
        }
    } 


    

    return false;
}
