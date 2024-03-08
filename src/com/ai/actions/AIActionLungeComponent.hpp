#pragma once

#include "AIActionComponent.hpp"

class AIActionLungeComponent : public AIActionComponent {
public:
    explicit AIActionLungeComponent(const uint32_t id);
    ~AIActionLungeComponent() override;

   float maxTempToAtack {0.75};
   float tempToAtack {maxTempToAtack};
   uint32_t attackId {0};
};