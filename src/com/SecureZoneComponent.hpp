#pragma once

#include "Component.hpp"
#include <array>


class SecureZoneComponent : public Component {
public:
    explicit SecureZoneComponent(const uint32_t id);
    ~SecureZoneComponent() override;

    bool isActive {false};

    // For the light
    uint32_t cristalLightId {0};
    std::array<float, 3>  activeLightColor {0.15f, 1.f, 0.3f};


};
