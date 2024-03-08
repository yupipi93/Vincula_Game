#pragma once

#include "Component.hpp"
#include <vector>

class EffectMagnifyBulletComponent : public Component {
public:
    explicit EffectMagnifyBulletComponent(const uint32_t id);
    ~EffectMagnifyBulletComponent() override;

    float	magnifyBulletTime	{1};
    float	MAGNIFY_ADD_SCALE	{3};
    float	MAGNIFY_ADD_ROTATE	{500};
};
