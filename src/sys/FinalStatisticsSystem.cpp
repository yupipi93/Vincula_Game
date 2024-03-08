#include "FinalStatisticsSystem.hpp"

#include <eng/GameEngine.hpp>


//#####################################################
//                == PROXIMITY SYSTEM ==
//  - Get all entities with proximity
//
//   Using Componets:
//      - Proximity     (Read - Write)
//      - MoveTo        (Read - Write)
//      - Sensor        (Read)
//#####################################################


FinalStatisticsSystem::FinalStatisticsSystem() {}

FinalStatisticsSystem::~FinalStatisticsSystem() {}


void FinalStatisticsSystem::update(GameEngine &gameContext) const {
    // PLAYED TIME
    gameContext.playedTime += gameContext.getDeltaTime();

    // DEATHS TIMES
    StatsComponent &playerStats = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
    if(playerStats.currentHealth == 0) {
        ++gameContext.deaths;
        gameContext.totalDropedCoins += gameContext.dropedCoins;
    }

    // /../, ENEMIES DEATHS IS IN DEATH SYSTEM BECAUSE WE ARE THAT FUCKERS YEAAAAH ,/../
}
