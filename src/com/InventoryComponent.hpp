#pragma once

#include "Component.hpp"
#include <vector>

class InventoryComponent : public Component {
public:
    explicit InventoryComponent(const uint32_t id);
    ~InventoryComponent() override;

    void setMaxKeys(const uint8_t keys);

    uint32_t				primaryWeaponId		{0};
    uint32_t				secondaryWeaponId	{0};

    std::vector<uint8_t>	keysCollected		{};
    uint8_t					MAX_KEYS			{0};

    float					showKeysTime		{0};
};
