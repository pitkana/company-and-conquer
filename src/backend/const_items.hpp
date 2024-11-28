#pragma once

#include <memory>

#include "building_part_type.hpp"
#include "item.hpp"

namespace ConstItem {
const std::shared_ptr<const BuildingPart> turret_legs = std::make_shared<const BuildingPart>(BuildingPartType::TurretLegs);
const std::shared_ptr<const BuildingPart> turret_barrel = std::make_shared<const BuildingPart>(BuildingPartType::TurretBarrel);

const std::shared_ptr<const BuildingPart> medic_tent_tent = std::make_shared<const BuildingPart>(BuildingPartType::MedicTentTent);
const std::shared_ptr<const BuildingPart> medic_tent_medkit = std::make_shared<const BuildingPart>(BuildingPartType::MedicTentMedkit);

// Items that are used when using a building
const std::shared_ptr<const Weapon> turret_weapon = std::make_shared<const Weapon>("turret", 80, 30, 10, 0);
const std::shared_ptr<const HealingItem> medic_tent_heal_item = std::make_shared<const HealingItem>("medic tent", 50, 0);

const std::shared_ptr<const Weapon> grenade = std::make_shared<const Weapon>("grenade", 100, 50, 0, 3);
}
