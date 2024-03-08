#include "EffectsSystem.hpp"

#include <vector>

#include <eng/GameEngine.hpp>

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

// NORMAL COLORS
const std::array<float, 3>  normalColor    {1.0f, 1.0f, 1.0f};


EffectsSystem::EffectsSystem() {}

EffectsSystem::~EffectsSystem() {}


void EffectsSystem::update(GameEngine &gameContext) const  {

    //TEMP
    addRorationToBanane(gameContext);

    checkWinHeartEffects(gameContext);
    checkPoisonEffects(gameContext);
    checkParalysisEffects(gameContext);
    checkSlowdownEffects(gameContext);
    checkMagnifyBulletEffects(gameContext);

    deleteEffectsWinHeart(gameContext);
    deleteEffectsPoison(gameContext);
    deleteEffectsParalysis(gameContext);
    deleteEffectsSlowdown(gameContext);
    deleteEffectsMagnifyBullet(gameContext);

}

void EffectsSystem::addRorationToBanane(GameEngine &gameContext) const {

    auto &allAttackComp = gameContext.entityMan.getComponents<AttackComponent>();
    for (auto &attackComp : allAttackComp) {
        if(attackComp.rotative) {
            auto &bulletSit = gameContext.entityMan.getComponent<SituationComponent>(attackComp.id);
            bulletSit.rotation += 500 * gameContext.getDeltaTime();

            gameContext.getWindowFacadeRef().updateEntities(gameContext, {bulletSit.id});


        }
    }

}

void EffectsSystem::checkWinHeartEffects(GameEngine &gameContext) const {
    auto &effectsWinheart = gameContext.entityMan.getComponents<EffectWinHeartComponent>();

    for (auto &efwh : effectsWinheart) {
        if (!efwh.activated) {
            AIComponent &aiEnemy = gameContext.entityMan.getComponent<AIComponent>(efwh.id);
            aiEnemy.hostileTo.clear();
            aiEnemy.aggredBy = 0;
            aiEnemy.forceStandBy = true;
            aiEnemy.hostileTo.emplace_back(ENEMY);
            efwh.activated = true;
        }
    }
}


void EffectsSystem::checkMagnifyBulletEffects(GameEngine &gameContext) const {
    auto &effectsMagnifyBullet = gameContext.entityMan.getComponents<EffectMagnifyBulletComponent>();

    for (auto &efmb : effectsMagnifyBullet) {
        DrawableComponent &drawableAttack = gameContext.entityMan.getComponent<DrawableComponent>(efmb.id);
        CollisionComponent &collisionAttack = gameContext.entityMan.getComponent<CollisionComponent>(efmb.id);
        SituationComponent &situationAttack = gameContext.entityMan.getComponent<SituationComponent>(efmb.id);
        drawableAttack.scaleZ    += efmb.MAGNIFY_ADD_SCALE * gameContext.getDeltaTime();
        drawableAttack.scaleY    += efmb.MAGNIFY_ADD_SCALE * gameContext.getDeltaTime();
        drawableAttack.scaleX    += efmb.MAGNIFY_ADD_SCALE * gameContext.getDeltaTime();
        collisionAttack.sizeX    += (efmb.MAGNIFY_ADD_SCALE * 10 * gameContext.getDeltaTime());
        collisionAttack.sizeY    += (efmb.MAGNIFY_ADD_SCALE * 10 * gameContext.getDeltaTime());
        collisionAttack.sizeZ    += (efmb.MAGNIFY_ADD_SCALE * 10 * gameContext.getDeltaTime());
        situationAttack.rotation += efmb.MAGNIFY_ADD_ROTATE * gameContext.getDeltaTime();
    }

}

void EffectsSystem::checkPoisonEffects(GameEngine &gameContext) const {
    auto &effectsPoison = gameContext.entityMan.getComponents<EffectPoisonComponent>();

    for (auto &efpo : effectsPoison) {
        efpo.poisonTime -= gameContext.getDeltaTime();
        if (efpo.poisonTime <= 0) {
            StatsComponent &statsEntity = gameContext.entityMan.getComponent<StatsComponent>(efpo.id);
            statsEntity.damageReceived += efpo.damage;
            statsEntity.damaged = true;
            statsEntity.damagedTime = 0;
        }
    }
}

void EffectsSystem::checkParalysisEffects(GameEngine &gameContext) const {
    auto &effectsParalysis = gameContext.entityMan.getComponents<EffectParalysisComponent>();

    for (auto &efpa : effectsParalysis) {
        VelocityComponent &velocityEntity = gameContext.entityMan.getComponent<VelocityComponent>(efpa.id);
        velocityEntity.speed = 0;
    }
}

void EffectsSystem::checkSlowdownEffects(GameEngine &gameContext) const {
    auto &effectsSlowdown = gameContext.entityMan.getComponents<EffectSlowdownComponent>();
    for (auto &efsd : effectsSlowdown) {
        VelocityComponent &velocityEntity = gameContext.entityMan.getComponent<VelocityComponent>(efsd.id);
        velocityEntity.velocityX = velocityEntity.velocityX * efsd.SLOWDOWN_FACTOR;
        velocityEntity.velocityZ = velocityEntity.velocityZ * efsd.SLOWDOWN_FACTOR;
        velocityEntity.velocityY = velocityEntity.velocityY * efsd.SLOWDOWN_FACTOR;
    }
}

void EffectsSystem::deleteEffectsWinHeart(GameEngine &gameContext) const {
    auto &effectsWinheart = gameContext.entityMan.getComponents<EffectWinHeartComponent>();

    for (auto &efwh : effectsWinheart) {
        AIComponent &aiEnemy = gameContext.entityMan.getComponent<AIComponent>(efwh.id);
        if (!efwh.active) {
            gameContext.entityMan.eraseComponent<EffectWinHeartComponent>(efwh.id);
            aiEnemy.hostileTo.clear();
            //aiEnemy.forceStandBy = true;
            aiEnemy.hostileTo.emplace_back(PLAYER);
        }

    }
}

void EffectsSystem::deleteEffectsPoison(GameEngine &gameContext) const {
    auto &effectsPoison = gameContext.entityMan.getComponents<EffectPoisonComponent>();

    std::vector<uint32_t> effectsPoisonToDelete;
    for (auto &efpo : effectsPoison) {
        if(efpo.poisonTime <= 0) {
            efpo.poisonTime = efpo.poisonMaxTime;
            --efpo.damageTimes;
            if (efpo.damageTimes == 0)
                effectsPoisonToDelete.push_back(efpo.id);
        }
    }

    for(uint32_t id : effectsPoisonToDelete) {
        gameContext.entityMan.eraseComponent<EffectPoisonComponent>(id);
        // Set to normal color
        gameContext.getWindowFacadeRef().addColorToEntity(id, normalColor);
    }
}

void EffectsSystem::deleteEffectsParalysis(GameEngine &gameContext) const {
    auto &effectsParalysis = gameContext.entityMan.getComponents<EffectParalysisComponent>();

    std::vector<uint32_t> effectsParalysisToDelete;
    for (auto &efpa : effectsParalysis) {
        efpa.paralysisTime -= gameContext.getDeltaTime();
        if(efpa.paralysisTime <= 0) {
            effectsParalysisToDelete.push_back(efpa.id);
            VelocityComponent &velocityEntity = gameContext.entityMan.getComponent<VelocityComponent>(efpa.id);

            velocityEntity.speed = velocityEntity.speedNormal;
        }
    }

    for(uint32_t id : effectsParalysisToDelete) {
        gameContext.entityMan.eraseComponent<EffectParalysisComponent>(id);
        // Set to normal color
        gameContext.getWindowFacadeRef().addColorToEntity(id, normalColor);
    }
}

void EffectsSystem::deleteEffectsSlowdown(GameEngine &gameContext) const {
    auto &effectsSlowdown = gameContext.entityMan.getComponents<EffectSlowdownComponent>();

    std::vector<uint32_t> effectsSlowdownToDelete;
    for (auto &efsd : effectsSlowdown) {
        efsd.slowdownTime -= gameContext.getDeltaTime();
        if(efsd.slowdownTime <= 0) {
            effectsSlowdownToDelete.push_back(efsd.id);
        }
    }

    for(uint32_t id : effectsSlowdownToDelete) {
        gameContext.entityMan.eraseComponent<EffectSlowdownComponent>(id);
        // Set to normal color
        gameContext.getWindowFacadeRef().addColorToEntity(id, normalColor);
    }
}

void EffectsSystem::deleteEffectsMagnifyBullet(GameEngine &gameContext) const {
    auto &effectsMagnifyBullet = gameContext.entityMan.getComponents<EffectMagnifyBulletComponent>();

    std::vector<uint32_t> effectsMagnifyBulletToDelete;
    for (auto &efmb : effectsMagnifyBullet) {
        efmb.magnifyBulletTime -= gameContext.getDeltaTime();
        if(efmb.magnifyBulletTime <= 0) {
            effectsMagnifyBulletToDelete.push_back(efmb.id);
        }
    }

    for(uint32_t id : effectsMagnifyBulletToDelete) {
        gameContext.entityMan.eraseComponent<EffectMagnifyBulletComponent>(id);
    }
}


