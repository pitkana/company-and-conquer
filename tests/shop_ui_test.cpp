#include <SFML/Window.hpp>
#include "shop_ui.hpp"
#include "shop.hpp"

#include "shop_ui_test.hpp"

void shop_ui_test()
{
    sf::RenderWindow window(sf::VideoMode(1000, 720), "Shop UI Test");

    std::shared_ptr<Weapon> rifle = std::make_shared<Weapon>("Rifle", 100, 50, 5);
    std::shared_ptr<Weapon> pistol = std::make_shared<Weapon>("Pistol", 60, 40, 10);
    std::shared_ptr<HealingItem> bandage = std::make_shared<HealingItem>("Bandage", 30, 0);

    std::map<std::shared_ptr<const Item>, int> catalogue{{std::static_pointer_cast<Item>(rifle), 100}, {std::static_pointer_cast<Item>(bandage), 50}, {std::static_pointer_cast<Item>(pistol), 75}};

    Shop shop = Shop(catalogue, 5, 600);

    ShopUI ui = ShopUI(shop);

    ui.initialize();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ui.execute_button_actions(window, event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        ui.update();
        window.draw(ui);
        window.display();
    }
}