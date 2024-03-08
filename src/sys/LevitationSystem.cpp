#include "LevitationSystem.hpp"
#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == LEVITATION SYSTEM ==
//
//   Using Componets:
//      -
//      -
//      -
//#####################################################

LevitationSystem::LevitationSystem() {}

LevitationSystem::~LevitationSystem() {}


void LevitationSystem::update(GameEngine &gameContext) const {
    auto &allLevitations = gameContext.entityMan.getComponents<LevitationComponent>();

    for(auto &levitation : allLevitations) {
        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(levitation.id);

        updateLevitation(gameContext, levitation, situation);

        gameContext.getWindowFacadeRef().updateEntities(gameContext, {levitation.id});
    }
}

void LevitationSystem::updateLevitation(GameEngine &gameContext, LevitationComponent &levitation, SituationComponent &situation) const {
    if(situation.y <= levitation.initialHeight) {
        levitation.signVelocity = levitation.levitationSpeed;
    } else if(situation.y >= levitation.initialHeight + levitation.levitationMargin) {
        levitation.signVelocity = -levitation.levitationSpeed;
    }

    situation.y = situation.y + levitation.signVelocity * gameContext.getDeltaTime();
}


