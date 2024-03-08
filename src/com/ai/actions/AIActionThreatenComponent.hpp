#pragma once

#include "AIActionComponent.hpp"

class AIActionThreatenComponent : public AIActionComponent {
public:
    explicit AIActionThreatenComponent(const uint32_t id);
    ~AIActionThreatenComponent() override;
};