#include "CollisionSystem.hpp"

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == COLLISION SYSTEM ==
//  - Get all entities with collision and velocity
//  - If some entity is collision with other, the entity with velocity save the entity colliding
//
//   Using Componets:
//      - Collidable (Read and Write)
//      - Situation  (Read)
//      - Velocity   (Read)
//#####################################################


CollisionSystem::CollisionSystem() {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::update(GameEngine &gameContext) const {

    auto &boundings = gameContext.entityMan.getComponents<CollisionComponent>();
    Utils::clearAllEntitiesColliding(boundings);

    std::vector<uint32_t> boundingsIdInsideChunk {};
    boundingsIdInsideChunk.reserve(boundings.size());
    Utils::insertBoundingsIdInsideChunk(gameContext, boundings, boundingsIdInsideChunk);


    for (std::size_t i = 0; i < boundingsIdInsideChunk.size(); ++i) {
        uint32_t idA = boundingsIdInsideChunk[i];
        BoundingComponent &boundingA = gameContext.entityMan.getComponent<CollisionComponent>(idA);
        SituationComponent &situationA = gameContext.entityMan.getComponent<SituationComponent>(idA);

        for (std::size_t j = i+1; j < boundingsIdInsideChunk.size(); ++j) {
            uint32_t idB = boundingsIdInsideChunk[j];
            BoundingComponent &boundingB = gameContext.entityMan.getComponent<CollisionComponent>(idB);
            SituationComponent &situationB = gameContext.entityMan.getComponent<SituationComponent>(idB);

            if (Utils::isCollisionAB(situationA, boundingA, situationB, boundingB)) {
                boundingB.entitiesColliding.emplace_back(idA);
                boundingA.entitiesColliding.emplace_back(idB);
            }     
            
        }
    }


}


/*void CollisionSystem::insertBoundingsIdWithVelocity(GameEngine &gameContext, std::vector<uint32_t> &idBoundingsWithVelocity) const {
    auto &velocitiesComps = gameContext.getVelocities();
    auto &boundings       = gameContext.getCollidables();

    for (auto &velComp : velocitiesComps) {
        uint32_t idVelEnt = velComp.first;

        auto it = boundings.find(idVelEnt);
        if ( it != boundings.end() )
            // found it
            idBoundingsWithVelocity.emplace_back(idVelEnt);
    }
}*/


