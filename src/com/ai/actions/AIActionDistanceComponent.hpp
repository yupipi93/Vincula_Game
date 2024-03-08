#pragma once

#include "AIActionComponent.hpp"

class AIActionDistanceComponent : public AIActionComponent {
public:
    explicit AIActionDistanceComponent(const uint32_t id);
    ~AIActionDistanceComponent() override;
};