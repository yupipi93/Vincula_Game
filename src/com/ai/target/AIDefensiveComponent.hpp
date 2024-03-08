#pragma once

#include "AITargetComponent.hpp"

class AIDefensiveComponent : public AITargetComponent {
public:
    explicit AIDefensiveComponent(const uint32_t id);
    ~AIDefensiveComponent() override;

    uint32_t friendId = 0;
    bool friendConfirmed = false;
    
};