#pragma once

#include "Component.hpp"
#include <vector>

class EffectPoisonComponent : public Component {
public:
    explicit EffectPoisonComponent(const uint32_t id);
    ~EffectPoisonComponent() override;

    float	poisonTime				{1};
    float	poisonMaxTime			{1};
    float	damageTimes				{5};
    float	damage 					{3};
};
