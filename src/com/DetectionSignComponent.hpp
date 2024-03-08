#pragma once

#include "Component.hpp"

class DetectionSignComponent : public Component {
public:
    explicit DetectionSignComponent(const uint32_t id);
    ~DetectionSignComponent() override;

    float rotationSpeed    {75};

    float levitationSpeed  {2.5};
    float signVelocity     {0};
    float levitationMargin {3};
  	float initialHeight    {0};
};