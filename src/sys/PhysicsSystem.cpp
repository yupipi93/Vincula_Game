#include "PhysicsSystem.hpp"

#include <tools/Utils.hpp>
#include <math.h>

#include <eng/GameEngine.hpp>

//console.log



//#####################################################
//                == PHYSICS SYSTEM ==
//  - Update the entities' move
//  - Calculate the player's rotation and movement
//
//   Using Componets:
//      - Input     (Read)
//      - velocity  (Read)
//      - Dash      (Read/Write)
//      - Situation (Read/Write)
//#####################################################


PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}


void PhysicsSystem::update(GameEngine &gameContext) const {

    uint32_t playerId = gameContext.playerId;

    //Check the inputComponent to move the player
    playerMovement(playerId, gameContext);

    //Change the SituationComponent of the entities that moves
    updateSituations(gameContext);


    setTargetOfPlayer(gameContext);

    //Change player's rotation
    bool paralyzed = false;
    if (gameContext.entityMan.existsComponent<EffectParalysisComponent>(gameContext.playerId))
        if (gameContext.entityMan.getComponent<EffectParalysisComponent>(gameContext.playerId).paralysisTime)
            paralyzed = true;
    bool dashing = gameContext.entityMan.getComponent<DashComponent>(gameContext.playerId).dashing;

    if (!paralyzed && !dashing)
        updatePlayerRotation(playerId, gameContext);

    gameContext.getWindowFacadeRef().updateEntities(gameContext, {playerId});


    //auto &entitiesToUpdate = gameContext.getEntitiesToUpdate();
    //gameContext.getWindowFacadeRef().updateEntities(gameContext, entitiesToUpdate);
}

void PhysicsSystem::setTargetOfPlayer(GameEngine &gameContext) const {
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    auto &aIMoveTo = gameContext.entityMan.getComponent<AIMoveToComponent>(gameContext.playerId);

    if (playerInput.setingTarget) {
        if (aIMoveTo.targetId) {
            aIMoveTo.targetId = 0;
        } else {
            auto &mouseSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.mouseShapeId);
            auto &mouseCol       = gameContext.entityMan.getComponent<BoundingComponent>(gameContext.mouseShapeId);

            float    minDistance = 999999;
            uint32_t minId       = 0;
            for(size_t i = 0; i < mouseCol.entitiesColliding.size(); i++) {

                uint32_t entityId = mouseCol.entitiesColliding[i];
                EntityType entType = gameContext.getEntity(entityId).getType();


                if(entType == ENEMY || entType == KEYEGG) {
                    SituationComponent &entitySituation = gameContext.entityMan.getComponent<SituationComponent>(entityId);
                    float dist = Utils::calculateDistance(mouseSituation.x, entitySituation.x, mouseSituation.z, entitySituation.z);
                    if (dist <= minDistance) {
                        minDistance = dist;
                        minId = entityId;
                    }
                }

            }

            if (minId) {
                aIMoveTo.targetId = minId;

            }
        }
    }
}

// Get Direction of the animation:
// 0: Not moving
// 1: Fordward
// 2: Fordward-Left
// 3: Left
// 4: Backward-Left
// 5: Backward
// 6: Backward-Right
// 7: Right
// 8: Fordward-Right
uint8_t PhysicsSystem::stablishAnimationDirection(GameEngine &gameContext) const {
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    auto &aIMoveTo = gameContext.entityMan.getComponent<AIMoveToComponent>(gameContext.playerId);

    uint8_t direction = 0;


    if (playerInput.pressingW || playerInput.pressingA || playerInput.pressingS || playerInput.pressingD) {

        uint32_t targetId = gameContext.mouseShapeId;

        if (aIMoveTo.targetId)
            aIMoveTo.targetId;

        auto &playerVelocity = gameContext.entityMan.getComponent<VelocityComponent>(gameContext.playerId);
        float angle =  atan2(playerVelocity.velocityX, playerVelocity.velocityZ);
        angle = Utils::radToDeg(angle);

        auto &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
        auto &targetSituation = gameContext.entityMan.getComponent<SituationComponent>(targetId);

        float uX = targetSituation.x - playerSituation.x;
        float uZ = targetSituation.z - playerSituation.z;

        float angle2 =  atan2(uX, uZ);
        angle2 = Utils::radToDeg(angle2);

        float finalAngle = angle2 - angle;

        if (finalAngle > 180 && finalAngle < 360)
            finalAngle -= 360;
        if (finalAngle < -180 && finalAngle > -360)
            finalAngle += 360;

        if (finalAngle < 22.5 && finalAngle > -22.5) // FORDWARD
            direction = 1;
        else if (finalAngle > 22.5 && finalAngle < 67.5)
            direction = 2;
        else if (finalAngle > 67.5 && finalAngle < 112.5)
            direction = 3;
        else if (finalAngle > 112.5 && finalAngle < 157.5)
            direction = 4;
        else if (finalAngle > 157.5 || finalAngle < -157.5) // BACKWARD
            direction = 5;
        else if (finalAngle > -157.5 && finalAngle < -112.5)
            direction = 6;
        else if (finalAngle > -112.5 && finalAngle < -67.5)
            direction = 7;
        else if (finalAngle > -67.5 && finalAngle < -22.5)
            direction = 8;
    }

    return direction;
}


void PhysicsSystem::updatePlayerRotation(uint32_t &playerId, GameEngine &gameContext) const {
    /*if (!gameContext.entityMan.getComponent<AIMoveToComponent>(playerId).targetId) {
        SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(playerId);
        std::array<float, 3> mouseProjectedCoor = gameContext.getWindowFacadeRef().getProjectedCursor();


        float xdistance = playerSituation.x - mouseProjectedCoor[0];
        float zdistance = playerSituation.z - mouseProjectedCoor[2];

        float angleXZ = abs( Utils::radToDeg(atan2(xdistance, zdistance)) );

        if(xdistance > 0) {
            angleXZ *= -1;
            angleXZ -= 90;
        } else
            angleXZ -= 90;

        playerSituation.rotation = angleXZ;
        }*/
    auto &aIMoveTo = gameContext.entityMan.getComponent<AIMoveToComponent>(gameContext.playerId);

    uint32_t targetId = gameContext.mouseShapeId;

    if (aIMoveTo.targetId)
        aIMoveTo.targetId;


    auto &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
    auto &targetSituation = gameContext.entityMan.getComponent<SituationComponent>(targetId);

    float uX = targetSituation.x - playerSituation.x;
    float uZ = targetSituation.z - playerSituation.z;

    float angle2 =  atan2(uX, uZ);
    angle2 = Utils::radToDeg(angle2);
    angle2 += 180;
    angle2 = 360 - angle2;
    angle2 -= 90;
    playerSituation.rotation = angle2;


}
/*void PhysicsSystem::updatePlayerRotation(uint32_t &playerId, GameEngine &gameContext) const {
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);
    SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(playerId);
    float pAngle = Utils::radToDeg(atan2(playerSituation.x, playerSituation.z));
    std::array<float, 2> mousePos = playerInput.mousePosition;


    float x = mousePos[0] - gameContext.getWindowFacadeRef().windowWidth() / 2;
    float z = mousePos[1] - gameContext.getWindowFacadeRef().windowHeight() / 2;


    float rot = Utils::radToDeg(atan2(x, z));

    if(Utils::isSmartCamera(playerInput)) {
        rot -= pAngle; //TO ADAPT SMART CAMERA
    }

    rot = rot - 90;//TO FIX ROTATE AGLES

    playerSituation.rotation = rot;
}*/


void PhysicsSystem::updateSituations(GameEngine &gameContext) const {
    //Use the deltaTime for interpolation
    const float deltaTime = gameContext.getDeltaTime();
    std::vector<uint32_t> entitiesToUpdate = gameContext.getEntitiesToUpdate();
    for (uint32_t id : entitiesToUpdate) {
        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);
        VelocityComponent &velocity   = gameContext.entityMan.getComponent<VelocityComponent>(id);

        situation.z = situation.z + velocity.velocityZ * deltaTime;
        situation.x = situation.x + velocity.velocityX * deltaTime;
        situation.y = situation.y + velocity.velocityY * deltaTime;
    }

    //Updatate 3D Sounds position
    gameContext.getSoundFacadeRef().add3DSoundToUpdate(entitiesToUpdate);
}


void PhysicsSystem::playerMovement(uint32_t &playerId, GameEngine &gameContext) const {
    VelocityComponent   &playerVelocity     = gameContext.entityMan.getComponent<VelocityComponent>(playerId);
    InputComponent      &playerInput        = gameContext.entityMan.getComponent<InputComponent>(playerId);
    DashComponent       &playerDash         = gameContext.entityMan.getComponent<DashComponent>(playerId);
    DrawableComponent   &playerDrawable     = gameContext.entityMan.getComponent<DrawableComponent>(playerId);
    SituationComponent  &playerSituation    = gameContext.entityMan.getComponent<SituationComponent>(playerId);
    SituationComponent  &cameraSituation    = gameContext.entityMan.getComponent<SituationComponent>(gameContext.cameraId);

    bool moved = false;

    float angle =  atan2(playerSituation.x, playerSituation.z);

    if(!playerDash.dashing) {
        playerDash.timerCoolDown -= gameContext.getDeltaTime();
        if (playerDash.timerCoolDown < 0)
            playerDash.timerCoolDown = 0;
        if(!playerInput.doingAttack) {


            playerVelocity.resetAllVelocities();


            if(playerInput.pressingW) {
                if(Utils::isSmartCamera(playerInput)) {
                    playerVelocity.velocityX += playerVelocity.speed * sin(angle);
                    playerVelocity.velocityZ += playerVelocity.speed * cos(angle);
                    moved = true;
                } else {
                    playerVelocity.velocityZ = playerVelocity.speed;
                    moved = true;
                }
            }
            if(playerInput.pressingS) {
                if(Utils::isSmartCamera(playerInput)) {
                    playerVelocity.velocityX += -playerVelocity.speed * sin(angle);
                    playerVelocity.velocityZ += -playerVelocity.speed * cos(angle);
                    moved = true;
                } else {
                    playerVelocity.velocityZ = -playerVelocity.speed;
                    moved = true;
                }
            }
            if(playerInput.pressingA) {
                if(Utils::isSmartCamera(playerInput)) {
                    playerVelocity.velocityX += playerVelocity.speed * sin(angle - 3.14159 / 2);
                    playerVelocity.velocityZ += playerVelocity.speed * cos(angle - 3.14159 / 2);
                    moved = true;
                } else {
                    playerVelocity.velocityX = -playerVelocity.speed;
                    moved = true;
                }
            }
            if(playerInput.pressingD) {
                if(Utils::isSmartCamera(playerInput)) {
                    playerVelocity.velocityX += playerVelocity.speed * sin(angle + 3.14159 / 2);
                    playerVelocity.velocityZ += playerVelocity.speed * cos(angle + 3.14159 / 2);
                    moved = true;
                } else {
                    playerVelocity.velocityX = playerVelocity.speed;

                    moved = true;
                }
            }

            auto totalVel = sqrt(playerVelocity.velocityX * playerVelocity.velocityX + playerVelocity.velocityZ * playerVelocity.velocityZ);

            if (totalVel > playerVelocity.speed) {
                float over = totalVel * 100. / playerVelocity.speed;
                playerVelocity.velocityX = playerVelocity.velocityX * 100 / over;
                playerVelocity.velocityZ = playerVelocity.velocityZ * 100 / over;
            }

            playerInput.boredTime += gameContext.getDeltaTime();
            if(playerInput.boredTime >= playerInput.maxBoredTime) {
                playerInput.getBored = true;
            }

            if(playerInput.getBored) {
                updateBoredAnimation(gameContext, playerDrawable, playerInput, playerId);
                playerInput.boring     = true;
                playerInput.getBored   = false;
                playerInput.doingDance = false;
            }

            //DANCE
            if(playerInput.dancing) {
                updateDanceAnimation(gameContext, playerDrawable, playerId);
                playerInput.doingDance = true;
                playerInput.boredTime  = 0;
            }


            //DASH
            uint8_t animationDirection = stablishAnimationDirection(gameContext);
            if(moved && playerInput.dashing && playerDash.timerCoolDown <= 0 ) {
                playerDash.activateDash();
                playerVelocity.velocityX = playerVelocity.velocityX * playerDash.velocityFactor;
                playerVelocity.velocityZ = playerVelocity.velocityZ * playerDash.velocityFactor;

                updateDashAnimation(gameContext, playerDrawable, animationDirection, playerId);
                playerInput.doingDance = false;
                playerInput.boring     = false;
                playerInput.boredTime  = 0;
            } else if(((playerInput.doingDance || playerInput.boring) && animationDirection != 0) || (!playerInput.doingDance && !playerInput.boring)) {
                updateMovementAnimation(gameContext, playerDrawable, animationDirection, playerId);
                playerInput.doingDance = false;
                playerInput.boring     = false;
                if(animationDirection != 0) {
                    playerInput.boredTime  = 0;
                }
            }


        }
    } else {
        moved = true;
        playerDash.timerDash -= gameContext.getDeltaTime();
        if (playerDash.timerDash <= 0) {
            playerDash.deactivateDash();
            moved = false;
        }
    }

    if(moved) {
        // Check if we have to update
        gameContext.addEntityToUpdate(playerId);
    }
}

void PhysicsSystem::updateBoredAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, InputComponent &playerInput, uint32_t playerId) const {
    if(playerDrawable.model.compare("./media/player/Animations/bored/bored")) {
        playerDrawable.model = "./media/player/Animations/bored/bored";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    }
}

void PhysicsSystem::updateDashAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint8_t animationDirection, uint32_t playerId) const {
    if(animationDirection == 1) {
        playerDrawable.model = "./media/player/Animations/dashForward/dashForward";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(animationDirection == 2) {
        playerDrawable.model = "./media/player/Animations/dashForwardLeft/dashForwardLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(animationDirection == 3) {
        playerDrawable.model = "./media/player/Animations/dashLeft/dashLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(animationDirection == 4) {
        playerDrawable.model = "./media/player/Animations/dashBackwardLeft/dashBackwardLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }  else if(animationDirection == 5) {
        playerDrawable.model = "./media/player/Animations/dashBackward/dashBackward";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }  else if(animationDirection == 6) {
        playerDrawable.model = "./media/player/Animations/dashBackwardRight/dashBackwardRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }  else if(animationDirection == 7) {
        playerDrawable.model = "./media/player/Animations/dashRight/dashRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }  else if(animationDirection == 8) {
        playerDrawable.model = "./media/player/Animations/dashForwardRight/dashForwardRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }
}

void PhysicsSystem::updateMovementAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint8_t animationDirection, uint32_t playerId) const {
    if(animationDirection == 0 && playerDrawable.model.compare("./media/player/Animations/idleNoWeapon/idleNoWeapon")) {
        playerDrawable.model = "./media/player/Animations/idleNoWeapon/idleNoWeapon";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 12.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 1 && playerDrawable.model.compare("./media/player/Animations/runForward/runForward")) {
        playerDrawable.model = "./media/player/Animations/runForward/runForward";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 2 && playerDrawable.model.compare("./media/player/Animations/runForwardLeft/runForwardLeft")) {
        playerDrawable.model = "./media/player/Animations/runForwardLeft/runForwardLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 3 && playerDrawable.model.compare("./media/player/Animations/runLeft/runLeft")) {
        playerDrawable.model = "./media/player/Animations/runLeft/runLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 4 && playerDrawable.model.compare("./media/player/Animations/runBackwardLeft/runBackwardLeft")) {
        playerDrawable.model = "./media/player/Animations/runBackwardLeft/runBackwardLeft";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 5 && playerDrawable.model.compare("./media/player/Animations/runBackward/runBackward")) {
        playerDrawable.model = "./media/player/Animations/runBackward/runBackward";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 6 && playerDrawable.model.compare("./media/player/Animations/runBackwardRight/runBackwardRight")) {
        playerDrawable.model = "./media/player/Animations/runBackwardRight/runBackwardRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 7 && playerDrawable.model.compare("./media/player/Animations/runRight/runRight")) {
        playerDrawable.model = "./media/player/Animations/runRight/runRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    } else if(animationDirection == 8 && playerDrawable.model.compare("./media/player/Animations/runForwardRight/runForwardRight")) {
        playerDrawable.model = "./media/player/Animations/runForwardRight/runForwardRight";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 40.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    }
}

void PhysicsSystem::updateDanceAnimation(GameEngine &gameContext, DrawableComponent &playerDrawable, uint32_t playerId) const {
    if(playerDrawable.model.compare("./media/player/Animations/dance/dance")) {
        playerDrawable.model = "./media/player/Animations/dance/dance";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, playerId, true);
    }
}



