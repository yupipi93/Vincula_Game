#pragma once

#include "AITargetComponent.hpp"

class AICombatComponent : public AITargetComponent {
public:
    explicit AICombatComponent(const uint32_t id);
    ~AICombatComponent() override;
    
};