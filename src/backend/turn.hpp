#pragma once

#include <variant>

#include "coordinates.hpp"
#include "action.hpp"
#include "unit.hpp"

/*
* @brief Contains data on what is done during a single unit's turn
*/
struct Turn {
    Turn(const Unit& actor, const coordinates<size_t>& actor_origin, 
         const coordinates<size_t>& movement_destination, const std::variant<ACTION_TYPES>& action):
         actor(actor), actor_origin(actor_origin), movement_destination(movement_destination), action(action) {}

    const Unit& actor;
    const coordinates<size_t>& actor_origin;
    const coordinates<size_t>& movement_destination;
    const std::variant<ACTION_TYPES>& action;
};
