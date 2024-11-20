#include <random>
#include <cmath>
#include <memory>
#include <iostream>

#include "unit.hpp"
#include "item.hpp"

std::default_random_engine generator;
std::uniform_int_distribution<int> hit_chance(0,100);

bool Unit::add_item(std::shared_ptr<const Item> item) {
    if (inventory_.size() < unit_consts.inventory_size) {
        inventory_.push_back(item);
        return true;
    }
    return false;
}

bool Unit::remove_item(std::shared_ptr<const Item> item)
{
    for (auto it = inventory_.begin(); it != inventory_.end(); ++it)
    {
        if (*it == item)
        {
            inventory_.erase(it);
            return true;
        }
    }
    return false;
}

bool Unit::deal_damage(std::shared_ptr<const Weapon> weapon, unsigned int distance_from) {
    if (hit_chance(generator) > weapon->get_accuracy()) {
        return false;
    } else {
        double falloff_rate = (100.0 - (double)weapon->get_falloff())/100.0;
        double falloff_effect = std::pow(falloff_rate, (double)distance_from);
        int final_dmg = (int)round(weapon->get_damage() * falloff_effect);
        current_hp_ -= final_dmg;
        return true;
    }
}
