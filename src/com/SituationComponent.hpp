#pragma once

#include "Component.hpp"

class SituationComponent : public Component {
public:
    explicit SituationComponent(const uint32_t id);
    ~SituationComponent() override;

    float	x			{0.f};
    float	y			{0.f};
    float	z			{0.f};
    float	rotation	{0.f};
};
