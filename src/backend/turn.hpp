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
    Turn(const Unit& unit, coordinates<size_t> unit_origin, 
         coordinates<size_t> movement_destination, std::shared_ptr<Action> action):
         unit(unit), unit_origin(std::move(unit_origin)), movement_destination(std::move(movement_destination)), action(action) {}

    const Unit& unit;
    const coordinates<size_t> unit_origin;
    const coordinates<size_t> movement_destination;
    std::shared_ptr<Action> action;
};
