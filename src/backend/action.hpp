#pragma once

#include <vector>
#include <utility>

#include "item.hpp"
#include "coordinates.hpp"
#include "building_part_type.hpp"

class Game; //forward declaration
class BuildingPart; //forward declaration
class Item;
class Unit;

class Action {
public:
    Action(coordinates<size_t> target, Unit& executing_unit):
        target_(std::move(target)), executing_unit_(executing_unit) {}

    virtual ~Action() = default;

    [[nodiscard]]
    virtual const coordinates<size_t>& target() const;

    /**
     * @brief executes the action, affecting the game given as parameter
     *
     * @param game reference to game in which this action will be executed
     * @return void
     */
    virtual void execute(Game& game, coordinates<size_t> unit_location) = 0;

    /**
     * @brief Undoes the effects of the action on the game given as parameter
     *
     * @param game Where the actions of this effect are undone
     * @return void
     */
    virtual void undo(Game& game) = 0;

    /**
     * @brief Returns bool value depending on if the action's result depends on RNG. Only non-random actions will be executed before turn is over
     *
     * @return bool
     */
    [[nodiscard]]
    virtual bool contains_randomness() const = 0;

    [[nodiscard]]
    virtual bool is_movement() const = 0;

    [[nodiscard]]
    Unit& get_unit() { return executing_unit_; }

protected:
    const coordinates<size_t> target_;
    Unit& executing_unit_;
    bool has_been_executed_ = false;
};

class MovementAction : public Action {
public:
    MovementAction(coordinates<size_t> source_location, coordinates<size_t> target_location, Unit& executing_unit):
        Action(std::move(target_location), executing_unit), source_location_(source_location) {}

    virtual void execute(Game& game, coordinates<size_t> unit_location [[maybe_unused]]);

    virtual void undo(Game& game);

    [[nodiscard]]
    virtual bool contains_randomness() const {return false;}

    [[nodiscard]]
    virtual bool is_movement() const {return true;}

private:
    coordinates<size_t> source_location_;
};

class WeaponAction : public Action {
public:
    WeaponAction(const Weapon& weapon, coordinates<size_t> target, Unit& executing_unit):
        Action(std::move(target), executing_unit), weapon_(weapon) {}

    virtual void execute(Game& game, coordinates<size_t> unit_location);

    // WeaponActions are only executed at the end of a turn and cannot be undone due to their randomness
    virtual void undo(Game& game [[maybe_unused]]) {return;}

    [[nodiscard]]
    virtual bool contains_randomness() const {return true;}

    [[nodiscard]]
    virtual bool is_movement() const {return false;}
private:
    // Coordinates of the unit who does this action at the point of execution
    const Weapon& weapon_;
};

class HealingAction : public Action {
public:
    HealingAction(const HealingItem& healing_item, coordinates<size_t> target, Unit& executing_unit) :
        Action(std::move(target), executing_unit), healing_item_(healing_item) {}

    [[nodiscard]]
    int heal_amount() const;

    [[nodiscard]]
    int area_of_effect() const;

    virtual void execute(Game& game, coordinates<size_t> unit_location);
    virtual void undo(Game& game);

    [[nodiscard]]
    virtual bool contains_randomness() const {return false;}

    [[nodiscard]]
    virtual bool is_movement() const {return false;}
private:
    const HealingItem& healing_item_;

    // Store the amount of health this action has healed per unit healed so the right amount can be undone when necessary
    // Vector since an AoE healing item can heal multiple units
    std::vector<std::pair<Unit*, int>> healed_amounts_;
};

class BuildingAction : public Action {
public:
    BuildingAction(const BuildingPart& part, coordinates<size_t> target, Unit& executing_unit):
        Action(std::move(target), executing_unit), building_part_(part) {}

    [[nodiscard]]
    BuildingPartType get_part_type() const;

    [[nodiscard]]
    const BuildingPart& get_part() const;

    virtual void execute(Game& game, coordinates<size_t> unit_location);
    virtual void undo(Game& game);

    [[nodiscard]]
    virtual bool contains_randomness() const {return false;}

    [[nodiscard]]
    virtual bool is_movement() const {return false;}
private:
    const BuildingPart& building_part_;
};
