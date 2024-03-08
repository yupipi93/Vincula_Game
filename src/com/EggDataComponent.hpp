#pragma once

#include "Component.hpp"
#include <string>
#include <cstdint>

class EggDataComponent : public Component {
public:
    explicit EggDataComponent(const uint32_t id);
    ~EggDataComponent() override;

    bool	broken		{false};
    bool isChanting 	{true};
    uint8_t	biome		{0};
    std::string modelEggBroken {""};
    float healthHeight	{100.f};
};
