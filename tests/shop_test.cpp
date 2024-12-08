#include "unit.hpp"
#include "item.hpp"
#include "shop.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>

int shop_test() {
    std::shared_ptr<Weapon> rifle = std::make_shared<Weapon>("Rifle", 100, 50, 5);
    std::shared_ptr<Weapon> pistol = std::make_shared<Weapon>("Pistol", 60, 40, 10);
    std::shared_ptr<HealingItem> bandage = std::make_shared<HealingItem>("Bandage", 30, 0);

    std::map<std::shared_ptr<const Item>, int> catalogue{{std::static_pointer_cast<Item>(rifle), 100}, {std::static_pointer_cast<Item>(bandage), 50}, {std::static_pointer_cast<Item>(pistol), 75}};

    Shop shop = Shop(catalogue, 5, 600);

    std::cout << "Shop initialized:" << std::endl;
    std::cout << "Budget: " << shop.get_budget() << std::endl;

    auto shop_catalogue = shop.get_catalogue();
    auto units = shop.get_units();

    std::cout << "Catalogue:" << std::endl;
    for (auto it = shop_catalogue.begin(); it != shop_catalogue.end(); it++)
    {
        std::cout << it->first->get_name() << ": " << it->second << std::endl;
    }

    std::cout << "Units:" << std::endl;
    for (auto it = units.begin(); it != units.end(); it++)
    {
        std::cout << it->get_name() << "(ID " << it->get_id() << ")" << std::endl;
    }

    std::cout << "Buying items:" << std::endl;
    for (auto it = shop_catalogue.begin(); it != shop_catalogue.end(); it++)
    {
        auto item = it->first;
        if (item != pistol)
        {
            for (size_t i = 0; i < shop.get_units().size(); i++)
            {
                std::string success = shop.buy_item(item) ? "Succeeded" : "Not enough money";
                std::cout << "Trying to buy " << item->get_name() << ": " << success << std::endl;
            }
        }
    }

    std::cout << "Assigning items to team:" << std::endl;
    for (auto it = units.begin(); it != units.end(); it++)
    {
        std::string message = "Trying to assign to " + it->get_name() + "(ID " + std::to_string(it->get_id()) + "):\n";
        for (auto jt = shop_catalogue.begin(); jt != shop_catalogue.end(); jt++)
        {
            if (shop.assign_to_unit(jt->first, &*it))
            {
                message += "Gave " + jt->first->get_name() + "\n";
            } else
            {
                message += "Could not give " + jt->first->get_name() + ": not enough items owned\n";
            }
        }

        std::cout << message << std::endl;
    }

    std::cout << "Testing retrieving items:" << std::endl;
    Unit team_leader = units[0];
    std::string get_rifle = (shop.retrieve_from_unit(rifle, &team_leader)) ? "Acquired Rifle" : "Unable to retrieve Rifle";
    std::string get_pistol = (shop.retrieve_from_unit(pistol, &team_leader)) ? "Acquired Pistol" : "Unable to retrieve Pistol";
    std::cout << get_rifle << ", " << get_pistol << std::endl;

    std::cout << "Testing refunding items" << std::endl;
    std::string refund_rifle = shop.refund_item(rifle) ? "Refunded Rifle" : "Can't refund Rifle";
    std::string refund_pistol = shop.refund_item(pistol) ? "Refunded Pistol" : "Can't refund Pistol";
    std::cout << refund_rifle << ", " << refund_pistol << std::endl;

    std::cout << "Testing team formation:" << std::endl;
    Team team = shop.form_team();
    std::cout << "Formed team with ID " << team.get_id() << ", units: " << std::endl;
    for (auto it = team.get_units().begin(); it != team.get_units().end(); it++)
    {
        std::cout << it->get_name() << "(ID " << it->get_id() << ")" << std::endl;
    }

    return 0;
}
