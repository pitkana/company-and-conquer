#pragma once

#include <variant>
#include <memory>

#include "coordinates.hpp"
#include "action.hpp"
#include "unit.hpp"

/*
* @brief Contains data on what is done during a single unit's turn
*/
struct Turn {
    Turn(const Unit& unit, const coordinates<size_t>& unit_origin, 
         const coordinates<size_t>& movement_destination, std::shared_ptr<Action> action):
         unit(unit), unit_origin(unit_origin), movement_destination(movement_destination), action(action) {}

    const Unit& unit;
    const coordinates<size_t>& unit_origin;
    const coordinates<size_t>& movement_destination;
    std::shared_ptr<Action> action;
};
