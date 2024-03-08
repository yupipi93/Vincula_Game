#include "BaseSystem.hpp"


#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>


//#####################################################
//                == BASE SYSTEM ==
//  - Checks if player is in base to allow base actions
//
//   Using Componets:
//      - Wallet        (Read and Write)
//      - MoveTo        (Read and Write)
//      - Input         (Read)
//      - Bounding      (Read)
//#####################################################
BaseSystem::BaseSystem() {}

BaseSystem::~BaseSystem() {}

void BaseSystem::update(GameEngine &gameContext) const {
    if(gameContext.playerBiome == 0) {
        restorePlayerHealth(gameContext);
        bankActions(gameContext);
        sourceAnimation(gameContext);
    }

}


void BaseSystem::bankActions(GameEngine &gameContext) const {
    InputComponent      &inputComp = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    AtributesComponent  &atribComp = gameContext.entityMan.getComponent<AtributesComponent>(gameContext.playerId);

    bool inDepositor = isPlayerInSensor(gameContext, gameContext.bankDepositorId);
    bool inExtractor = isPlayerInSensor(gameContext, gameContext.bankExtractorId);

    //ACTIONS
    if(inputComp.interacting) {
        if(inDepositor) {
            depositeCoins(gameContext);
            inputComp.interactingTimer = inputComp.bankCoinsMaxTimer;
        }
        if(inExtractor) {
            extractCoins(gameContext);
            inputComp.interactingTimer = inputComp.bankCoinsMaxTimer;
        }
    }



    //SWITCH MODE DEPOSITOR AN EXTRACTOR
    if(inDepositor && !atribComp.wasInBanckDepositor) {
        setSecundaryModel(gameContext, gameContext.bankDepositorId);
    }
    if(!inDepositor && atribComp.wasInBanckDepositor) {
        setDefaultModel(gameContext, gameContext.bankDepositorId);
    }

    if(inExtractor && !atribComp.wasInBanckExtractor) {
        setSecundaryModel(gameContext, gameContext.bankExtractorId);
    }
    if(!inExtractor && atribComp.wasInBanckExtractor) {
        setDefaultModel(gameContext, gameContext.bankExtractorId);
    }


    atribComp.wasInBanckDepositor = inDepositor;
    atribComp.wasInBanckExtractor = inExtractor;






}


void BaseSystem::setDefaultModel(GameEngine &gameContext, uint32_t id) const {
    DrawableComponent &drawableComp {gameContext.entityMan.getComponent<DrawableComponent>(id)};
    std::string aux = drawableComp.modelSecondary;
    drawableComp.modelSecondary = drawableComp.model;
    drawableComp.model = aux;
    gameContext.getWindowFacadeRef().updateModel(gameContext, id);
    gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId).y = 0;
}

void BaseSystem::setSecundaryModel(GameEngine &gameContext, uint32_t id) const {
    DrawableComponent &drawableComp {gameContext.entityMan.getComponent<DrawableComponent>(id)};
    std::string aux = drawableComp.model;
    drawableComp.model = drawableComp.modelSecondary;
    drawableComp.modelSecondary = aux;
    gameContext.getWindowFacadeRef().updateModel(gameContext, id);
    gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId).y = 2;
}

void BaseSystem::sourceAnimation(GameEngine &gameContext) const {
    auto &sourceSituationComp {gameContext.entityMan.getComponent<SituationComponent>(gameContext.sourceId)};
    sourceSituationComp.rotation += 10 * gameContext.getDeltaTime();
    gameContext.getWindowFacadeRef().updateEntities(gameContext, {gameContext.sourceId});

}


void BaseSystem::depositeCoins(GameEngine &gameContext) const {
    exchangeCoins(gameContext, gameContext.playerId, gameContext.bankId);

}

void BaseSystem::extractCoins(GameEngine &gameContext) const {
    exchangeCoins(gameContext, gameContext.bankId, gameContext.playerId);

}


void BaseSystem::exchangeCoins(GameEngine &gameContext, uint32_t entityId, uint32_t targetId) const {
    WalletComponent &entiyWallet = gameContext.entityMan.getComponent<WalletComponent>(entityId);

    if(entiyWallet.coins > 0) {
        //Spawn a coin
        Utils::dropCoinWithTarget(gameContext, entityId, targetId);
    } else {
        if(entityId == gameContext.playerId) {
            SituationComponent &depositorSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.bankDepositorId);
            //gameContext.entityMan.createNotification(gameContext, depositorSituation.x, depositorSituation.y + 18, depositorSituation.z, 0, NOTIFICATION_BANK_DEPOSITOR);
            Utils::activeInfoNotification(gameContext, 0);
        } else {
            SituationComponent &extractorSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.bankExtractorId);
            //gameContext.entityMan.createNotification(gameContext, extractorSituation.x, extractorSituation.y + 18, extractorSituation.z, 0, NOTIFICATION_BANK_EXTRACTOR);
            Utils::activeInfoNotification(gameContext, 1);
        }
    }
}




void BaseSystem::restorePlayerHealth(GameEngine &gameContext) const {
    if(isPlayerInSensor(gameContext, gameContext.sourceId)) {

        StatsComponent      &playerStats    = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
        if(playerStats.currentHealth < playerStats.maxHealth) {
            playerStats.healing = true;
            //playerStats.healingTrue = true;
            playerStats.currentHealth = playerStats.currentHealth + 100 * gameContext.getDeltaTime();
        }
    }
}



bool BaseSystem::isPlayerInSensor(GameEngine &gameContext, uint32_t sensorId) const {
    std::vector<uint32_t> &detectedIds = gameContext.entityMan.getComponent<BoundingComponent>(sensorId).entitiesColliding;
    for (auto entityId : detectedIds) {
        if(gameContext.playerId == entityId) {
            return true;
        }
    }
    return false;
}






