#pragma once

#include "coordinates.hpp"
#include "building_part_type.hpp"

class Game; //forward declaration
class BuildingPart; //forward declaration

class Action {
public:
    Action(coordinates<size_t> target):
        target_(std::move(target)){}

    virtual ~Action() = default;

    [[nodiscard]]
    virtual const coordinates<size_t>& target() const;

    virtual void execute(Game& game) const = 0;

protected:
    const coordinates<size_t> target_;
};

class WeaponAction : public Action {
public:
    WeaponAction(int acc, int hp_eff, int area_of_effect, coordinates<size_t> target):
        Action(std::move(target)), accuracy_(acc), hp_effect_(hp_eff), area_of_effect_(area_of_effect) {}

    [[nodiscard]]
    int accuracy() const;

    [[nodiscard]]
    int hp_effect() const;

    [[nodiscard]]
    int area_of_effect() const;

    virtual void execute(Game& game) const;

private:
    const int accuracy_;
    const int hp_effect_;
    const int area_of_effect_;
};

class HealingAction : public Action {
public:
    HealingAction(int heal_amount, int area_of_effect, coordinates<size_t> target) :
        Action(std::move(target)), heal_amount_(heal_amount), area_of_effect_(area_of_effect) {}

    [[nodiscard]]
    int heal_amount() const;

    [[nodiscard]]
    int area_of_effect() const;

    virtual void execute(Game& game) const;

private:
    const int heal_amount_;
    const int area_of_effect_;
};

class BuildingAction : public Action {
public:
    BuildingAction(const BuildingPart& part, coordinates<size_t> target):
        Action(std::move(target)), building_part_(part) {}

    [[nodiscard]]
    BuildingPartType get_part_type() const;

    [[nodiscard]]
    const BuildingPart& get_part() const;

    virtual void execute(Game& game) const;

private:
    const BuildingPart& building_part_;
};
