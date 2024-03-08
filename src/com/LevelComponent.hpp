#pragma once
#include "Component.hpp"
#include <stdint.h>

class LevelComponent : public Component {
public:
    explicit LevelComponent(const uint32_t id);
    ~LevelComponent() override;

    uint8_t level {1};

    bool    dashUnlocked    {false};
    bool    compassUnlocked {false};
    bool    mapUnlocked     {false};
    bool	levelingUp		{false};
    float	levelingTime	{0};
};