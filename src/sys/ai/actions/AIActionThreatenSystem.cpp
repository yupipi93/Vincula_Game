#include "AIActionThreatenSystem.hpp"
#include <eng/GameEngine.hpp>

#include <tools/AIfunctions.hpp>


//#####################################################
//     == AI ACTION THREATEN SYSTEM [OFFENSIVE] ==
//  - Get All entities whith AiThreaten Component
//  - Get each offensive component
//  - Get holstile entity id from Offensive Componet
//  - Set Target in MoveTo Component
//
//   Using Componets:
//      - MoveTo            (Read - Write)
//      - AiThreaten        (Read)
//      - AiOffensive       (Read)
//#####################################################

AIActionThreatenSystem::AIActionThreatenSystem() {}

AIActionThreatenSystem::~AIActionThreatenSystem() {}


void AIActionThreatenSystem::update(GameEngine &gameContext) const {

    auto &allThreatens {gameContext.entityMan.getComponents<AIActionThreatenComponent>()};

    for(auto &threatenComp : allThreatens) {
        uint32_t entityId       = threatenComp.id;
        auto &aiOffensive       {gameContext.entityMan.getComponent<AIOffensiveComponent>(entityId)};
        auto &moveToComp        {gameContext.entityMan.getComponent<AIMoveToComponent>(entityId)};
        if(gameContext.entityMan.existsComponent<SituationComponent>(aiOffensive.targetId)) {
            auto &targetSituation   {gameContext.entityMan.getComponent<SituationComponent>(aiOffensive.targetId)};

            //Delete last waypoint
            gameContext.eraseEntityByID(moveToComp.targetId);

            moveToComp.targetId = AIfunctions::createTarget(gameContext,  moveToComp.id,  targetSituation.x,  targetSituation.z);
        }




    }

}

