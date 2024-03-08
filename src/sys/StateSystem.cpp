#include "StateSystem.hpp"
//console.log


#include<tools/Utils.hpp>

#include <eng/GameEngine.hpp>


//#####################################################
//                == STATE SYSTEM ==
//  - ?????????????????????
//  - ?????????????????????
//
//   Using Componets:
//      - ?????                 (Read)
//      - ?????                 (Read)
//#####################################################

StateSystem::StateSystem() {}

StateSystem::~StateSystem() {}


void StateSystem::update(GameEngine &gameContext) const {
    uint32_t playerId = gameContext.playerId;


    checkGameOver(playerId, gameContext);
    checkWinGame(playerId, gameContext);


    


}

void StateSystem::checkGameOver (uint32_t &playerId, GameEngine &gameContext) const {

    StatsComponent &playerStats = gameContext.entityMan.getComponent<StatsComponent>(playerId);

    //Check if player is dead
    if(playerStats.currentHealth == 0) {
        gameContext.setLastState(gameContext.getGameState());
        gameContext.setGameState(GAMEOVER);
    }
}

void StateSystem::checkWinGame(uint32_t &playerId, GameEngine &gameContext) const {

    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(playerId);
    InventoryComponent   &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(playerId);
    //First check if player is interacting with Door
    if(playerInteraction.openingDoor) { 
        //Later check if player has collected all keyGems   
        if(playerInventory.keysCollected.size() >= playerInventory.MAX_KEYS){
            gameContext.setLastState(gameContext.getGameState());
            gameContext.setGameState(WIN);
        }else{ //Else solo para el comentario
            SituationComponent &situationPlayer = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
            //gameContext.entityMan.createNotification(gameContext, situationPlayer.x, situationPlayer.y + 20, situationPlayer.z, 0, NOTIFICATION_FAILDOOR);
            Utils::activeInfoNotification(gameContext, 3);
        }
    }
}








