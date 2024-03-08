#pragma once

#include "Component.hpp"
#include <vector>

class EffectWinHeartComponent : public Component {
public:
    explicit EffectWinHeartComponent(const uint32_t id);
    ~EffectWinHeartComponent() override;
    

    bool active		{true};
    bool activated	{false};

};
