#include "TutorialSystem.hpp"

#include <tools/Utils.hpp>
#include <eng/GameEngine.hpp>


//console.log

//#####################################################
//                == TUTORIAL SYSTEM ==
//  -
//
//#####################################################


TutorialSystem::TutorialSystem() {}

TutorialSystem::~TutorialSystem() {}

void TutorialSystem::update(GameEngine &gameContext) const {

    if(gameContext.tutorialPhases < 8) {
        auto &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(gameContext.playerId);
        if(playerInteraction.dialogueInteracting) {
            gameContext.tutorialPhases++;
        }
    }


}