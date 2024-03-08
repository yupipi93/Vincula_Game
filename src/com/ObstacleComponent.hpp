#pragma once

#include <enum/GameObjectType.hpp>
#include "Component.hpp"

class ObstacleComponent : public Component {
public:
    explicit ObstacleComponent(const uint32_t id);
    ~ObstacleComponent() override;

    GameObjectType obstacleType;
};
