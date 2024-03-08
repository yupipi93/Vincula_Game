#pragma once

#include "Component.hpp"
#include <enum/EntityType.hpp>
#include <enum/EffectType.hpp>
#include <enum/AttackType.hpp>
#include <vector>
#include <chrono>
#include <ctime>

class AttackComponent : public Component {
public:
    explicit AttackComponent(const uint32_t id);
    ~AttackComponent() override;

    uint32_t              	attackerId              {0};
    EffectType              effectType              {NOEFFECT};

    bool                    isCritic                {false};
    uint32_t                damage                  {0};
    uint8_t                 biome                   {0};

    float                   lifeTime                {1};

    std::vector<uint32_t>   entitiesAlreadyDamaged  {};

    AttackType 				attackType              {ATTACKMELEE};

    bool 					rotative 				{false};
};