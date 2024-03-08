#pragma once

#include "AIActionComponent.hpp"

class AIActionEscapeComponent : public AIActionComponent {
public:
    explicit AIActionEscapeComponent(const uint32_t id);
    ~AIActionEscapeComponent() override;
};