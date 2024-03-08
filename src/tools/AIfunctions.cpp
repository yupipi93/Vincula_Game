#include "AIfunctions.hpp"
#include <eng/GameEngine.hpp>
#include <com/ComponentsIncluder>


AIfunctions::AIfunctions() {}

void AIfunctions::turnToTarget(GameEngine &gameContext, float angle, SituationComponent &situation) {
    situation.rotation = angle - 180;
    gameContext.getWindowFacadeRef().updateEntities(gameContext, {situation.id});
}

void AIfunctions::setAction(GameEngine &gameContext, AIComponent &aIComponent, AiAction action) {
    uint32_t entityId = aIComponent.id;

    if(aIComponent.lastAction != action) {
        if (aIComponent.lastAction == LUNGE) {
            auto &lungeComp  = gameContext.entityMan.getComponent<AIActionLungeComponent>(entityId);

            if (lungeComp.attackId) {
                if (gameContext.entityMan.existsComponent<AttackComponent>(lungeComp.attackId)) {
                    auto &attack = gameContext.entityMan.getComponent<AttackComponent>(lungeComp.attackId);
                    attack.lifeTime = 0;
                }
                lungeComp.attackId = 0;
            }
        }

        if (aIComponent.lastAction == LUNGE || aIComponent.lastAction == ESCAPE) {
            auto &velocComp  = gameContext.entityMan.getComponent<VelocityComponent>(entityId);
            velocComp.speed = velocComp.speedNormal;
        }
        
        aIComponent.lastAction = action;
        removeAiAction(gameContext, entityId);
        removeTarget(gameContext, entityId);


        switch(action) {
        case WANDER:
            gameContext.entityMan.createComponent<AIActionWanderComponent>(entityId);
            break;

        case THREATEN:
            gameContext.entityMan.createComponent<AIActionThreatenComponent>(entityId);
            break;

        case ESCAPE:
            gameContext.entityMan.createComponent<AIActionEscapeComponent>(entityId);
            break;

        case MELEE:
            gameContext.entityMan.createComponent<AIActionMeleeComponent>(entityId);
            break;

        case LUNGE:
            gameContext.entityMan.createComponent<AIActionLungeComponent>(entityId);
            break;

        case DISTANCE:
            gameContext.entityMan.createComponent<AIActionDistanceComponent>(entityId);
            break;

        case KEEPDISTANCE:
            gameContext.entityMan.createComponent<AIActionKeepDistanceComponent>(entityId);
            break;

        case REGROUP:
            gameContext.entityMan.createComponent<AIActionRegroupComponent>(entityId);
            break;

        case NO_ACTION:
            break;
        default:
            break;

        }

    }
}


void AIfunctions::removeAiAction(GameEngine &gameContext, uint32_t entityId) {
    //Remember all ActionComponets
    gameContext.entityMan.eraseComponent<AIActionWanderComponent>       (entityId);
    gameContext.entityMan.eraseComponent<AIActionThreatenComponent>     (entityId);
    gameContext.entityMan.eraseComponent<AIActionEscapeComponent>       (entityId);
    gameContext.entityMan.eraseComponent<AIActionMeleeComponent>        (entityId);
    gameContext.entityMan.eraseComponent<AIActionLungeComponent>        (entityId);
    gameContext.entityMan.eraseComponent<AIActionDistanceComponent>     (entityId);
    gameContext.entityMan.eraseComponent<AIActionKeepDistanceComponent> (entityId);
    gameContext.entityMan.eraseComponent<AIActionRegroupComponent>      (entityId);
}

void AIfunctions::removeTarget(GameEngine &gameContext, uint32_t entityId) {
    auto &moveToComp = gameContext.entityMan.getComponent<AIMoveToComponent>(entityId);
    gameContext.eraseEntityByID(moveToComp.targetId);
    moveToComp.targetId = 0;
}





uint32_t AIfunctions::createTarget(GameEngine &gameContext, uint32_t entityId, float posX, float posZ) {
    uint32_t wayPointId = gameContext.entityMan.createWaypoint(gameContext, posX, 0.f, posZ, 0.f);
    gameContext.entityMan.getComponent<WaypointComponent>(wayPointId).target_of = entityId;
    return wayPointId;

}

void AIfunctions::moveTarget(GameEngine &gameContext, uint32_t wayPointId, float posX, float posZ) {
    EntityType entityType = gameContext.entityMan.getEntity(wayPointId).getType();
    if(entityType == WAYPOINT) {
        auto &wayPoint  = gameContext.entityMan.getComponent<WaypointComponent>(wayPointId);
        auto &sitComp   = gameContext.entityMan.getComponent<SituationComponent>(wayPointId);
        
        // PATHFINDIG LOGIC

        sitComp.x = posX;
        sitComp.z = posZ;

    }


}
