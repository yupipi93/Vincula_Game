#pragma once

#include "AITargetComponent.hpp"

class AIMoveTo3DComponent : public AITargetComponent {
public:
    explicit AIMoveTo3DComponent(const uint32_t id);
    ~AIMoveTo3DComponent() override;
};