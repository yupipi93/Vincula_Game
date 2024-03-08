#pragma once

#include "AIActionComponent.hpp"

class AIActionWanderComponent : public AIActionComponent {
public:
    explicit AIActionWanderComponent(const uint32_t id);
    ~AIActionWanderComponent() override;

    uint8_t	sleepTime	{0};
};