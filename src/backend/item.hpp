#pragma once

#include <string>
#include <variant>
#include <sstream>
#include <memory>

#include "coordinates.hpp"
#include "building_part_type.hpp"

class Action; //forward declaration
class Building; //forward declaration

//Item base class
class Item {
public:
    //Initialize item with a name
    Item(std::string name): name_(std::move(name)) {}
    Item(std::string name, std::string desc): name_(std::move(name)), description_(std::move(desc)) {}

    virtual ~Item() = default;

    // Return the action associated with this item on the coordinates given as parameter
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(coordinates<size_t> target) const = 0;

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
    Weapon(std::string name, int accuracy, int damage, int falloff, int area_of_effect = 0):
           Item(std::move(name)), accuracy_(accuracy), damage_(damage), falloff_(falloff), area_of_effect_(area_of_effect) 
    {
        //Initialize description based on parameters
        std::stringstream desc;
        desc << name << ", accuracy: " << accuracy << ", damage: " << damage << ", area of effect: " << area_of_effect;
        description_ = desc.str();
    }

    //Returns an WeaponAction for the damaging action
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(coordinates<size_t> target) const;

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

class HealingItem : public Item {
public:
    HealingItem(std::string name, int heal_amount, int area_of_effect = 0):
        Item(std::move(name)), heal_amount_(heal_amount), area_of_effect_(area_of_effect)
    {
        std::stringstream desc;
        desc << name << ", heal amount: " << heal_amount << ", area of effect: " << area_of_effect;
        description_ = desc.str();
    }

    //Returns an HealAction for the healing action
    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(coordinates<size_t> target) const;

    [[nodiscard]]
    int get_heal_amount() const {
        return heal_amount_;
    }

    [[nodiscard]]
    int get_aoe() const {
        return area_of_effect_;
    }

private:
    const int heal_amount_;
    const int area_of_effect_;
};


class BuildingPart : public Item {
public:
    BuildingPart(BuildingPartType part_type): Item(name_from_type(part_type), desc_from_type(part_type)), part_type_(part_type) {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> get_action(coordinates<size_t> target) const;

    [[nodiscard]]
    BuildingPartType get_part_type() const;

    /**
     * @brief Constructs and returns a shared_ptr to a building that this part builds, also initializes it with this part added to it
     *
     * @return std::shared_ptr<Building>
     */
    [[nodiscard]]
    std::shared_ptr<Building> get_building() const;

private:
    [[nodiscard]]
    std::string name_from_type(BuildingPartType part_type) const {
        switch (part_type) {
            case BuildingPartType::TurretLegs: return "turret legs";
            case BuildingPartType::TurretBarrel: return "turret barrel";

            case BuildingPartType::MedicTentMedkit: return "medkit";
            case BuildingPartType::MedicTentTent: return "tent";
        }
        //Unreachable case since all enum class cases are handled
        __builtin_unreachable();
    }

    [[nodiscard]]
    std::string desc_from_type(BuildingPartType part_type) const {
        switch (part_type) {
            case BuildingPartType::TurretLegs:
            case BuildingPartType::TurretBarrel: return "used for building a gun turret";

            case BuildingPartType::MedicTentMedkit:
            case BuildingPartType::MedicTentTent: return "used for building a medic tent";
        }
        //Unreachable case since all enum class cases are handled
        __builtin_unreachable();
    }

private:
    const BuildingPartType part_type_;
};
