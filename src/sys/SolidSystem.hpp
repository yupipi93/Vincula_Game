#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class SolidSystem : public System {
public:
    SolidSystem();
    ~SolidSystem();

    void update(GameEngine &gameContext) const override;
private:
    void checkCollisionsType(GameEngine &gameContext, CollisionComponent &currentCollidable, const float limitLeft, const float limitRigth, const float limitTop, const float limitBottom) const;
    void checkAndUpdateCollisions(GameEngine &gameContext, uint32_t currentId, uint32_t collisionId, CollisionComponent &currentCollidable, CollisionComponent &collisionCollidable, const float limitLeft, const float limitRigth, const float limitTop, const float limitBottom) const;

    void playerNeverOutOfWall(GameEngine &gameContext) const;
};