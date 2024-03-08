#include "AIDefensiveStateSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/AIfunctions.hpp>




//#####################################################
//           == AI DEFENSIVE STATE SYSTEM ==
//  - Get All entities whith AIDefensive Component
//  - Decide by difuse logic wich defensive action has to do
//
//   Using Componets:
//      - AI              (Read - Write)
//      - AiDefensive     (Read - Write)
//#####################################################

AIDefensiveStateSystem::AIDefensiveStateSystem() {}

AIDefensiveStateSystem::~AIDefensiveStateSystem() {}

void AIDefensiveStateSystem::update(GameEngine &gameContext) const {

    auto &allAiInDefensiveState {gameContext.entityMan.getComponents<AIDefensiveComponent>()};

    for(auto &entity : allAiInDefensiveState) {
        auto       &aIComponent         {gameContext.entityMan.getComponent<AIComponent>(entity.id)};
        AIfunctions::setAction(gameContext, aIComponent, aIComponent.defensiveAction);
    }
}