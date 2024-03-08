#pragma once

#include "Component.hpp"

class CompassComponent : public Component {
public:
    explicit CompassComponent(const uint32_t id);
    ~CompassComponent() override;

    float lifeTime {7};
    bool increaseAnimation = true;
};