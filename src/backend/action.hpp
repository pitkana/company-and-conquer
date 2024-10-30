#pragma once

#include <memory>
#include <string_view>

#include "coordinates.hpp"

namespace Action {
class CharacterAction {
    public:
        CharacterAction(float acc, float hp_eff, const coordinates<size_t>& target):
            accuracy_(acc), hp_effect_(hp_eff), target_(target) {}

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
        float accuracy_;
        float hp_effect_;
        coordinates<size_t> target_;
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
