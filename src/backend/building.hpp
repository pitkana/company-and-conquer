#pragma once

#include <memory>

#include "coordinates.hpp"
#include "action.hpp"

class Building {
public:
    Building(const std::string& name): name_(name) {}
    virtual ~Building() = default;

    virtual std::shared_ptr<Action> use_building(const coordinates<size_t>& target) const = 0;
    virtual bool add_part(const BuildingPart& part) = 0;
    virtual bool remove_part(const BuildingPart& part) = 0;

    [[nodiscard]]
    virtual bool has_part(const BuildingPart& part) const = 0;

    [[nodiscard]]
    virtual bool is_ready() const = 0;

protected:
    std::string name_;
};


class Turret : public Building {
public:
    Turret(const std::string& name): Building(name) {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> use_building(const coordinates<size_t>& target) const;
    virtual bool add_part(const BuildingPart& part);
    virtual bool remove_part(const BuildingPart& part);

    [[nodiscard]]
    virtual bool has_part(const BuildingPart& part) const;
    [[nodiscard]]
    virtual bool is_ready() const;

    [[nodiscard]]
    bool has_legs() const;
    [[nodiscard]]
    bool has_barrel() const;
private:
    int damage_ = 50;
    int accuracy_ = 80;
    int area_of_effect_ = 0;

    bool has_legs_ = false;
    bool has_barrel_ = false;
};

class MedicTent : public Building {
public:
    MedicTent(const std::string& name): Building(name) {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> use_building(const coordinates<size_t>& target) const;
    virtual bool add_part(const BuildingPart& part);
    virtual bool remove_part(const BuildingPart& part);

    [[nodiscard]]
    virtual bool is_ready() const;
    [[nodiscard]]
    bool has_medkit() const;
    [[nodiscard]]
    bool has_tent() const;

private:
    bool set_part(const BuildingPart& part, bool value);

private:
    int heal_amount_ = 70;
    int area_of_effect_ = 0;

    bool has_medkit_ = false;
    bool has_tent_ = false;
};
