#pragma once
#include <com/Component.hpp>
#include <enum/EntityType.hpp>
#include <enum/AiAction.hpp>
#include <enum/AiState.hpp>

#include <vector>

class AIDifuseStateComponent : public Component {
public:
    explicit AIDifuseStateComponent(const uint32_t id);
    ~AIDifuseStateComponent() override;

    // HEALTH
    uint16_t    dyingX0         {0};
    uint16_t    dyingX1         {0};

    uint16_t    injuredX0       {0};
    uint16_t    injuredX1       {0};
    uint16_t    injuredX2       {0};
    uint16_t    injuredX3       {0};

    uint16_t    healthyX0       {0};
    uint16_t    healthyX1       {100};


    // STRENGTH
    int8_t      looserX0        {-1};
    int8_t      looserX1        {0};

    int8_t      strongerX0      {0};
    int8_t      strongerX1      {1};


    // MONEY
    int16_t     poorerX0        {-1};
    int16_t     poorerX1        {0};

    int16_t     richerX0        {0};
    int16_t     richerX1        {1};

};