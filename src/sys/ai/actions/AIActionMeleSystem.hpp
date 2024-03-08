#pragma once

#include <sys/System.hpp>

class AIActionMeleSystem : public System {
public:
    AIActionMeleSystem();
    ~AIActionMeleSystem();

    void update(GameEngine &gameContext) const override;

private:
    void createMeleeAttack(GameEngine &gameContext, MeleeWeaponComponent &meleeWeapon, SituationComponent &entitySituation, StatsComponent &entityStats, EntityType entityType) const;

};