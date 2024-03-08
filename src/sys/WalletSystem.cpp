#include "WalletSystem.hpp"

#include <eng/GameEngine.hpp>

//#####################################################
//                == WALLET SYSTEM ==
//  - Get All Entities whith Wallet
//  - If entity detect Coin    (set selft target to coin)
//  - If entity touching Coin  (chatch and remove this coin)
//
//   Using Componets:
//      - Wallet        (Read - Write)
//      - MoveTo        (Read - Write)
//      - CoinDetector  (Read)
//      - Collider      (Read)
//      - CoinComponent (Read)
//#####################################################


WalletSystem::WalletSystem() {}

WalletSystem::~WalletSystem() {}

void WalletSystem::update(GameEngine &gameContext) const {


    if(gameContext.entityMan.getComponents<CoinComponent>().size() > 0) {

        auto &allWallets = gameContext.entityMan.getComponents<WalletComponent>();
        for(auto &entity : allWallets) {
            if(gameContext.entityMan.existsComponent<CoinDetectorComponent>(entity.id)) {
                processCoinsSensing(gameContext, entity.id);
                processCoinsColliding(gameContext, entity.id);
            }
        }
    }

}


void WalletSystem::processCoinsSensing(GameEngine &gameContext, uint32_t entityId) const {
    //Get all EntitiesColliding by entity sensor
    auto &coinsColliding = gameContext.entityMan.getComponent<CoinDetectorComponent>(entityId).entitiesColliding;
    //Check if any Coin is detected
    for(auto &coinId : coinsColliding) {
        setTarget(gameContext, entityId, coinId);
    }
}



void WalletSystem::processCoinsColliding(GameEngine &gameContext, uint32_t entityId) const {
    std::vector<uint32_t> coinsToErase;
    auto &entitiesCollidingIds = gameContext.entityMan.getComponent<CollisionComponent>(entityId).entitiesColliding;

    //Check if any is Coin is colliding
    for(uint32_t collidingId : entitiesCollidingIds) {
        if(gameContext.entityMan.existsComponent<CoinComponent>(collidingId)) {
            auto &coinComp = gameContext.entityMan.getComponent<CoinComponent>(collidingId);
            if(coinComp.owner != entityId) {
                catchCoin(gameContext, entityId, collidingId);
                coinsToErase.emplace_back(collidingId);
            }
        }
    }

    for(size_t i = 0; i < coinsToErase.size(); ++i) {
        gameContext.eraseEntityByID(coinsToErase[i]);
    }
}




void WalletSystem::setTarget(GameEngine &gameContext, uint32_t entityId, uint32_t coinId) const {
    if(gameContext.entityMan.existsComponent<AIMoveToComponent>(coinId)) {
        auto &moveToComp = gameContext.entityMan.getComponent<AIMoveToComponent>(coinId);
        moveToComp.targetId = entityId;
    }

}

void WalletSystem::catchCoin(GameEngine &gameContext, uint32_t entityId, uint32_t coinId) const {
    //Add coin to entity wallet
    WalletComponent &walletComp = gameContext.entityMan.getComponent<WalletComponent>(entityId);
    CoinComponent   &coinComp   = gameContext.entityMan.getComponent<CoinComponent>(coinId);

    walletComp.coins += coinComp.value;

    //TEMP
    if(gameContext.playerId == entityId) {
        gameContext.getSoundFacadeRef().setParameterEventByID(coinId, DEAD_SOUND);

    }


}
