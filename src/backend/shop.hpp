#pragma once

#include <memory>
#include <map>
#include <vector>

#include "unit.hpp"
#include "item.hpp"
#include "team.hpp"

class Shop {

public:
    Shop(std::map<std::shared_ptr<Item>, int> catalogue, int team_size, int budget, std::vector<std::shared_ptr<Item>> stockpile = std::vector<std::shared_ptr<Item>>());

    [[nodiscard]]
    const std::map<std::shared_ptr<Item>, int>& get_catalogue() const
    {
        return catalogue_;
    }

    [[nodiscard]]
    const std::vector<std::shared_ptr<Item>>& get_owned() const
    {
        return items_owned_;
    }

    [[nodiscard]]
    const std::vector<std::shared_ptr<Item>>& get_purchases() const
    {
        return purchased_items_;
    }

    [[nodiscard]]
    const std::vector<Unit>& get_units() const
    {
        return units_;
    }

    [[nodiscard]]
    int get_budget() const
    {
        return budget_;
    }

    bool buy_item(std::shared_ptr<Item> item);

    bool refund_item(std::shared_ptr<Item> item);

    bool assign_to_unit(std::shared_ptr<Item> item, Unit *unit);

    bool retrieve_from_unit(std::shared_ptr<Item> item, Unit *unit);

    [[nodiscard]]
    Team form_team() const;

private:
    std::map<std::shared_ptr<Item>, int> catalogue_;
    std::vector<Unit> units_;
    std::vector<std::shared_ptr<Item>> items_owned_;
    std::vector<std::shared_ptr<Item>> purchased_items_;

    int budget_;
};
