#pragma once

#include "Component.hpp"

class InfoBoxComponent : public Component {
public:
    explicit InfoBoxComponent(const uint32_t id);
    ~InfoBoxComponent() override;

    uint32_t parentEntityId {0};
};
