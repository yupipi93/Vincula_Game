#pragma once

#include "Component.hpp"
#include <vector>

class EffectParalysisComponent : public Component {
public:
    explicit EffectParalysisComponent(const uint32_t id);
    ~EffectParalysisComponent() override;

    float	paralysisTime	{1};
};
