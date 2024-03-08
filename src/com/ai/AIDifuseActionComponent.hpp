#pragma once
#include <com/Component.hpp>
#include <enum/EntityType.hpp>
#include <enum/AiAction.hpp>
#include <enum/AiState.hpp>

#include <vector>

class AIDifuseActionComponent : public Component {
public:
    explicit AIDifuseActionComponent(const uint32_t id);
    ~AIDifuseActionComponent() override;

    // MELEE
    float meleeX0 {0};
    float meleeX1 {0};

    // DISTANCE
    float distanceX0 {0};
    float distanceX1 {0};
    float distanceX2 {0};
    float distanceX3 {0};

    // LUNGE
    float lungeX0 {0};
    float lungeX1 {0};
    float lungeX2 {0};
    float lungeX3 {0};

    // FOLLOW
    float followX0 {0};
    float followX1 {0};

    // KEEPDISTANCE
    float keepDistanceX0 {0};
    float keepDistanceX1 {0};
    float keepDistanceX2 {0};
    float keepDistanceX3 {0};
};