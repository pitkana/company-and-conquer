#ifndef unit
#define unit

#include <vector>
#include <string>
#include <cstdint>

struct {
    uint8_t max_hp = 10;
    uint8_t move_range = 5;
    uint8_t visual_range = 5;
    uint8_t inventory_size = 4;
} unit_consts;


class Unit
{

public:
    Unit(std::string &name) :
        name_(name), inventory_({}), current_hp_(unit_consts.max_hp) {}

    const std::string GetName() const {
        return name_;
    }

    const std::vector<uint8_t> GetInventory() const {
        return inventory_;
    }

    const uint8_t GetHP() const {
        return current_hp_;
    }


private:
    std::string &name_;

    std::vector<uint8_t> inventory_;
    uint8_t current_hp_;

};

#endif
