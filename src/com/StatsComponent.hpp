#pragma once

#include "Component.hpp"

class StatsComponent : public Component {
public:
    explicit StatsComponent(const uint32_t id);
    ~StatsComponent() override;

    void resetHealth();

    uint16_t    maxHealth           {10};
    uint16_t    currentHealth       {10};
    uint16_t    lastHealth          {0};

    uint16_t    damageReceived      {0};

    uint16_t    lastDamageReceived  {0};
    uint8_t     damage              {1};

    uint16_t    lastDamageHealing   {0};
    uint16_t    damageHealing       {0};

    bool        isCritic            {false};
    bool        damaged             {false};
    float       damagedTime         {0};
    
    bool        healing             {false};
    float       healingTime         {0};
    
    float       deadTime            {0};
};
