#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AnimationSystem : public System {
public:
    AnimationSystem();
    ~AnimationSystem();

    void update(GameEngine &gameContext) const override;
private:
	EnemyAnimation getAnimation(GameEngine &gameContext, uint32_t entityId) const;
	void updateEnemyAnimations(GameEngine &gameContext) const;

	void updateIdleAnimation(GameEngine &gameContext, AIComponent &enemy) const;    
    void updateWalkingAnimation(GameEngine &gameContext, AIComponent &enemy) const; 
    void updateRunningAwayAnimation(GameEngine &gameContext, AIComponent &enemy) const;
    void updateChargingLungeAnimation(GameEngine &gameContext, AIComponent &enemy) const;
    void updateLungingAnimation(GameEngine &gameContext, AIComponent &enemy) const;
    void updateAttackMeleeAnimation(GameEngine &gameContext, AIComponent &enemy) const;
    void updateAttackDistanceAnimation(GameEngine &gameContext, AIComponent &enemy) const;
};