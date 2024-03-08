#pragma once

#include "AITargetComponent.hpp"

class AIOffensiveComponent : public AITargetComponent {
public:
    explicit AIOffensiveComponent(const uint32_t id);
    ~AIOffensiveComponent() override;

    bool inAction = false;

};