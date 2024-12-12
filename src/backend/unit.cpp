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

int Unit::change_hp_by(int amount) {
    // HP can't go past max_hp
    int new_hp = std::min(current_hp_ + amount, unit_consts.max_hp);
    int changed_hp = new_hp - current_hp_;
    current_hp_ = new_hp;

    return changed_hp;
}

bool Unit::deal_damage(float damage, int accuracy) {
    if (hit_chance(generator) > accuracy) {
        return false;
    }

    change_hp_by(-damage);
    return true;
}

int Unit::heal(const HealingItem &heal_item) {
    //Can't heal if dead
    if (is_dead()) return 0;

    return change_hp_by(heal_item.get_heal_amount());
}

bool Unit::has_weapon() const {
    for (const auto& item : inventory_) {
        if (item->is_weapon()) return true;
    }

    return false;
}

bool Unit::has_healing_item() const {
    for (const auto& item : inventory_) {
        if (item->is_healing_item()) return true;
    }

    return false;
}

bool Unit::has_building_part() const {
    for (const auto& item : inventory_) {
        if (item->is_building_part()) return true;
    }

    return false;
}

std::vector<std::shared_ptr<const Weapon>> Unit::get_weapons() const {
    std::vector<std::shared_ptr<const Weapon>> weapons;
    for (const std::shared_ptr<const Item>& item : inventory_) {
        if (!item->is_weapon()) continue;
        weapons.push_back(std::dynamic_pointer_cast<const Weapon>(item));
    }

    return weapons;
}

std::vector<std::shared_ptr<const HealingItem>> Unit::get_healing_items() const {
    std::vector<std::shared_ptr<const HealingItem>> healing_items;
    for (const std::shared_ptr<const Item>& item : inventory_) {
        if (!item->is_healing_item()) continue;
        healing_items.push_back(std::dynamic_pointer_cast<const HealingItem>(item));
    }

    return healing_items;

}

std::vector<std::shared_ptr<const BuildingPart>> Unit::get_building_parts() const {
    std::vector<std::shared_ptr<const BuildingPart>> building_parts;
    for (const std::shared_ptr<const Item>& item : inventory_) {
        if (!item->is_building_part()) continue;
        building_parts.push_back(std::dynamic_pointer_cast<const BuildingPart>(item));
    }

    return building_parts;

}
