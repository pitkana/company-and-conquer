#pragma once

#include <memory>
#include <utility>

#include "coordinates.hpp"
#include "building_part_type.hpp"
#include "item.hpp"

class Action; //forward declaration

class Building {
public:
    Building(const std::string& name): name_(name) {}
    virtual ~Building() = default;

    //Return action associated with using this specific building
    virtual std::shared_ptr<Action> use_building(coordinates<size_t> target) const = 0;


    //Check if this specific part is already added to this building
    [[nodiscard]]
    virtual bool has_part(const BuildingPart& part) const;
    // Returns false if couldn't add/remove the part, true if could
    virtual bool add_part(const BuildingPart& part);
    virtual bool remove_part(const BuildingPart& part);

    // Check if this building is fully built
    [[nodiscard]]
    virtual bool is_ready() const = 0;

    /**
     * @brief Returns true if it has no parts added to it (and therefore shouldn't exist). Possible by adding a part and undoing it.
     *
     * @return bool
     */
    [[nodiscard]]
    virtual bool has_no_parts() const = 0;

private:
    // These pure virtual functions should be implemented in derived classes, as they are used within has_part, add_part and remove_part
    // They return a non-const/const pointer to the bool value associated with a specific part (or nullptr if that part is not part of the building)
    // Used for modifying (or reading in has_part's case) the value of that part
    inline virtual bool* get_part_bool_ptr(const BuildingPart& part) = 0;
    inline virtual const bool* get_part_bool_ptr(const BuildingPart& part) const = 0;

protected:
    std::string name_;

};


class Turret : public Building {
public:
    Turret(): Building("turret") {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> use_building(coordinates<size_t> target) const;

    [[nodiscard]]
    virtual bool is_ready() const;
    [[nodiscard]]
    virtual bool has_no_parts() const;

    [[nodiscard]]
    bool has_legs() const;
    [[nodiscard]]
    bool has_barrel() const;

private:
    //Helper function that returns pointer to the specific part's bool variable
    //Makes a lot of functions a lot simpler and easier to change in future
    inline virtual bool* get_part_bool_ptr(const BuildingPart& part) {
        switch (part.get_part_type()) {
            case BuildingPartType::TurretLegs:
                return &has_legs_;
            case BuildingPartType::TurretBarrel:
                return &has_barrel_;
            default:
                return nullptr;
        }
    }

    inline virtual const bool* get_part_bool_ptr(const BuildingPart& part) const {
        // const_cast to be able to use get_part_bool_ptr non const version to return const pointer. Safe since get_part_bool_ptr does not modify
        // the class, just returns a pointer which get turned into a const pointer by this function.
        return const_cast<Turret*>(this)->get_part_bool_ptr(part);
    }

private:
    int damage_ = 50;
    int accuracy_ = 80;
    int area_of_effect_ = 0;

    bool has_legs_ = false;
    bool has_barrel_ = false;
};

class MedicTent : public Building {
public:
    MedicTent(): Building("medic tent") {}

    [[nodiscard]]
    virtual std::shared_ptr<Action> use_building(coordinates<size_t> target) const;

    [[nodiscard]]
    virtual bool is_ready() const;
    [[nodiscard]]
    virtual bool has_no_parts() const;

    [[nodiscard]]
    bool has_medkit() const;
    [[nodiscard]]
    bool has_tent() const;

private:
    //Helper function that returns pointer to the specific part's bool variable
    //Makes a lot of functions a lot simpler and easier to change in future
    inline virtual bool* get_part_bool_ptr(const BuildingPart& part) {
        switch (part.get_part_type()) {
            case BuildingPartType::MedicTentTent:
                return &has_tent_;
            case BuildingPartType::MedicTentMedkit:
                return &has_medkit_;
            default:
                return nullptr;
        }
    }

    inline virtual const bool* get_part_bool_ptr(const BuildingPart& part) const {
        // const_cast to be able to use get_part_bool_ptr non const version to return const pointer. Safe since get_part_bool_ptr does not modify
        // the class, just returns a pointer which get turned into a const pointer by this function.
        return const_cast<MedicTent*>(this)->get_part_bool_ptr(part);
    }

private:
    int heal_amount_ = 70;
    int area_of_effect_ = 0;

    bool has_medkit_ = false;
    bool has_tent_ = false;
};
