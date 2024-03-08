#pragma once

#include "AIActionComponent.hpp"

class AIActionKeepDistanceComponent : public AIActionComponent {
public:
    explicit AIActionKeepDistanceComponent(const uint32_t id);
    ~AIActionKeepDistanceComponent() override;
};