#include <memory>
#include <map>
#include <vector>

#include "unit.hpp"
#include "item.hpp"
#include "team.hpp"
#include "shop.hpp"
#include "name_gen.hpp"

Shop::Shop(std::map<std::shared_ptr<Item>, int> catalogue, int team_size, int budget,
    std::vector<std::shared_ptr<Item>> stockpile) : catalogue_(catalogue), budget_(budget), items_owned_(stockpile) {
    NameGen name_gen = NameGen();
    for (int i = 0; i < team_size; i++) {
         units_.push_back(Unit(name_gen.generate(name_lists.english, 2, 4)));
    }
}

bool Shop::buy_item(std::shared_ptr<Item> item) {
    auto find_price = catalogue_.find(item);
    if (find_price == catalogue_.end())
    {
        return false;
    } else if (find_price->second > budget_)
    {
        return false;
    } else
    {
        budget_ -= find_price->second;
        items_owned_.push_back(item);
        purchased_items_.push_back(item);
        return true;
    }
}

bool Shop::refund_item(std::shared_ptr<Item> item)
{
    for (auto it_owned = items_owned_.begin(); it_owned != items_owned_.end(); it_owned++)
    {
        if (*it_owned == item)
        {
            for (auto it_purchased = purchased_items_.begin(); it_purchased != purchased_items_.end(); it_purchased++)
            {
                if (item == *it_purchased)
                {
                    auto find_price = catalogue_.find(item);
                    budget_ += find_price->second;
                    purchased_items_.erase(it_purchased);
                    items_owned_.erase(it_owned);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

bool Shop::assign_to_unit(std::shared_ptr<Item> item, Unit* unit)
{
    for (auto it = items_owned_.begin(); it != items_owned_.end(); it++)
    {
        if ((*it) == item)
        {
            if (unit->add_item(item))
            {
                items_owned_.erase(it);
                return true;
            }
            return false;
        }
    }
    return false;
}

bool Shop::retrieve_from_unit(std::shared_ptr<Item> item, Unit* unit)
{
    if (unit->remove_item(item))
    {
        items_owned_.push_back(item);
        return true;
    }
    return false;
}

Team Shop::form_team() const
{
    Team team;
    for (auto it = units_.begin(); it != units_.end(); it++)
    {
        team.add_unit(*it);
    }
    return team;
}

