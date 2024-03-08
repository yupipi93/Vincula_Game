#include "SensorSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

//console.log


//#####################################################
//                == SENSOR SYSTEM ==
//  - Get all collisions by sensor
//  - Calculate the proximity between entities
//  - Delete old collisions by sensor
//
//   Using Componets:
//      - Bounding(Sensor)      (Read)
//      - Bounding(Collidable)  (Read)
//      - Situation             (Read)
//#####################################################


SensorSystem::SensorSystem() {}

SensorSystem::~SensorSystem() {}


void SensorSystem::update(GameEngine &gameContext) const {
    auto &sensors = gameContext.entityMan.getComponents<BoundingComponent>();
    std::vector<uint32_t> boundingsIdInsideChunk {};

    Utils::clearAllEntitiesColliding(sensors);

    boundingsIdInsideChunk.reserve(250);
    auto &boundings  = gameContext.entityMan.getComponents<CollisionComponent>();
    Utils::insertBoundingsIdInsideChunk(gameContext, boundings, boundingsIdInsideChunk);

    for (auto &boundingA : sensors) {
        checkIsSensing(gameContext, boundingA.id, boundingA, boundingsIdInsideChunk);
    }
}

void SensorSystem::checkIsSensing(GameEngine &gameContext, uint32_t idA, BoundingComponent &boundingA, std::vector<uint32_t> boundingsIdInsideChunk) const {
    //Get ID and Situation of boundingA and all collidables to check their collisions
    SituationComponent &situationA = gameContext.entityMan.getComponent<SituationComponent>(idA);

    for (auto &idB : boundingsIdInsideChunk) {

        //diferent entities
        if (idA != idB) {
            BoundingComponent &boundingB = gameContext.entityMan.getComponent<CollisionComponent>(idB);
            SituationComponent &situationB = gameContext.entityMan.getComponent<SituationComponent>(idB);

            if (Utils::isCollisionAB(situationA, boundingA, situationB, boundingB)) {
                boundingA.entitiesColliding.emplace_back(idB);
            }

        }
    }

}
