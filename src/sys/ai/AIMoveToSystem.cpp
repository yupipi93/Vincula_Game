#include "AIMoveToSystem.hpp"
#include <tools/Utils.hpp>
#include <math.h>

#include <eng/GameEngine.hpp>

//#####################################################
//                == AI MOVETO SYSTEM ==
//  - Get All Entities whith MoveTo component
//  - Get target if IAentity had
//  - Set angle to target
//  - Set speed to follow target
//
//   Using Componets:
//      - Situation  (Read - Write)
//      - Velocity   (Read)
//      - MoveTo     (Read)
//#####################################################

AIMoveToSystem::AIMoveToSystem() {}

AIMoveToSystem::~AIMoveToSystem() {}

void AIMoveToSystem::update(GameEngine &gameContext) const {

    //removeEmptyWaypoints(gameContext);
    processEntities2D(gameContext);
    processEntities3D(gameContext);

}


void AIMoveToSystem::processEntities2D(GameEngine &gameContext) const {
    auto &allMoveToEnttites {gameContext.entityMan.getComponents<AIMoveToComponent>()};
    for(auto &moveToComp : allMoveToEnttites) {
        auto    entityId                {moveToComp.id};
        if (entityId != gameContext.playerId) {
            auto    targetId                {moveToComp.targetId};

            if(targetId) {
                auto    &entitySituation        {gameContext.entityMan.getComponent<SituationComponent>(entityId)};
                auto    &entityVeloc            {gameContext.entityMan.getComponent<VelocityComponent>(entityId)};
                auto    &targetSituation        {gameContext.entityMan.getComponent<SituationComponent>(targetId)};

                float   angleXZ                 {Utils::angleToTarget(entitySituation, targetSituation)};


                setSpeedDirectionToEntity2D(entityVeloc, angleXZ);

                entitySituation.rotation = Utils::radToDeg(angleXZ);

                //Update Entity
                gameContext.addEntityToUpdate(entityId);



            }
        }

    }
}

void AIMoveToSystem::processEntities3D(GameEngine &gameContext) const {
    auto &allMoveToEnttites {gameContext.entityMan.getComponents<AIMoveTo3DComponent>()};
    for(auto &moveToComp : allMoveToEnttites) {
        auto    entityId                {moveToComp.id};
        auto    targetId                {moveToComp.targetId};

        if(targetId) {
            float   targetSizeY             {gameContext.entityMan.getComponent<CollisionComponent>(targetId).sizeY};
            auto    &entityVeloc            {gameContext.entityMan.getComponent<VelocityComponent>(entityId)};
            auto    &entitySituation        {gameContext.entityMan.getComponent<SituationComponent>(entityId)};
            auto    &targetSituation        {gameContext.entityMan.getComponent<SituationComponent>(targetId)};

            float   angleXZ                 {Utils::angleToTarget(entitySituation, targetSituation)};
            float   angleY                  {getAngleToTargetInY(entitySituation, targetSituation, targetSizeY / 2)};

            setSpeedDirectionToEntity3D(entityVeloc, angleXZ, angleY);

            //Update Entity
            gameContext.addEntityToUpdate(entityId);

        }

    }
}

float AIMoveToSystem::getAngleToTargetInY(SituationComponent &entitySituation, SituationComponent &targetSituation, float targetSizeY) const {

    float angle {0};
    float distanceInY   = entitySituation.y - targetSizeY; //Distancia en Y de la moneda - tamaño de la base
    float distanceInXZ  = Utils::calculateDistance(entitySituation.x, targetSituation.x, entitySituation.z, targetSituation.z);

    angle = atan(distanceInXZ / distanceInY);

    return angle;

}


void AIMoveToSystem::setSpeedDirectionToEntity2D(VelocityComponent  &entityVeloc, float angleXZ) const {
    float entitySpeed {entityVeloc.speed};

    float   entityX         {entitySpeed * cos(angleXZ)};
    float   entityZ         {entitySpeed * sin(angleXZ)};

    entityVeloc.velocityX = entityX;
    entityVeloc.velocityZ = entityZ;

}

void AIMoveToSystem::setSpeedDirectionToEntity3D(VelocityComponent  &entityVeloc, float angleXZ, float angleY) const {
    float entitySpeed {entityVeloc.speed};
    float maxSpeedXZ = entitySpeed * angleY / (3.14159 / 2);

    float   entityX         {maxSpeedXZ * cos(angleXZ)};
    float   entityY         {-(entitySpeed - maxSpeedXZ) *cos(angleY)};
    float   entityZ         {maxSpeedXZ * sin(angleXZ)};

    entityVeloc.velocityX = entityX;
    entityVeloc.velocityY = entityY;
    entityVeloc.velocityZ = entityZ;


}


