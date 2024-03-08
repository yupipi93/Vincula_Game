#include "AIStateMachineSystem.hpp"

#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == AI STATE MACHINE SYSTEM ==
//  - Check if IAEntities detect other dangerous enties
//  - Set Combat state or StandBy state depending situation
//
//   Using Componets:
//      - AI            (Read - Write)
//      - Sensor        (Read)
//      - AiOffensive   (Create - Delete)
//      - AiDefensive   (Create - Delete)
//      - AiStandBy     (Create - Delete)
//#####################################################

AIStateMachineSystem::AIStateMachineSystem() {}

AIStateMachineSystem::~AIStateMachineSystem() {}

void AIStateMachineSystem::update(GameEngine &gameContext) const {

    auto &allAiEntities {gameContext.entityMan.getComponents<AIComponent>()};

    for(auto &ai : allAiEntities) {
        changeEntityState(gameContext, ai);

    }
}

void AIStateMachineSystem::changeEntityState(GameEngine &gameContext, AIComponent &aIComponent) const {
    uint32_t entityId = aIComponent.id;
    auto dangerousEntityId = getDetectDangerousEntity(gameContext, entityId, aIComponent);
    if(!aIComponent.forceStandBy && (dangerousEntityId || aIComponent.forceCombat)) {
        setCombatState(gameContext, aIComponent, dangerousEntityId);
        aIComponent.currentAlertState = aIComponent.defaultAlertState;
        aIComponent.forceCombat = false;
    } else if(aIComponent.forceStandBy || (!dangerousEntityId && aIComponent.currentAlertState <= 0)) {
        setStandByState(gameContext, aIComponent);
        aIComponent.forceStandBy = false;
    } else if(aIComponent.currentAlertState > 0) {
        aIComponent.currentAlertState -= gameContext.getDeltaTime() * 50;
    }

}



void AIStateMachineSystem::setCombatState(GameEngine &gameContext, AIComponent &aIComponent, uint32_t entityTarget) const {
    uint32_t entityId = aIComponent.id;
    auto &moveToComp = gameContext.entityMan.getComponent<AIMoveToComponent>(entityId);

    if(aIComponent.lastState != OFFENSIVE && aIComponent.lastState != DEFENSIVE) {//FFENSIVE || DEFENSIVE = COMBAT
        //Save standBy Target in Combat
        uint32_t standByTargetId = 0;
        if(aIComponent.lastState == STANDBY) {

            if (gameContext.entityMan.existsComponent<AIStandByComponent>(entityId))
                standByTargetId = gameContext.entityMan.getComponent<AIStandByComponent>(entityId).targetId;

        }

        deleteLastStateComponent(gameContext, entityId);

        //Clean Target
        gameContext.eraseEntityByID(moveToComp.targetId);
        moveToComp.targetId = 0;

        //Create New Component
        AICombatComponent &combatComponent = gameContext.entityMan.createComponent<AICombatComponent>(entityId);

        if (entityTarget)
            combatComponent.targetId = entityTarget;
        else if(aIComponent.aggredBy)
            combatComponent.targetId = aIComponent.aggredBy;

        // adjust melee to attack to the taget
        if (gameContext.entityMan.getEntity(entityId).getGameObjectType() == ENEMY_SPIDER) {
            auto                    &aiDifuseActionComp         = gameContext.entityMan.getComponent<AIDifuseActionComponent>(entityId);
            auto                    &meleeWeaponComp            = gameContext.entityMan.getComponent<MeleeWeaponComponent>(entityId);
            auto                     COLLIDABLE_SIZE            = gameContext.entityMan.getComponent<CollisionComponent>(entityId).sizeX;
            auto                     TARGET_COLLIDABLE_SIZE     = gameContext.entityMan.getComponent<CollisionComponent>(combatComponent.targetId).sizeX;

            aiDifuseActionComp.meleeX1 = COLLIDABLE_SIZE / 2 + meleeWeaponComp.attackSize / 1.1 + TARGET_COLLIDABLE_SIZE / 2;
            aiDifuseActionComp.followX0 = aiDifuseActionComp.meleeX1;

        }


        if(standByTargetId) {
            combatComponent.targetId = standByTargetId;
        }



        aIComponent.lastState = NO_STATE;//OFFENSIVE || DEFENSIVE = COMBAT
    }

}



void AIStateMachineSystem::setStandByState(GameEngine &gameContext, AIComponent &aIComponent) const {
    uint32_t entityId = aIComponent.id;

    if(aIComponent.lastState != STANDBY) {
        aIComponent.lastState = STANDBY;

        deleteLastStateComponent(gameContext, entityId);
        AITargetComponent &standByComponent = gameContext.entityMan.createComponent<AIStandByComponent>(entityId);


        //Set Green texture to enemy ()
        //DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(entityId);
        //drawable.texture = "./media/enemy-green.png";//TEMP
        //gameContext.getWindowFacadeRef().updateTextures(gameContext, {entityId});
        gameContext.getSoundFacadeRef().setParameterEventByID(entityId, STANDBY_SOUND);


    }



}


void AIStateMachineSystem::deleteLastStateComponent(GameEngine &gameContext, uint32_t entityId) const {

    //Erase states
    gameContext.entityMan.eraseComponent<AIStandByComponent>(entityId);
    gameContext.entityMan.eraseComponent<AICombatComponent>(entityId);
    gameContext.entityMan.eraseComponent<AIOffensiveComponent>(entityId);
    gameContext.entityMan.eraseComponent<AIDefensiveComponent>(entityId);


}




uint32_t AIStateMachineSystem::getDetectDangerousEntity(GameEngine &gameContext, uint32_t entityId, AIComponent &aIComponent) const {
    auto &aiComp                {gameContext.entityMan.getComponent<AIComponent>(entityId)};
    auto entitiesSensing        {getEntitiesSensing(gameContext, entityId)};
    auto dangerousEntityTypes   {aiComp.hostileTo};
    uint32_t aggredBy           {aiComp.aggredBy};
    uint32_t playerId           {gameContext.playerId};

    for(auto &entityIdSensing : entitiesSensing ) {
        bool ifSesingDangerousEntity = entityIdSensing == aggredBy || checkIfDangerousType(gameContext, dangerousEntityTypes, entityIdSensing);

        if(ifSesingDangerousEntity) {
            if (entityIdSensing != playerId)
                return entityIdSensing;
            else {
                auto &atribComp = gameContext.entityMan.getComponent<AtributesComponent>(playerId);
                if (!atribComp.isSecure) {
                    return entityIdSensing;
                } else
                    aIComponent.forceStandBy = true;
            }
        }
    }
    return 0;
}

bool AIStateMachineSystem::checkIfDangerousType(GameEngine &gameContext, std::vector<EntityType> &dangerousEntityType, uint32_t entityIdSensing) const {
    EntityType typeOfEntitySensing {gameContext.getEntity(entityIdSensing).getType()};
    for(auto &dangerousType : dangerousEntityType) {
        if(typeOfEntitySensing == dangerousType) {
            return true;
        }
    }
    return false;
}

std::vector<EntityType> AIStateMachineSystem::getAggressiveToEntitiesType(GameEngine &gameContext, uint32_t entityId) const {

}

std::vector<uint32_t> AIStateMachineSystem::getEntitiesSensing(GameEngine &gameContext, uint32_t entityId) const {
    auto &sensorComponent {gameContext.entityMan.getComponent<BoundingComponent>(entityId)};
    return sensorComponent.entitiesColliding;
}

