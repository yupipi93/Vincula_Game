#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class EffectsSystem : public System {
public:
    EffectsSystem();
    ~EffectsSystem();

    void update(GameEngine &gameContext) const override;


private:
    void addRorationToBanane(GameEngine& gameContext) const;
    
    void checkWinHeartEffects(GameEngine& gameContext) const;
    void checkPoisonEffects(GameEngine& gameContext) const;
    void checkParalysisEffects(GameEngine& gameContext) const;
    void checkSlowdownEffects(GameEngine& gameContext) const;
    void checkMagnifyBulletEffects(GameEngine& gameContext) const;

    void deleteEffectsWinHeart(GameEngine& gameContext) const;
    void deleteEffectsPoison(GameEngine& gameContext) const;
    void deleteEffectsParalysis(GameEngine& gameContext) const;
    void deleteEffectsSlowdown(GameEngine& gameContext) const;
    void deleteEffectsMagnifyBullet(GameEngine& gameContext) const;

};