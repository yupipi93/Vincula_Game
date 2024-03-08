#pragma once

#include "Component.hpp"

class VelocityComponent : public Component {
public:
    explicit VelocityComponent(const uint32_t id);
    ~VelocityComponent() override;

    void resetAllVelocities();

    float	velocityX	{0};
    float	velocityY	{0};
    float	velocityZ	{0};
    float	speed 	    {70};
    float	speedNormal	{70};
    float	speedUpped  {70};
};
