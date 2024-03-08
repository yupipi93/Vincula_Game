#pragma once

#include "AITargetComponent.hpp"

class AIMoveToComponent : public AITargetComponent {
public:
    explicit AIMoveToComponent(const uint32_t id);
    ~AIMoveToComponent() override;
};