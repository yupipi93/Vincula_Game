#include "CoinDetectorSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <tools/Storage.hpp>


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


CoinDetectorSystem::CoinDetectorSystem() {}

CoinDetectorSystem::~CoinDetectorSystem() {}


void CoinDetectorSystem::update(GameEngine &gameContext) const {

   auto &coins = gameContext.entityMan.getComponents<CoinComponent>();
   if(coins.size()) {
      updateCoinDetectors(gameContext, coins);
   }



}


void CoinDetectorSystem::updateCoinDetectors(GameEngine &gameContext, Storage<CoinComponent> &coins) const {
    auto &coinDetectectors = gameContext.entityMan.getComponents<CoinDetectorComponent>();
        for (auto &coinDetector : coinDetectectors) {
            coinDetector.entitiesColliding.clear();
            auto &detecorSituation = gameContext.entityMan.getComponent<SituationComponent>(coinDetector.id);
            for(auto &coin : coins) {
                checkIsSensing(gameContext, coinDetector, detecorSituation, coin.id);
            }
        }
}

void CoinDetectorSystem::checkIsSensing(GameEngine &gameContext, BoundingComponent &coinDetector, SituationComponent &detecorSituation, uint32_t coinId) const {
    auto &coinSituation = gameContext.entityMan.getComponent<SituationComponent>(coinId);
    auto &coinCollision = gameContext.entityMan.getComponent<CollisionComponent>(coinId);

    if (Utils::isCollisionAB(detecorSituation, coinDetector, coinSituation, coinCollision)) {
        coinDetector.entitiesColliding.emplace_back(coinId);
    }

}
