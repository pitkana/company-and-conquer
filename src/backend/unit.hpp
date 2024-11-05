#ifndef unit
#define unit

#include <array>

struct unit_consts {
    uint_8t max_hp = 10;
    uint_8t move_range = 5;
    uint_8t visual_range = 5;
    uint_8t inventory_size = 4;
}


class Unit
{

public:
    Unit(std::string &name) :
        name_(name) {};

    const std::string GetName() const {
        return name_;
    }

    const std::array<uint_8t> GetInventory() const {
        return inventory_;
    }

    const uint_8t GetHP() const {
        return current_hp_;
    }


private:
    std::string &name_;

    std::array<uint_8t, unit_consts.inventory_size> inventory_;
    uint_8t current_hp_ = unit_consts.max_hp;


}

#endif
