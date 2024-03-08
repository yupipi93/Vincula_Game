#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class EffectsColorSystem : public System {
public:
    EffectsColorSystem();
    ~EffectsColorSystem();

    void update(GameEngine &gameContext) const override;

private:
    void resetColors(GameEngine &gameContext) const;

	void applyNormalOrHitColor(GameEngine &gameContext, uint32_t id) const;
    void addPoisonEffectsColor(GameEngine &gameContext) const;
    void addParalysisEffectsColor(GameEngine &gameContext) const;
    void addSlowdownEffectsColor(GameEngine &gameContext) const;
    void addWinheartEffectsColor(GameEngine &gameContext) const;
    void addHealthStealEffectsColor(GameEngine &gameContext) const;

};