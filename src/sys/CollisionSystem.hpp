#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CollisionSystem : public System {
public:
    CollisionSystem();
    ~CollisionSystem();

    void update(GameEngine &gameContext) const override;
};