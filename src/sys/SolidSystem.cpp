#include "SolidSystem.hpp"

//console.log

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

//#####################################################
//                == SOLID SYSTEM ==
//  - Check if the entities colliding have to drill (atravesarse) or not
//
//   Using Componets:
//      - Collidable (Read and Write)
//      - Situation  (Read)
//
//#####################################################


//#COLLISIONS TABLE (THEY COLLIDING?):
//
// (#TYPE)          STATIC   DYNAMIC   KINEMATICMELEE   KINEMATIC   DRILLCOLL
//
// STATIC            YES       YES          YES            YES         NO
//
// DYNAMIC           YES       YES           NO            YES         NO
//
// KINEMATICMELEE    YES       NO            NO            YES         NO
//
// KINEMATIC         YES       YES          YES            NO          NO
//
// DRILLCOLL         NO        NO            NO            NO          NO
//
//#SOLID TABLE (Do they physicaly colide?)
//
// (#TYPE)          STATIC   DYNAMIC   KINEMATICMELEE   KINEMATIC   DRILLCOLL
//
// DYNAMIC           YES       YES          NO            NO         NO

SolidSystem::SolidSystem() {}

SolidSystem::~SolidSystem() {}

void SolidSystem::update(GameEngine &gameContext) const {

    auto &collidables = gameContext.entityMan.getComponents<CollisionComponent>();

    float limitLeft;
    float limitRigth;
    float limitTop;
    float limitBottom;
    Utils::calculateTerrainLimits(limitLeft, limitRigth, limitTop, limitBottom, gameContext);

    // ALL CAN PUSH THE PALANTIR
    if (gameContext.palantirId)
        checkCollisionsType(gameContext, gameContext.entityMan.getComponent<CollisionComponent>(gameContext.palantirId), limitLeft, limitRigth, limitTop, limitBottom);


    for (auto &collidable : collidables) {
        if(collidable.type == DYNAMIC) {
            checkCollisionsType(gameContext, collidable, limitLeft, limitRigth, limitTop, limitBottom);
        }
    }

    playerNeverOutOfWall(gameContext);


    //!!!!!This might be moved to a new system¡¡¡¡¡¡¡
    std::vector<uint32_t> entitiesToUpdate = gameContext.getEntitiesToUpdate();
    gameContext.getWindowFacadeRef().updateEntities(gameContext, {entitiesToUpdate});

}

void SolidSystem::playerNeverOutOfWall(GameEngine &gameContext) const {
    auto &playerSituation {gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId)};
    gameContext.distanceToCenter = Utils::calculateDistance(playerSituation.x, 0, playerSituation.z, 0);

    float maxDistanceAwoll = 6486.f;

    if(gameContext.distanceToCenter > maxDistanceAwoll) {

        //REPOSICIONATE PLAYER IN THE WALL
        float hypotenuse = maxDistanceAwoll;
        float angle =  atan2(playerSituation.x, playerSituation.z);

        float newX = sin(angle) * hypotenuse;
        float newZ = cos(angle) * hypotenuse;

        playerSituation.x = newX;
        playerSituation.z = newZ;
        gameContext.addEntityToUpdate(gameContext.playerId);

    }




}


void SolidSystem::checkCollisionsType(GameEngine &gameContext, CollisionComponent &currentCollidable, const float limitLeft, const float limitRigth, const float limitTop, const float limitBottom) const {
    auto collisions = currentCollidable.entitiesColliding;

    for (uint32_t collisionId : collisions) {
        CollisionComponent &collisionCollidable = gameContext.entityMan.getComponent<CollisionComponent>(collisionId);
        if (collisionCollidable.type == DYNAMIC || collisionCollidable.type == STATIC) {
            checkAndUpdateCollisions(gameContext, currentCollidable.id, collisionId, currentCollidable, collisionCollidable, limitLeft, limitRigth, limitTop, limitBottom);
        }
    }
}

// Este metodo comprueba si las entidades colisionan y si es asi evita que se atraviesen
void SolidSystem::checkAndUpdateCollisions(GameEngine &gameContext, uint32_t currentId, uint32_t collisionId, CollisionComponent &currentCollidable, CollisionComponent &collisionCollidable, const float limitLeft, const float limitRigth, const float limitTop, const float limitBottom) const {
    // We take the positions
    SituationComponent &currentSituation = gameContext.entityMan.getComponent<SituationComponent>(currentId);
    SituationComponent &collisionSituation = gameContext.entityMan.getComponent<SituationComponent>(collisionId);

    float currentPosX = currentSituation.x;
    float collisionPosX = collisionSituation.x;

    float currentPosZ = currentSituation.z;
    float collisionPosZ = collisionSituation.z;

    //We check if they collision and overlap
    if(currentPosX >= limitLeft && currentPosX <= limitRigth && currentPosZ <= limitTop && currentPosZ >= limitBottom) {
        // We calculate the minimum distance on both axes so that the entities do not overlap
        float x_min = currentCollidable.sizeX / 2 + collisionCollidable.sizeX / 2;
        float z_min = currentCollidable.sizeZ / 2 + collisionCollidable.sizeZ / 2;

        // We detect the distances in both axes involved
        float x_dist = abs(currentPosX - collisionPosX);
        float z_dist = abs(currentPosZ - collisionPosZ);

        // We calculate the overlap in each axis
        float x_overlap = x_min - x_dist;
        float z_overlap = z_min - z_dist;

        // Finally, we'll update the position in the axis with less overloap
        if (x_overlap > z_overlap) {
            // move current in z
            if (currentPosZ < collisionPosZ)
                currentSituation.z = (collisionPosZ - z_min);
            else
                currentSituation.z = (collisionPosZ + z_min);

        } else {
            // move current in x
            if (currentPosX < collisionPosX)
                currentSituation.x = (collisionPosX - x_min);
            else
                currentSituation.x = (collisionPosX + x_min);
        }

        gameContext.addEntityToUpdate(currentId);
    }
}

/*
void SolidSystem::update(GameEngine &gameContext) const {
    auto &collidables = gameContext.entityMan.getComponents<CollisionComponent>();

    float limitLeft;
    float limitRigth;
    float limitTop;
    float limitBottom;
    Utils::calculateTerrainLimits(limitLeft, limitRigth, limitTop, limitBottom, gameContext);

    for (auto &collidable : collidables) {
        if (collidable.type == DYNAMIC) {
            checkAndUpdateCollisions(gameContext, collidable.id, collidable, limitLeft, limitRigth, limitTop, limitBottom);
        }
    }

    // !!!!!This might be moved to a new system¡¡¡¡¡¡¡
    std::vector<uint32_t> entitiesToUpdate = gameContext.getEntitiesToUpdate();

    gameContext.getWindowFacadeRef().updateEntities(gameContext, {entitiesToUpdate});

}

void SolidSystem::checkAndUpdateCollisions(GameEngine &gameContext, uint32_t currentId, CollisionComponent &currentCollidable, const float limitLeft, const float limitRigth, const float limitTop, const float limitBottom) const {
    SituationComponent &currentSituation = gameContext.entityMan.getComponent<SituationComponent>(currentId);

    auto collisions = currentCollidable.entitiesColliding;

    for (uint32_t collisionId : collisions) {
        CollisionComponent &collisionCollidable = gameContext.entityMan.getComponent<CollisionComponent>(collisionId);

        if (collisionCollidable.type == DYNAMIC || collisionCollidable.type == STATIC) {
            // If there's a collision that must cause solid impact, we have to separate the
            // entities implied for they not to overlap
            SituationComponent &collisionSituation = gameContext.entityMan.getComponent<SituationComponent>(collisionId);

            // We take the positions
            float currentPosX = currentSituation.x;
            float collisionPosX = collisionSituation.x;

            float currentPosZ = currentSituation.z;
            float collisionPosZ = collisionSituation.z;

            if(currentPosX >= limitLeft && currentPosX <= limitRigth && currentPosZ <= limitTop && currentPosZ >= limitBottom) {
                // We calculate the minimum distance on both axes so that the entities do not overlap
                float x_min = currentCollidable.sizeX / 2 + collisionCollidable.sizeX / 2;
                float z_min = currentCollidable.sizeZ / 2 + collisionCollidable.sizeZ / 2;

                // We detect the distances in both axes involved
                float x_dist = abs(currentPosX - collisionPosX);
                float z_dist = abs(currentPosZ - collisionPosZ);

                // We calculate the overlap in each axis
                float x_overlap = x_min - x_dist;
                float z_overlap = z_min - z_dist;

                // Finally, we'll update the position in the axis with less overloap
                if (x_overlap > z_overlap) {
                    // move current in z
                    if (currentPosZ < collisionPosZ)
                        currentSituation.z = (collisionPosZ - z_min);
                    else
                        currentSituation.z = (collisionPosZ + z_min);

                } else {
                    // move current in x
                    if (currentPosX < collisionPosX)
                        currentSituation.x = (collisionPosX - x_min);
                    else
                        currentSituation.x = (collisionPosX + x_min);
                }
                gameContext.addEntityToUpdate(currentId);
            }

        }
    }
}
*/