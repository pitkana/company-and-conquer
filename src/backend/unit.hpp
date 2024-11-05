#ifndef unit
#define unit

#include <vector>
#include <string>
#include <cstdint>

struct {
    int max_hp = 10;
    unsigned int move_range = 5;
    unsigned int visual_range = 5;
    unsigned int inventory_size = 4;
} unit_consts;


class Unit
{

public:
    Unit(std::string &name) :
        name_(name) {
        inventory_ = {};
        current_hp_ = unit_consts.max_hp;
    }

    const std::string& GetName() const {
        return name_;
    }

    const std::vector<uint8_t>& GetInventory() const {
        return inventory_;
    }

    int GetHP() const {
        return current_hp_;
    }


private:
    std::string &name_;

    std::vector<uint8_t> inventory_;
    int current_hp_;

};

#endif 
