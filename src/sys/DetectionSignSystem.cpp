#include "DetectionSignSystem.hpp"
#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == DETECTION SIGN SYSTEM ==
//
//   Using Componets:
//      -
//      -
//      -
//#####################################################

DetectionSignSystem::DetectionSignSystem() {}

DetectionSignSystem::~DetectionSignSystem() {}


void DetectionSignSystem::update(GameEngine &gameContext) const {
    updateDetectionSignsAnimation(gameContext);
}

void DetectionSignSystem::updateDetectionSignsAnimation(GameEngine &gameContext) const {
    auto &allDetectionSigns = gameContext.entityMan.getComponents<DetectionSignComponent>();

    for(auto &detectionSign : allDetectionSigns) {
        SituationComponent &detectionSignSit  = gameContext.entityMan.getComponent<SituationComponent>(detectionSign.id);

        updateRotation(gameContext, detectionSign, detectionSignSit);
        updateLevitation(gameContext, detectionSign, detectionSignSit);

        gameContext.getWindowFacadeRef().updateEntities(gameContext, {detectionSign.id});
    }
}

void DetectionSignSystem::updateRotation(GameEngine &gameContext, DetectionSignComponent &detectionSign, SituationComponent &detectionSignSit) const {
    detectionSignSit.rotation += detectionSign.rotationSpeed * gameContext.getDeltaTime();
}

void DetectionSignSystem::updateLevitation(GameEngine &gameContext, DetectionSignComponent &detectionSign, SituationComponent &detectionSignSit) const {
    if(detectionSignSit.y <= detectionSign.initialHeight - detectionSign.levitationMargin) {
        detectionSign.signVelocity = detectionSign.levitationSpeed;
    } else if(detectionSignSit.y >= detectionSign.initialHeight + detectionSign.levitationMargin) {
        detectionSign.signVelocity = -detectionSign.levitationSpeed;
    }

    detectionSignSit.y = detectionSignSit.y + detectionSign.signVelocity * gameContext.getDeltaTime();
}


