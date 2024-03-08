#pragma once

#include "Component.hpp"
#include <vector>

class EffectSlowdownComponent : public Component {
public:
    explicit EffectSlowdownComponent(const uint32_t id);
    ~EffectSlowdownComponent() override;

    float	slowdownTime	{3};
    float	SLOWDOWN_FACTOR	{0.5};
};
