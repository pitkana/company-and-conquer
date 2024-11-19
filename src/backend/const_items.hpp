#pragma once

#include <memory>

#include "building_part_type.hpp"
#include "item.hpp"

namespace ConstItem {
const std::shared_ptr<const BuildingPart> turret_legs = std::make_shared<const BuildingPart>(BuildingPartType::TurretLegs);
const std::shared_ptr<const BuildingPart> turret_barrel = std::make_shared<const BuildingPart>(BuildingPartType::TurretBarrel);

const std::shared_ptr<const BuildingPart> medic_tent_tent = std::make_shared<const BuildingPart>(BuildingPartType::MedicTentTent);
const std::shared_ptr<const BuildingPart> medic_tent_medkit = std::make_shared<const BuildingPart>(BuildingPartType::MedicTentMedkit);
}
