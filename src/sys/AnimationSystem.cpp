#include "AnimationSystem.hpp"

#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == ANIMATION SYSTEM ==
//  -
//
//   Using Componets:
//     -
//     -
//#####################################################

AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update(GameEngine &gameContext) const {
    updateEnemyAnimations(gameContext);
}


void AnimationSystem::updateEnemyAnimations(GameEngine &gameContext) const {

    auto &allEnemies = gameContext.entityMan.getComponents<AIComponent>();

    for (auto &enemy : allEnemies) {
        if(gameContext.getWindowFacadeRef().existsNode(enemy.id)) {

            auto animation = getAnimation(gameContext, enemy.id);
            switch(animation) {
            case ANIMATION_IDDLE:
                updateIdleAnimation(gameContext, enemy);
                break;
            case ANIMATION_WALKING:
                updateWalkingAnimation(gameContext, enemy);
                break;
            case ANIMATION_RUNNING_AWAY:
                updateRunningAwayAnimation(gameContext, enemy);
                break;
            case ANIMATION_CHARGIN_LUNGE:
                updateChargingLungeAnimation(gameContext, enemy);
                break;
            case ANIMATION_LUNGING:
                updateLungingAnimation(gameContext, enemy);
                break;
            case ANIMATION_MELEE:
                updateAttackMeleeAnimation(gameContext, enemy);
                break;
            case ANIMATION_DISTANCE:
                updateAttackDistanceAnimation(gameContext, enemy);
                break;
            }
        }
    }
}

void AnimationSystem::updateIdleAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case CHICK:
        if(enemyDrawable.model.compare("./media/enemies/rodent/StillChick.obj")) {
            enemyDrawable.model = "./media/enemies/rodent/StillChick.obj";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        break;
    case PIG:
        if(enemyDrawable.model.compare("./media/enemies/pig/Pig.obj")) {
            enemyDrawable.model = "./media/enemies/pig/Pig.obj";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        break;
    case PINEAPPLE:
        if(enemyDrawable.model.compare("./media/enemies/pineapple/pinecone.obj")) {
            enemyDrawable.model = "./media/enemies/pineapple/pinecone.obj";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        break;
    case SPIDER:
        if(enemyDrawable.model.compare("./media/enemies/spider/spiderIdle/spiderIdle")) {
            enemyDrawable.model = "./media/enemies/spider/spiderIdle/spiderIdle";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 12.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
        break;
    case MONKEY:
        if(enemyDrawable.model.compare("./media/enemies/monkey/monkeyIdle/monkeyIdle")) {
            enemyDrawable.model = "./media/enemies/monkey/monkeyIdle/monkeyIdle";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
        break;
    case GHOST:
        if(enemyDrawable.model.compare("./media/enemies/ghost/ghostIdle/ghostIdle")) {
            enemyDrawable.model = "./media/enemies/ghost/ghostIdle/ghostIdle";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
        break;
    }
}


void AnimationSystem::updateWalkingAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case CHICK:
        if(enemyDrawable.model.compare("./media/enemies/rodent/chickWalking/chickWalking")) {
            enemyDrawable.model = "./media/enemies/rodent/chickWalking/chickWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 15.f); //Out of the if because is the same animation that run
        break;
    case PIG:
        if(enemyDrawable.model.compare("./media/enemies/pig/pigWalking/pigWalking")) {
            enemyDrawable.model = "./media/enemies/pig/pigWalking/pigWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case PINEAPPLE:
        if(enemyDrawable.model.compare("./media/enemies/pineapple/pineconeWalking/pineconeWalking")) {
            enemyDrawable.model = "./media/enemies/pineapple/pineconeWalking/pineconeWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
            gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        }
        break;
    case SPIDER:
        if(enemyDrawable.model.compare("./media/enemies/spider/spiderWalking/spiderWalking")) {
            enemyDrawable.model = "./media/enemies/spider/spiderWalking/spiderWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case MONKEY:
        if(enemyDrawable.model.compare("./media/enemies/monkey/monkeyWalking/monkeyWalking")) {
            enemyDrawable.model = "./media/enemies/monkey/monkeyWalking/monkeyWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case GHOST:
        if(enemyDrawable.model.compare("./media/enemies/ghost/ghostIdle/ghostIdle")) {
            enemyDrawable.model = "./media/enemies/ghost/ghostIdle/ghostIdle";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    }

    gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
}

void AnimationSystem::updateRunningAwayAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case CHICK:
        if(enemyDrawable.model.compare("./media/enemies/rodent/chickWalking/chickWalking")) {
            enemyDrawable.model = "./media/enemies/rodent/chickWalking/chickWalking";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case PIG:
        if(enemyDrawable.model.compare("./media/enemies/pig/pigRunning/pigRunning")) {
            enemyDrawable.model = "./media/enemies/pig/pigRunning/pigRunning";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    }

    gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
}

void AnimationSystem::updateChargingLungeAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case PINEAPPLE:
        if(enemyDrawable.model.compare("./media/enemies/pineapple/pineconeChargingLunge/pineconeChargingLunge")) {
            enemyDrawable.model = "./media/enemies/pineapple/pineconeChargingLunge/pineconeChargingLunge";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case GHOST:
        if(enemyDrawable.model.compare("./media/enemies/ghost/ghostChargingLunge/ghostChargingLunge")) {
            enemyDrawable.model = "./media/enemies/ghost/ghostChargingLunge/ghostChargingLunge";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 32.f);
        break;
    }
}

void AnimationSystem::updateLungingAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case PINEAPPLE:
        if(enemyDrawable.model.compare("./media/enemies/pineapple/pineconeLunging/pineconeLunging")) {
            enemyDrawable.model = "./media/enemies/pineapple/pineconeLunging/pineconeLunging";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
        break;
    case GHOST:
        if(enemyDrawable.model.compare("./media/enemies/ghost/ghostLunge/ghostLunge.obj")) {
            enemyDrawable.model = "./media/enemies/ghost/ghostLunge/ghostLunge.obj";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        break;
    }
}


void AnimationSystem::updateAttackMeleeAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case SPIDER:
        if(enemyDrawable.model.compare("./media/enemies/spider/spiderAttack/spiderAttack")) {
            enemyDrawable.model = "./media/enemies/spider/spiderAttack/spiderAttack";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    }

    gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
}

void AnimationSystem::updateAttackDistanceAnimation(GameEngine &gameContext, AIComponent &enemy) const {
    DrawableComponent &enemyDrawable = gameContext.entityMan.getComponent<DrawableComponent>(enemy.id);

    switch(enemy.enemyType) {
    case MONKEY:
        if(enemyDrawable.model.compare("./media/enemies/monkey/monkeyDistanceAttack/monkeyDistanceAttack")) {
            enemyDrawable.model = "./media/enemies/monkey/monkeyDistanceAttack/monkeyDistanceAttack";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    case GHOST:
        if(enemyDrawable.model.compare("./media/enemies/ghost/ghostDistanceAttack/ghostDistanceAttack")) {
            enemyDrawable.model = "./media/enemies/ghost/ghostDistanceAttack/ghostDistanceAttack";
            gameContext.getWindowFacadeRef().updateModel(gameContext, enemy.id);
        }
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, enemy.id, 1.f / 30.f);
        break;
    }

    gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, enemy.id, true);
}


EnemyAnimation AnimationSystem::getAnimation(GameEngine &gameContext, uint32_t entityId) const {
    bool lunge = gameContext.entityMan.existsComponent<AIActionLungeComponent>(entityId);
    if (lunge) {
        if (gameContext.entityMan.getComponent<AIActionLungeComponent>(entityId).attackId) {
            // Cargando
            return ANIMATION_LUNGING;
        } else {
            // Preparando cargar
            return ANIMATION_CHARGIN_LUNGE;
        }
    }


    bool escape = gameContext.entityMan.existsComponent<AIActionEscapeComponent>(entityId);
    if (escape) {
        // Moviendose para escapar
        return ANIMATION_RUNNING_AWAY;
    }

    bool melee = gameContext.entityMan.existsComponent<AIActionMeleeComponent>(entityId);
    if (melee) {
        // Ataque cuerpo a cuerpo
        return ANIMATION_MELEE;
    }


    bool distance = gameContext.entityMan.existsComponent<AIActionDistanceComponent>(entityId);
    if (distance) {
        // Ataque a distancia
        return ANIMATION_DISTANCE;
    }



    bool hasMoveTo = gameContext.entityMan.existsComponent<AIMoveToComponent>(entityId);
    bool hasTarget = gameContext.entityMan.getComponent<AIMoveToComponent>(entityId).targetId;
    bool moving    = hasMoveTo && hasTarget;

    if (moving) {
        // Moviendose
        return ANIMATION_WALKING;
    }

    // Quieto
    return ANIMATION_IDDLE;
}



