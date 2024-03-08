#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    ~PhysicsSystem();

    void update(GameEngine &gameContext) const override;

private:
    void setTargetOfPlayer(GameEngine &gameContext) const;


    uint8_t stablishAnimationDirection(GameEngine &gameContext) const;
    void 	updatePlayerRotation(uint32_t &playerId, GameEngine &gameContext) const;
    void 	updateSituations(GameEngine &gameContext) const;
    void 	playerMovement(uint32_t &playerId, GameEngine &gameContext) const;

    void updateDashAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint8_t animationDirection, uint32_t playerId) const;
    void updateMovementAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint8_t animationDirection, uint32_t playerId) const;
    void updateDanceAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint32_t playerId) const;
    void updateBoredAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, InputComponent &playerInput, uint32_t playerId) const;
};