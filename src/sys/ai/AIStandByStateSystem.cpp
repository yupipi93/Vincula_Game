#include "AIStandByStateSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/AIfunctions.hpp>

//console.log

//#####################################################
//           == AI STANDBY STATE SYSTEM ==
//  - Get All entities whith AiStandBy Component
//  - Decide to Difise Action to execute
//
//   Using Componets:
//      - AI            (Read - Write)
//      - AiStandBy     (Read - Write)
//#####################################################

AIStandByStateSystem::AIStandByStateSystem() {}

AIStandByStateSystem::~AIStandByStateSystem() {}

void AIStandByStateSystem::update(GameEngine &gameContext) const {

    auto &allAiInStandByState {gameContext.entityMan.getComponents<AIStandByComponent>()};

    for(auto &entity : allAiInStandByState) {
        auto &aIComponent {gameContext.entityMan.getComponent<AIComponent>(entity.id)};
        auto &standByAction = aIComponent.standByAction;
        //Difuse Logic to decide Stand By Action

        //WANDER
        AIfunctions::setAction(gameContext, aIComponent, standByAction);


    }

}









/*
        aIComponent.componentName = typeid(AIActionWanderComponent).name();
        //Set Wander
        if(!aIComponent.componentName.compare(typeid(AIActionWanderComponent).name())) {
            AIfunctions::removeAiAction(gameContext, entity.id);
            gameContext.entityMan.createComponent<AIActionWanderComponent>(entity.id);
            aIComponent.componentName = typeid(AIActionWanderComponent).name();

        }
*/