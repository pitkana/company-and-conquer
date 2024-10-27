#pragma once

#include "helper_tools.hpp"
#include <memory>
#include <string_view>

namespace Action {
class CharacterAction {
    public:
        CharacterAction(float acc, float hp_eff, const helper::coordinates<size_t>& target):
            accuracy_(acc), hp_effect_(hp_eff), target_(target) {}

        const float accuracy() const
        {
            return accuracy_;
        }

        const float hp_effect() const
        {
            return hp_effect_;
        }

        const helper::coordinates<size_t>& target() const
        {
            return target_;
        }

    private:
        float accuracy_;
        float hp_effect_;
        helper::coordinates<size_t> target_;
    };

class BuildingAction {
    public:
        BuildingAction(const std::string& building, const helper::coordinates<size_t>& target):
            building_type_(building), target_(target) {}
        
        // use string_view for efficient non-owning copy
        const std::string_view building_type() const
        {
            return building_type_;
        }

        const helper::coordinates<size_t>& target() const
        {
            return target_;
        }

    private:
        const std::string building_type_;
        helper::coordinates<size_t> target_;
};
}
