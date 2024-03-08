#include "InfoActionSystem.hpp"

#include <tools/Utils.hpp>
#include <eng/GameEngine.hpp>


//console.log

//#####################################################
//                == INFO ACTION SYSTEM ==
//  - 
//
//#####################################################


InfoActionSystem::InfoActionSystem() {}

InfoActionSystem::~InfoActionSystem() {}

void InfoActionSystem::update(GameEngine &gameContext) const {
    
    auto &playerId = gameContext.playerId;
    auto &mapId = gameContext.mapId;
    auto &playerAtrib = gameContext.entityMan.getComponent<AtributesComponent>(playerId);
    auto &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(playerId);
    auto &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);
    auto &playerLevel = gameContext.entityMan.getComponent<LevelComponent>(playerId);
    auto &dash = gameContext.entityMan.getComponent<DashComponent>(playerId);


    //WALK ACTION
    if(playerInventory.primaryWeaponId != 0 && (playerInput.pressingW || playerInput.pressingA || playerInput.pressingS || playerInput.pressingD)){
        gameContext.desactiveInfoActions[0] = true;
    }

    //ATTACK ACTION
    if(!playerAtrib.isSecure && playerInventory.primaryWeaponId != 0 && gameContext.desactiveInfoActions[0] && playerInput.attacking){
        gameContext.desactiveInfoActions[1] = true;
    }

    //COMPASS ACTION
    if(playerLevel.compassUnlocked && playerInput.callingCompass){
        gameContext.desactiveInfoActions[2] = true;
    }

    //DASH ACTION
    if(!playerAtrib.isSecure && playerLevel.dashUnlocked && dash.dashing){
        gameContext.desactiveInfoActions[3] = true;
    }

     //MAP ACTION
    if(mapId != 0){
         if(playerLevel.mapUnlocked && playerInput.lookingMap){
            gameContext.desactiveInfoActions[4] = true;
        }
    }

    //CHANGE WEAPON ACTION
    if(playerInventory.secondaryWeaponId != 0 && playerInput.changingWeapon){
        gameContext.desactiveInfoActions[5] = true;
    }

    if (playerInput.dropingPalantir)
        gameContext.desactiveInfoActions[6] = true;
}