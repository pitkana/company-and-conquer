#pragma once

#include <string_view>

#include "coordinates.hpp"
// #include "game.hpp"

#define ACTION_TYPES Action::CharacterAction, Action::BuildingAction

namespace Action {
class CharacterAction {
    public:
        CharacterAction(float acc, int hp_eff, int area_of_effect, const coordinates<size_t>& target):
            accuracy_(acc), hp_effect_(hp_eff), area_of_effect_(area_of_effect), target_(target) {}

        float accuracy() const
        {
            return accuracy_;
        }

        float hp_effect() const
        {
            return hp_effect_;
        }

        const coordinates<size_t>& target() const
        {
            return target_;
        }

    private:
        const float accuracy_;
        const int hp_effect_;
        const int area_of_effect_;
        const coordinates<size_t> target_;
    };

class BuildingAction {
    public:
        // using string_view to avoid situations where we might construct a std::string when passing the parameter values
        // also allows us to pass also other types not just std::string
        BuildingAction(std::string_view building, const coordinates<size_t>& target):
            building_type_(building), target_(target) {}
        

        const std::string& building_type() const
        {
            return building_type_;
        }

        const coordinates<size_t>& target() const
        {
            return target_;
        }

    private:
        const std::string building_type_;
        coordinates<size_t> target_;
};
}
