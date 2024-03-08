#pragma once

#include "Component.hpp"

class AtributesComponent : public Component {
public:
    explicit AtributesComponent(const uint32_t id);
    ~AtributesComponent() override;

    bool isSecure {true};
    uint32_t secureZone {0};

    bool wasInBanckDepositor {false};
    bool wasInBanckExtractor {false};
};
