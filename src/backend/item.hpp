#pragma once

#include <string>
#include <variant>
#include <sstream>
#include <memory>

#include "action.hpp"
#include "coordinates.hpp"

//Item base class
class Item {
public:
    //Initialize item with a name
    Item(const std::string& name): name_(name) {}

    virtual ~Item() = default;

    // Return the action associated with this item on the coordinates given as parameter
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const = 0;

    // Return name of this item
    [[nodiscard]]
    inline const std::string& get_name() const;

    [[nodiscard]]
    inline const std::string& get_description() const;

protected:
    std::string name_;
    std::string description_;
};

class Weapon : public Item {
public:
    Weapon(const std::string& name, int accuracy, int damage, int area_of_effect = 0):
           Item(name), accuracy_(accuracy), damage_(damage), area_of_effect_(area_of_effect) 
    {
        //Initialize description based on parameters
        std::stringstream desc;
        desc << name << ", accuracy: " << accuracy << ", damage: " << damage << ", area of effect: " << area_of_effect;
        description_ = desc.str();
    }

    //Returns an ItemAction for the damaging action
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const;

private:
    const int accuracy_;
    const int damage_;
    //Area of effect means how many squares out will this item affect from the target, 0 means only the target square.
    const int area_of_effect_;
};

class Consumable : public Item {
public:
    Consumable(const std::string& name, int heal_amount, int area_of_effect):
        Item(name), heal_amount_(heal_amount), area_of_effect_(area_of_effect)
    {
        std::stringstream desc;
        desc << name << ", heal amount: " << heal_amount << ", area of effect: " << area_of_effect;
        description_ = desc.str();
    }

    //Returns an ItemAction for the healing action
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const;

private:
    const int heal_amount_;
    const int area_of_effect_;
};

//TODO:
// Add BuildingPart item when it's known how building Buildings works
