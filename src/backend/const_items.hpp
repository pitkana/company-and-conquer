#pragma once

#include <memory>
#include <unordered_map>

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

const std::shared_ptr<const Weapon> rifle = std::make_shared<const Weapon>("Rifle", 95, 60, 5);
const std::shared_ptr<const Weapon> smg = std::make_shared<const Weapon>("Machine Pistol", 80, 75, 15);
const std::shared_ptr<const Weapon> shotgun = std::make_shared<const Weapon>("Shotgun", 70, 90, 30);
const std::shared_ptr<const Weapon> grenade_launcher = std::make_shared<const Weapon>("Grenade Launcher", 60, 70, 40, 2);

const std::shared_ptr<const HealingItem> bandage = std::make_shared<const HealingItem>("Bandage", 40);
const std::shared_ptr<const HealingItem> healing_kit = std::make_shared<const HealingItem>("First Aid Kit", 60, 1);

inline std::unordered_map<std::string, const std::shared_ptr<const Item>> item_ids = {
    {"turret_legs", turret_legs},
    {"turret_barrel", turret_barrel},

    {"medic_tent_tent", medic_tent_tent},
    {"medic_tent_medkit", medic_tent_medkit},


    {"rifle", rifle},
    {"smg", smg},
    {"shotgun", shotgun},
    {"grenade_launcher", grenade_launcher},

    {"bandage", bandage},
    {"healing_kit", healing_kit}
};

}
