#pragma once

#include "Component.hpp"

class LevitationComponent : public Component {
public:
    explicit LevitationComponent(const uint32_t id);
    ~LevitationComponent() override;

    float levitationSpeed  {2.5};
    float signVelocity     {0};
    float levitationMargin {3};
  	float initialHeight    {0};
};