#pragma once

#include "Component.hpp"

class IPDataComponent : public Component {
public:
    explicit IPDataComponent(const uint32_t id);
    ~IPDataComponent() override;

    uint32_t secureZoneId {0};
    bool isDiscoveredForPlayer {false};
    bool isChanting {true};
};
