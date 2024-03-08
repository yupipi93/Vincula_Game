#include "AIActionWanderSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/AIfunctions.hpp>



//#####################################################
//      == AI ACTION WANDER SYSTEM [STANDBY] ==
//  - Get All entities whith Wander Component
//	- Remove Target
//  - If target and sleepTimer == 0
//  - Set random target
//  - Set random sllepTimer
//
//   Using Componets:
//      - Wander                (Read - Write)
//      - MoveTo                (Read - Write)
//#####################################################

constexpr uint8_t MAX_SEEP_TIME = 100;
constexpr uint8_t MAX_DISTANCE 	= 100;


AIActionWanderSystem::AIActionWanderSystem() {
 //srand (time(NULL));
}

AIActionWanderSystem::~AIActionWanderSystem() {}


void AIActionWanderSystem::update(GameEngine &gameContext) const {
    
    auto &wanderComp {gameContext.entityMan.getComponents<AIActionWanderComponent>()};

    for(auto &wanderComp : wanderComp) {
        uint32_t entityId = wanderComp.id;
        auto &moveToComp {gameContext.entityMan.getComponent<AIMoveToComponent>(entityId)};

        if(!moveToComp.targetId) {
            if(!wanderComp.sleepTime) {

                setRandomTarget(gameContext, moveToComp, entityId);
                setRandomSleepTime(wanderComp);

            } else {
                wanderComp.sleepTime--;
            }
        }

    }

}

void AIActionWanderSystem::setRandomTarget(GameEngine &gameContext, AIMoveToComponent &moveToComp,  uint32_t entityId) const {
    auto &situationComp 	{gameContext.entityMan.getComponent<SituationComponent>(entityId)};

    //rand between -MAX_DISTANCE and MAX_DISTANCE
    int8_t randX = rand() % (MAX_DISTANCE*2+1) + (-MAX_DISTANCE); 
    int8_t randZ = rand() % (MAX_DISTANCE*2+1) + (-MAX_DISTANCE);

    moveToComp.targetId = AIfunctions::createTarget(gameContext,  moveToComp.id,  situationComp.x + randX,  situationComp.z + randZ);

}


void AIActionWanderSystem::setRandomSleepTime(AIActionWanderComponent &wanderComp) const {
    wanderComp.sleepTime = rand() % MAX_SEEP_TIME;
}