#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <memory>

#include "item.hpp"

/** Constant values used for initializing Unit instances.
 */

struct {
    int max_hp = 100;
    unsigned int move_range = 5;
    unsigned int visual_range = 5;
    unsigned int inventory_size = 4;
} unit_consts;


/** Unit class. This class represents playable units in the game.
 *  Each unit has an HP value, an inventory consisting of up to unit_consts.inventory_size Item pointers, and a unique ID as well as a name.
 */
class Unit
{

public:
    Unit(const std::string &name) :
        name_(name){
        // inventory_.reserve(unit_consts.inventory_size);
        current_hp_ = unit_consts.max_hp;
        id_ = count_++;
    }

    const std::string& get_name() const {
        return name_;
    }

    int get_id() const {
        return id_;
    }

    const std::vector<std::shared_ptr<const Item>>& get_inventory() const {
        return inventory_;
    }

    bool add_item(std::shared_ptr<const Item> item);

    bool remove_item(std::shared_ptr<const Item> item);

    int get_hp() const {
       return current_hp_;
    }

    /**
     * @brief Changes this units hp by the amount specified
     *
     * @param amount the amount of hp to change by, positive to heal, negative to deal damage
     * @return int, the amount of hp that was changed (not necessarily same as amount, since hp cant go over max)
     */
    int change_hp_by(int amount);

    bool deal_damage(const Weapon& weapon, unsigned int distance_from);

    /**
     * @brief Heals this unit by the amount specified by parameter item
     *
     * @param heal_item
     * @return int, how much was actually healed
     */
    int heal(const HealingItem& heal_item);

    bool is_dead() const {
        return current_hp_ <= 0;
    }


private:
    std::string name_;

    std::vector<std::shared_ptr<const Item>> inventory_;
    int current_hp_;

    unsigned int id_;

    static inline unsigned int count_ = 0;
};
