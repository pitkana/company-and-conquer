#pragma once

#include <string>
#include <variant>
#include <sstream>
#include <memory>

#include "coordinates.hpp"
#include "building_part_type.hpp"

class Action; //forward declaration

//Item base class
class Item {
public:
    //Initialize item with a name
    Item(const std::string& name): name_(name) {}
    Item(const std::string& name, const std::string& desc): name_(name), description_(desc) {}

    virtual ~Item() = default;

    // Return the action associated with this item on the coordinates given as parameter
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const = 0;

    // Return name of this item
    [[nodiscard]]
    const std::string& get_name() const;

    [[nodiscard]]
    const std::string& get_description() const;

protected:
    std::string name_;
    std::string description_;
};

class Weapon : public Item {
public:
    Weapon(const std::string& name, int accuracy, int damage, int falloff, int area_of_effect = 0):
           Item(name), accuracy_(accuracy), damage_(damage), falloff_(falloff), area_of_effect_(area_of_effect) 
    {
        //Initialize description based on parameters
        std::stringstream desc;
        desc << name << ", accuracy: " << accuracy << ", damage: " << damage << ", area of effect: " << area_of_effect;
        description_ = desc.str();
    }

    //Returns an ItemAction for the damaging action
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const;

    int get_accuracy() const {
        return accuracy_;
    }

    int get_damage() const {
        return damage_;
    }

    int get_falloff() const {
        return falloff_;
    }

    int get_aoe() const {
        return area_of_effect_;
    }

private:
    const int accuracy_;
    const int damage_;
    const int falloff_;
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


class BuildingPart : public Item {
public:
    BuildingPart(BuildingPartType part_type): Item(name_from_type(), desc_from_type()), part_type_(part_type) {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(const coordinates<size_t>& target) const;

    [[nodiscard]]
    BuildingPartType get_part_type() const {
        return part_type_;
    }

private:
    [[nodiscard]]
    std::string name_from_type() const {
        switch (part_type_) {
            case BuildingPartType::TurretLegs: return "Turret legs";
            case BuildingPartType::TurretBarrel: return "Turret barrel";
            case BuildingPartType::MedicTentMedkit: return "Medkit";
            case BuildingPartType::MedicTentTent: return "Tent";
        }

        return "you messed up";
    }

    [[nodiscard]]
    std::string desc_from_type() const {
        switch (part_type_) {
            case BuildingPartType::TurretLegs: return "Used for building a gun turret";
            case BuildingPartType::TurretBarrel: return "Used for building a gun turret";
            case BuildingPartType::MedicTentMedkit: return "Used for building a medic tent";
            case BuildingPartType::MedicTentTent: return "Used for building a medic tent";
        }

        return "you messed up";
    }

private:
    const BuildingPartType part_type_;
};
