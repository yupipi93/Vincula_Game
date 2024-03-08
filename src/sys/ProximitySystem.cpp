#include "ProximitySystem.hpp"

#include <eng/GameEngine.hpp>


//#####################################################
//                == PROXIMITY SYSTEM ==
//  - Get all entities with proximity
//
//   Using Componets:
//      - Proximity     (Read - Write)
//      - MoveTo        (Read - Write)
//      - Sensor        (Read)
//#####################################################


ProximitySystem::ProximitySystem() {}

ProximitySystem::~ProximitySystem() {}


void ProximitySystem::update(GameEngine &gameContext) const {
    auto &allProximityComponents = gameContext.entityMan.getComponents<ProximityComponent>();
    for(auto &proximityComp : allProximityComponents) {
        processEntitiesSensing(gameContext, proximityComp);
    }

}

void ProximitySystem::processEntitiesSensing(GameEngine &gameContext, ProximityComponent &proximityComp) const {
    uint32_t entityId = proximityComp.id;
    std::vector<EntityType> &reactiveTypes = proximityComp.attractiveTo;
    auto &entitiesCollidingIds = gameContext.entityMan.getComponent<BoundingComponent>(entityId).entitiesColliding;


    for(auto &collidingId : entitiesCollidingIds) {
        // Little patch for monkeys
        bool throwedByThatMonkey = false;
        if (gameContext.entityMan.existsComponent<AttackComponent>(entityId))
            if (gameContext.entityMan.getComponent<AttackComponent>(entityId).attackerId == collidingId)
                throwedByThatMonkey = true;

        if(!throwedByThatMonkey && isAttractiveType(gameContext, collidingId, reactiveTypes)) {
            setTarget(gameContext, entityId, collidingId);
            setNewSpeed(gameContext, entityId);
        }
    }
}


bool ProximitySystem::isAttractiveType(GameEngine &gameContext, uint32_t entiyId, std::vector<EntityType> &reactiveTypes) const {
    EntityType entityType = gameContext.getEntity(entiyId).getType();
    for(EntityType &reactiveType : reactiveTypes) {
        if(entityType == reactiveType) {
            return true;
        }
    }
    return false;
}

void ProximitySystem::setTarget(GameEngine &gameContext, uint32_t entityId, uint32_t aimId) const {
    auto &moveToComp = gameContext.entityMan.getComponent<AIMoveToComponent>(entityId);
    moveToComp.targetId = aimId;
}

void ProximitySystem::setNewSpeed(GameEngine &gameContext, uint32_t entityId) const {
    auto &velocityComp = gameContext.entityMan.getComponent<VelocityComponent>(entityId);
    velocityComp.speed = 70;
}

