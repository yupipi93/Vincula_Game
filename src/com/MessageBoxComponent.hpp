#pragma once

#include "Component.hpp"

class MessageBoxComponent : public Component {
public:
    explicit MessageBoxComponent(const uint32_t id);
    ~MessageBoxComponent() override;

    uint32_t parentEntityId {0};
    float    lifeTime       {5};
};
