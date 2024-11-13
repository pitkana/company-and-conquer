#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <memory>

#include "item.hpp"

struct {
    int max_hp = 10;
    unsigned int move_range = 5;
    unsigned int visual_range = 5;
    unsigned int inventory_size = 4;
} unit_consts;


class Unit
{

public:
    Unit(const std::string &name) :
        name_(name) {
        inventory_.reserve(unit_consts.inventory_size);
        current_hp_ = unit_consts.max_hp;
    }

    const std::string& GetName() const {
        return name_;
    }

    const std::vector<std::shared_ptr<Item>>& GetInventory() const {
        return inventory_;
    }

    void add_item(std::shared_ptr<Item> item) {
        if (inventory_.size() < unit_consts.inventory_size) {
            inventory_.push_back(item);
        }
    }

    int GetHP() const {
        return current_hp_;
    }


private:
    std::string name_;

    std::vector<std::shared_ptr<Item>> inventory_;
    int current_hp_;

};

