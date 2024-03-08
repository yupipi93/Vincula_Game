#include "AIActionKeepDistanceSystem.hpp"
#include <math.h>
#include <tools/Utils.hpp>
#include <tools/AIfunctions.hpp>
#include <enum/AiAction.hpp>
#include <enum/EntityType.hpp>




//#####################################################
//     == AI ACTION ESCAPE SYSTEM [DEFENSIVE] ==
//  - Get All entities whith DEFENSIVE Component
//  - Make this entity to run away from the player
//
//   Using Componets:
//  - Situation Component       (read)
//
//#####################################################

AIActionKeepDistanceSystem::AIActionKeepDistanceSystem() {}

AIActionKeepDistanceSystem::~AIActionKeepDistanceSystem() {}

void AIActionKeepDistanceSystem::update(GameEngine &gameContext) const {

    auto &keepDisComps       {gameContext.entityMan.getComponents<AIActionKeepDistanceComponent>()};

    for(auto &keepDisComp : keepDisComps) {

        float objPointX, objPointZ;
        auto &moveToComp        {gameContext.entityMan.getComponent<AIMoveToComponent>(keepDisComp.id)};
        auto &velComp           {gameContext.entityMan.getComponent<VelocityComponent>(keepDisComp.id)};
        velComp.speed = velComp.speedNormal;

        if(!moveToComp.targetId) {
            calculateKeepDistancePosition(gameContext, keepDisComp.id, objPointX, objPointZ);
            moveToComp.targetId = AIfunctions::createTarget(gameContext,  moveToComp.id,  objPointX,  objPointZ);
        }

    }
}


void AIActionKeepDistanceSystem::calculateKeepDistancePosition(GameEngine &gameContext, uint32_t entityID, float &objPointX, float &objPointZ) const {
    auto &aiOffensive       {gameContext.entityMan.getComponent<AIOffensiveComponent>(entityID)};
    auto &targetSituation   {gameContext.entityMan.getComponent<SituationComponent>(aiOffensive.targetId)};
    auto &entitySituation   {gameContext.entityMan.getComponent<SituationComponent>(entityID)};

    float vectX = entitySituation.x - targetSituation.x;
    float vectZ = entitySituation.z - targetSituation.z;

    if(entitySituation.x < 0) {
        vectX *= -1;
    }
    if(entitySituation.z < 0) {
        vectZ *= -1;
    }

    objPointX = vectX * entitySituation.x;
    objPointZ = vectZ * entitySituation.z;
}