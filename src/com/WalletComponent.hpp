#pragma once

#include "Component.hpp"

class WalletComponent : public Component {
public:
    explicit WalletComponent(const uint32_t id);
    ~WalletComponent() override;

    uint16_t coins {0};
};
