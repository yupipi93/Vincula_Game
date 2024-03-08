#pragma once

#include "AIActionComponent.hpp"

class AIActionMeleeComponent : public AIActionComponent {
public:
    explicit AIActionMeleeComponent(const uint32_t id);
    ~AIActionMeleeComponent() override;
};