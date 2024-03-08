#pragma once

#include "Component.hpp"
#include <vector>

class CoinComponent : public Component {
public:
    explicit CoinComponent(const uint32_t id);
    ~CoinComponent() override;

    uint16_t	value	{1};
    uint32_t	owner	{0};
};
