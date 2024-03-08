#pragma once

#include "Component.hpp"
#include <vector>

class KeyComponent : public Component {
public:
    explicit KeyComponent(const uint32_t id);
    ~KeyComponent() override;

    uint8_t	keyType	{0};
};
