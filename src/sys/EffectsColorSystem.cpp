#include "EffectsColorSystem.hpp"

#include <vector>

#include <eng/GameEngine.hpp>
//console.log



//#####################################################
//                == EFFECTS SYSTEM ==
//  - Check all effects components and do the pertinent action
//
//   Using Componets:
//      - PoisonComponent    (Read)
//      - ParalysisComponent (Read)
//      - StatsComponent     (Write)
//      - VelocityComponent  (Write)
//#####################################################


// EFFECTS COLORS
const std::array<float, 3>  normalColor    {1.0f, 1.0f, 1.0f};
const std::array<float, 3>  hitColor       {1.0f, 0.0f, 0.0f};
const std::array<float, 3>  poisonColor    {0.3f, 1.0f,  0.3f};
const std::array<float, 3>  paralysisColor {1.0f, 0.94f, 0.1f};
const std::array<float, 3>  slowdownColor  {0.1f, 0.3f,  1.0f};
const std::array<float, 3>  winheartColor  {10.f / 255.f, 10.f / 255.f, 10.f / 255.f};
const std::array<float, 3>  healingColor   {0.f / 255.f, 130.f / 255.f, 0.f / 255.f};


EffectsColorSystem::EffectsColorSystem() {}

EffectsColorSystem::~EffectsColorSystem() {}


void EffectsColorSystem::update(GameEngine &gameContext) const  {

    resetColors                 (gameContext);
    addWinheartEffectsColor     (gameContext);
    addPoisonEffectsColor       (gameContext);
    addParalysisEffectsColor    (gameContext);
    addSlowdownEffectsColor     (gameContext);
    addHealthStealEffectsColor  (gameContext);

}
void EffectsColorSystem::resetColors(GameEngine &gameContext) const {
    auto &allEnemies = gameContext.entityMan.getComponents<AIComponent>();
    for (auto &enemyAi : allEnemies) {
        applyNormalOrHitColor(gameContext, enemyAi.id);
    }

    auto &allEggs = gameContext.entityMan.getComponents<EggDataComponent>();
    for (auto &eggs : allEggs) {
        applyNormalOrHitColor(gameContext, eggs.id);
    }
}

void EffectsColorSystem::applyNormalOrHitColor(GameEngine &gameContext, uint32_t id) const {
    auto &stats = gameContext.entityMan.getComponent<StatsComponent>(id);

    if (stats.damaged)
        gameContext.getWindowFacadeRef().addColorToEntity(id, hitColor);
    else {
        gameContext.getWindowFacadeRef().addColorToEntity(id, normalColor);
    }

}
void EffectsColorSystem::addPoisonEffectsColor(GameEngine &gameContext) const {
    auto &effectsPoison = gameContext.entityMan.getComponents<EffectPoisonComponent>();
    for (auto &efpo : effectsPoison) {
        // Set effect color
        gameContext.getWindowFacadeRef().addColorToEntity(efpo.id, poisonColor);
    }
}

void EffectsColorSystem::addParalysisEffectsColor(GameEngine &gameContext) const {
    auto &effectsParalysis = gameContext.entityMan.getComponents<EffectParalysisComponent>();
    for (auto &efpa : effectsParalysis) {
        // Set effect color
        gameContext.getWindowFacadeRef().addColorToEntity(efpa.id, paralysisColor);
    }
}

void EffectsColorSystem::addSlowdownEffectsColor(GameEngine &gameContext) const {
    auto &effectsSlowdown = gameContext.entityMan.getComponents<EffectSlowdownComponent>();
    for (auto &efsd : effectsSlowdown) {
        // Set effect color
        gameContext.getWindowFacadeRef().addColorToEntity(efsd.id, slowdownColor);
    }
}

void EffectsColorSystem::addWinheartEffectsColor(GameEngine &gameContext) const {
    auto &effectsWinheart = gameContext.entityMan.getComponents<EffectWinHeartComponent>();
    for (auto &efwh : effectsWinheart) {
        // Set effect color
        gameContext.getWindowFacadeRef().addColorToEntity(efwh.id, winheartColor);
    }
    /*
        auto &allEnemies = gameContext.entityMan.getComponents<AIComponent>();
        for (auto &enemyAi : allEnemies) {
            // Set effect color
            if (enemyAi.aggredBy != gameContext.playerId)
                for (auto type : enemyAi.hostileTo) {
                    if (type == ENEMY) {
                        gameContext.getWindowFacadeRef().addColorToEntity(enemyAi.id, winheartColor);
                    }
                }
        }*/
}

void EffectsColorSystem::addHealthStealEffectsColor(GameEngine &gameContext) const {
    auto &allStats = gameContext.entityMan.getComponents<StatsComponent>();
    for (auto &stat : allStats) {
        // Set effect color
        if (stat.healing) {
            gameContext.getWindowFacadeRef().addColorToEntity(stat.id, healingColor);
        }
    }
}

