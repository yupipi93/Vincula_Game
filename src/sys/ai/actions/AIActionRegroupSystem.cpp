#include "AIActionRegroupSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <tools/AIfunctions.hpp>
//console.log



//#####################################################
//     == AI ACTION REGROUP SYSTEM [DEFENSIVE] ==
//  - Get all entities in Regroup state
//  - Search other entity whith de same sub-type
//  - Share the original hostile target
//
//   Using Componets:
//  - ???????????
//#####################################################

AIActionRegroupSystem::AIActionRegroupSystem() {}

AIActionRegroupSystem::~AIActionRegroupSystem() {}

void AIActionRegroupSystem::update(GameEngine &gameContext) const {

    auto &entitiesInRegroupAction {gameContext.entityMan.getComponents<AIActionRegroupComponent>()};
    for(auto &regroupComp : entitiesInRegroupAction) {
        uint32_t entityId = regroupComp.id;
        auto &defensiveComp {gameContext.entityMan.getComponent<AIDefensiveComponent>   (entityId)};
        auto &aiComp        {gameContext.entityMan.getComponent<AIComponent>            (entityId)};
        auto &moveToComp    {gameContext.entityMan.getComponent<AIMoveToComponent>      (entityId)};

        aiComp.currentAlertState = 1000;
        uint32_t friendId = defensiveComp.friendId;

        if(friendId) {

            moveToTarget(gameContext, moveToComp, friendId);
            if(isSensingFriend(gameContext, entityId, friendId)) {

                shareHostileTargetWithFriend(gameContext, defensiveComp, friendId);

                entityTurnAgressive(gameContext, entityId);
                defensiveComp.friendId = 0;
                defensiveComp.friendConfirmed = true;

            }


        } else {
            if(!defensiveComp.friendConfirmed) {
                defensiveComp.friendId = getSatandByFriend(gameContext, entityId);
            } else {
                aiComp.forceCombat = true;
            }
        }


    }

}





uint32_t AIActionRegroupSystem::getSatandByFriend(GameEngine &gameContext, uint32_t entityId)const {
    GameObjectType entitySubType = gameContext.entityMan.getEntity(entityId).getGameObjectType();
    SituationComponent &enemySit = gameContext.entityMan.getComponent<SituationComponent>(entityId);

    uint32_t closerFriend = 0;
    float    minDistance = 999999;
    auto &allEnemies {gameContext.entityMan.getComponents<AIComponent>()};
    for (auto &enemy : allEnemies) {
        if(isSameSubType(gameContext, entitySubType, enemy.id) && isInStandBy(gameContext, enemy.id)) {
            SituationComponent &friendSit = gameContext.entityMan.getComponent<SituationComponent>(enemy.id);

            float dist = Utils::calculateDistance(enemySit.x, friendSit.x, enemySit.z, friendSit.z);
            if (dist <= minDistance) {
                minDistance = dist;
                closerFriend = enemy.id;
            }

        }
    }

    return closerFriend;
}


void AIActionRegroupSystem::shareHostileTargetWithFriend(GameEngine &gameContext, AIDefensiveComponent &defensiveComp, uint32_t friendId) const {
    uint32_t entityId = defensiveComp.id;
    uint32_t hostileEntityId = defensiveComp.targetId;

    if(gameContext.entityMan.existsComponent<AIStandByComponent>(friendId)) {

        auto &aiStandByComp = gameContext.entityMan.getComponent<AIStandByComponent>(friendId);
        auto &aiComp        = gameContext.entityMan.getComponent<AIComponent>(friendId);

        aiStandByComp.targetId      = hostileEntityId;
        aiComp.aggredBy             = hostileEntityId;
        aiComp.defaultAlertState    = 9000;
        aiComp.forceCombat          = true;
    }


}



void AIActionRegroupSystem::entityTurnAgressive(GameEngine &gameContext, uint32_t entityId) const {
    auto &fuzzyComp {gameContext.entityMan.getComponent<AIDifuseStateComponent>(entityId)};
    auto &aiComp    {gameContext.entityMan.getComponent<AIComponent>(entityId)};
    aiComp.defaultAlertState    = 9000;

    fuzzyComp.healthyX0         = 0;
    fuzzyComp.healthyX1         = 0;

    fuzzyComp.dyingX0           = 0;
    fuzzyComp.dyingX1           = 0;
    fuzzyComp.injuredX0         = 0;
    fuzzyComp.injuredX1         = 0;
    fuzzyComp.injuredX2         = 0;
    fuzzyComp.injuredX3         = 0;





}

bool AIActionRegroupSystem::isSameSubType(GameEngine &gameContext, GameObjectType entitySubType, uint32_t friendId) const {
    GameObjectType friendSubType = gameContext.entityMan.getEntity(friendId).getGameObjectType();
    if(entitySubType == friendSubType) {
        return true;
    }
    return false;
}

bool AIActionRegroupSystem::isSensingFriend(GameEngine &gameContext, uint32_t entityId, uint32_t friendId) const {
    auto &entitiesDetected {gameContext.entityMan.getComponent<CollisionComponent>(entityId).entitiesColliding};
    for(auto &entityDetectedId : entitiesDetected) {
        if(entityDetectedId == friendId) {
            return true;
        }
    }
    return false;
}

bool AIActionRegroupSystem::isInStandBy(GameEngine &gameContext, uint32_t entityId) const {
    return gameContext.entityMan.existsComponent<AIStandByComponent>(entityId);
}


void AIActionRegroupSystem::moveToTarget(GameEngine &gameContext, AIMoveToComponent &moveToComp, uint32_t targetId) const {
    uint32_t entityId = moveToComp.id;


    if(!moveToComp.targetId) {

        if(gameContext.entityMan.existsComponent<SituationComponent>(targetId)) {

            auto &targetSitComp     {gameContext.entityMan.getComponent<SituationComponent>(targetId)};
            moveToComp.targetId = AIfunctions::createTarget(gameContext,  entityId,  targetSitComp.x,  targetSitComp.z);


        } else {
            gameContext.entityMan.getComponent<AIDefensiveComponent>(entityId).friendId = 0;
        }

    }
}

