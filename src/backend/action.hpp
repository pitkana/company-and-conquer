#pragma once

#include "helper_tools.hpp"
#include <memory>

namespace Action {
class CharacterAction {
public:
    CharacterAction(float acc, float hp_eff, const helper::coordinates<size_t>& target):
        accuracy_(acc), hp_effect_(hp_eff), target_(target) {}

public:
    float accuracy_;
    float hp_effect_;
    helper::coordinates<size_t> target_;
};

class BuildingAction {
public:
    BuildingAction(const std::string& building, const helper::coordinates<size_t>& target):
        building_type_(building), target_(target) {}

public:
    const std::string& building_type_;
    helper::coordinates<size_t> target_;
};
}
