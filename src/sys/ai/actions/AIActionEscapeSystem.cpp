#include "AIActionEscapeSystem.hpp"
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

AIActionEscapeSystem::AIActionEscapeSystem() {}

AIActionEscapeSystem::~AIActionEscapeSystem() {}

void AIActionEscapeSystem::update(GameEngine &gameContext) const {

    auto &escapeComps       {gameContext.entityMan.getComponents<AIActionEscapeComponent>()};

    for(auto &escapeComp : escapeComps) {
        float objPointX, objPointZ;
        auto &moveToComp        {gameContext.entityMan.getComponent<AIMoveToComponent>(escapeComp.id)};
        auto &velComp           {gameContext.entityMan.getComponent<VelocityComponent>(escapeComp.id)};
        velComp.speed = velComp.speedUpped;

        if(!moveToComp.targetId) {
            calculateEscapePosition(gameContext, escapeComp.id, objPointX, objPointZ);
            moveToComp.targetId = AIfunctions::createTarget(gameContext,  moveToComp.id,  objPointX,  objPointZ);
        } else {
            calculateEscapePosition(gameContext, escapeComp.id, objPointX, objPointZ);
            AIfunctions::moveTarget(gameContext,  moveToComp.targetId,  objPointX,  objPointZ);
        }


    }
}


void AIActionEscapeSystem::calculateEscapePosition(GameEngine &gameContext, uint32_t entityID, float &objPointX, float &objPointZ) const {
    auto &aiDefensive       {gameContext.entityMan.getComponent<AIDefensiveComponent>(entityID)};
    if (gameContext.entityMan.existsComponent<SituationComponent>(aiDefensive.targetId)) {
        auto &targetSituation   {gameContext.entityMan.getComponent<SituationComponent>(aiDefensive.targetId)};
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
}


