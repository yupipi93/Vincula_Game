#pragma once

#include "AITargetComponent.hpp"

class AIStandByComponent : public AITargetComponent {
public:
    explicit AIStandByComponent(const uint32_t id);
    ~AIStandByComponent() override;
};