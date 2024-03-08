#pragma once

#include "Component.hpp"

class IluminationComponent : public Component {
public:
    explicit IluminationComponent(const uint32_t id);
    ~IluminationComponent() override;

    //bool isActive {false};
    uint32_t pointLightId {0};
};