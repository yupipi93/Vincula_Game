#include "CompassSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

//TEMP
//console.log


//#####################################################
//              == COMPASS SYSTEM ==
//
//
//   Using Componets:
//      -
//      -
//      -
//#####################################################

const float COMPASS_APPARITION_HEIGHT = 50;
const float ANIMATION_VELOCITY = 0.4;

const float COMPASS_MIN_SIZE = 0.9;
const float COMPASS_MAX_SIZE = 1.1;

const float COMPASS_MIN_ROTATE = 0.9;
const float COMPASS_MAX_ROTATE = 1.1;


CompassSystem::CompassSystem() {}

CompassSystem::~CompassSystem() {}

void CompassSystem::update(GameEngine &gameContext) const {

    InteractionComponent &interaction = gameContext.entityMan.getComponent<InteractionComponent>(gameContext.playerId);
    checkTakingPalantir(gameContext, interaction);
    InputComponent &input = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    SituationComponent   &playerSit   = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
    if(isCallCompass(gameContext) | checkDropingPalantir(gameContext, input, playerSit))
        createCompass(gameContext);

    updatePalantirRotation(gameContext);

    if(gameContext.compassId != 0) {

        SituationComponent   &compassSit  = gameContext.entityMan.getComponent<SituationComponent>(gameContext.compassId);
        CompassComponent     &compassComp = gameContext.entityMan.getComponent<CompassComponent>(gameContext.compassId);


        compassComp.lifeTime -= gameContext.getDeltaTime();

        updateCompassPosition(gameContext,  compassSit, playerSit);
        updateCompassLookAtPoint(gameContext, compassSit);
        updateCompassAnimation(gameContext, compassSit, compassComp);

        //gameContext.entityMan.addEntityToUpdate(gameContext.compassId);
        gameContext.getWindowFacadeRef().updateEntities(gameContext, {gameContext.compassId});

        if(compassComp.lifeTime <= 0) {
            removeCompass(gameContext);
        }
    }
}

void CompassSystem::updatePalantirRotation(GameEngine &gameContext) const {
    uint32_t palantirId = gameContext.palantirId;
    if (palantirId) {
        auto &palSit = gameContext.entityMan.getComponent<SituationComponent>(palantirId);
        palSit.rotation -= 100 * gameContext.getDeltaTime();
    }

}
void CompassSystem::checkTakingPalantir(GameEngine &gameContext, InteractionComponent &playerInteraction) const {
    if(playerInteraction.palantirInteracting != 0) { //Esta interactuando con un arma
        gameContext.eraseEntityByID(playerInteraction.palantirInteracting);
        gameContext.palantirId = 0;
    }
}

bool CompassSystem::checkDropingPalantir(GameEngine &gameContext, InputComponent &playerInput, SituationComponent &playerSit) const {
    if(playerInput.dropingPalantir) { //Esta interactuando con un arma
        float x = playerSit.x;
        float y = playerSit.y;
        float z = playerSit.z;
        float distance = 15;
        float angle = Utils::degToRad(playerSit.rotation);
        x += distance * cos(angle);
        z += distance * sin(angle);
        gameContext.compassPointsTo = gameContext.entityMan.createPalantir(gameContext, x, y, z);
        return true;
    }
    return false;
}

bool CompassSystem::isCallCompass(GameEngine &gameContext) const {
    InputComponent      &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    return playerInput.callingCompass;

}

void CompassSystem::createCompass(GameEngine &gameContext) const {

    if(gameContext.compassId == 0) {
        SituationComponent &playerSit   = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
        gameContext.entityMan.createCompass(gameContext, playerSit.x, COMPASS_APPARITION_HEIGHT, playerSit.z, 0);
    }

}

void CompassSystem::updateCompassAnimation(GameEngine &gameContext, SituationComponent &compassSit, CompassComponent &compassComp) const {
    DrawableComponent   &compassDrawable   = gameContext.entityMan.getComponent<DrawableComponent>(gameContext.compassId);


    if(compassComp.lifeTime > 0.4) {

        updateScale(gameContext, compassComp, compassDrawable);
        updateRotation(gameContext, compassComp, compassSit);


    } else {
        //FINAL ANIMATION
        if(compassDrawable.scaleX > 0) {
            compassDrawable.scaleX -= 3 * gameContext.getDeltaTime();
            compassDrawable.scaleZ -= 3 * gameContext.getDeltaTime();
        } else {
            compassDrawable.scaleY = 0;
        }
    }
}

void CompassSystem::updateScale(GameEngine &gameContext, CompassComponent &compassComp, DrawableComponent &compassDrawable) const {
    if(compassComp.increaseAnimation) {
        if(compassDrawable.scaleX >= COMPASS_MAX_SIZE) {
            compassComp.increaseAnimation = false;
        }
    } else {
        if(compassDrawable.scaleX <= COMPASS_MIN_SIZE) {
            compassComp.increaseAnimation = true;

        }
    }

    //INCREMENT SCALE
    if(compassComp.increaseAnimation) {
        compassDrawable.scaleX += ANIMATION_VELOCITY * gameContext.getDeltaTime();
        compassDrawable.scaleZ += ANIMATION_VELOCITY * gameContext.getDeltaTime();
        //DECREASE SCALE
    } else {
        compassDrawable.scaleX -= ANIMATION_VELOCITY * gameContext.getDeltaTime();
        compassDrawable.scaleZ -= ANIMATION_VELOCITY * gameContext.getDeltaTime();
    }
}

void CompassSystem::updateRotation(GameEngine &gameContext, CompassComponent &compassComp, SituationComponent &compassSit) const {
    uint32_t pointTo = gameContext.compassPointsTo;
    if (gameContext.entityMan.existsComponent<SituationComponent>(pointTo)) {
        //INCREMENT ROTATE
        if(compassComp.increaseAnimation) {
            compassSit.rotation += 100 * gameContext.getDeltaTime();
            //DECREASE ROTATE
        } else {
            compassSit.rotation -= 100 * gameContext.getDeltaTime();
        }
    } else {
        compassSit.rotation += 500 * gameContext.getDeltaTime();
    }
}


void CompassSystem::updateCompassLookAtPoint(GameEngine &gameContext, SituationComponent &compassSit) const {
    //CompassPosition - PointsToSituation
    uint32_t pointTo = gameContext.compassPointsTo;
    if (gameContext.entityMan.existsComponent<SituationComponent>(pointTo)) {
        auto &situationPointed = gameContext.entityMan.getComponent<SituationComponent>(pointTo);
        float xdistance = compassSit.x - situationPointed.x;
        float zdistance = compassSit.z - situationPointed.z;

        float angleXZ = Utils::radToDeg( abs(atan2(xdistance, zdistance)) );
        //compassSit.rotation = angleXZ;

        if(compassSit.x < situationPointed.x) {
            compassSit.rotation = angleXZ - 180;
        } else {
            compassSit.rotation = 180 - angleXZ;
        }
    }
}

void CompassSystem::updateCompassPosition(GameEngine &gameContext, SituationComponent &compassSit, SituationComponent &playerSit) const {
    compassSit.x = playerSit.x;
    //compassSit.y = COMPASS_APPARITION_HEIGHT;
    compassSit.z = playerSit.z;

    //TDESCOMENT TO COMPASS ORBIT PLAYER
    /*  uint8_t distance = 20;
        float angle =  atan2(playerSit.x, playerSit.z);
        compassSit.x   = playerSit.x +(distance *sin(angle));
        compassSit.y   = compassSit.y;
        compassSit.z   = playerSit.z +(distance *cos(angle));
    */


}

void CompassSystem::removeCompass(GameEngine &gameContext) const {
    gameContext.eraseEntityByID(gameContext.compassId);
    gameContext.compassId = 0;

}





