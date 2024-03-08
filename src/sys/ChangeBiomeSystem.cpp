#include "ChangeBiomeSystem.hpp"

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>


//console.log


//#####################################################
//                == CHANGE BIOME SYSTEM ==
//  - Does a function when the player moves to a different biome
//
//   Using Componets:
//      - Situation     (Read)
//
//#####################################################


ChangeBiomeSystem::ChangeBiomeSystem() {}

ChangeBiomeSystem::~ChangeBiomeSystem() {}

void ChangeBiomeSystem::update(GameEngine &gameContext) const {
    SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
    uint8_t            lastBiome        = gameContext.playerBiome;
    uint8_t            currentBiome     = Utils::calculateBiome(gameContext, playerSituation.x, playerSituation.z);
    

    if (gameContext.timeInBiomes.at(gameContext.biomeInAnimation) < 3)
        gameContext.timeInBiomes.at(gameContext.biomeInAnimation) += gameContext.getDeltaTime();
    else if (currentBiome < 6)
        gameContext.biomeInAnimation = currentBiome;

    if(lastBiome != currentBiome) {
        gameContext.lastPlayerBiome = gameContext.playerBiome;
        gameContext.playerBiome = currentBiome;
        callFunctionOfNewBiome(gameContext);
    }

}


void ChangeBiomeSystem::callFunctionOfNewBiome(GameEngine &gameContext) const {
    uint8_t currentBiome = gameContext.playerBiome;
}