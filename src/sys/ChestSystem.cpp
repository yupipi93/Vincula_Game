#include "ChestSystem.hpp"
#include <tools/Utils.hpp>
//console.log


//#####################################################
//                == CHEST SYSTEM ==
//  - ???????????????? REFACTOR PLEASE
//
//   Using Componets:
//      - ????????????????????  (Read - Write)
//
//#####################################################

constexpr uint8_t chestSpawnRange = 100;

ChestSystem::ChestSystem() {}

ChestSystem::~ChestSystem() {}

void ChestSystem::update(GameEngine &gameContext) const {

    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(gameContext.playerId);

    if(playerInteraction.coinChestIdInteracting != 0)
        searchChest(gameContext, playerInteraction);
}

void ChestSystem::searchChest(GameEngine &gameContext, InteractionComponent &playerInteraction) const {
    uint32_t chestId = playerInteraction.coinChestIdInteracting;

    ChestComponent &chestComponent = gameContext.entityMan.getComponent<ChestComponent>(chestId);
    DrawableComponent &chestDrawable   = gameContext.entityMan.getComponent<DrawableComponent>(chestId);

    if(!chestComponent.opened) {
        if(chestComponent.chestType == COINCHEST) {
            SituationComponent  &situation  = gameContext.entityMan.getComponent<SituationComponent>(chestId);
            //generateCoins(gameContext, chestComponent, situation);
            Utils::dropCoins<ChestComponent>(gameContext, chestId, chestSpawnRange);
        } else if(chestComponent.chestType == WEAPONCHEST) {
            SituationComponent  &situation  = gameContext.entityMan.getComponent<SituationComponent>(chestId);
            Utils::dropWeapon(gameContext, chestComponent.weaponType, situation.x, situation.y, situation.z - 10);
            chestComponent.weaponType = WeaponType::UNKNOWN;
        }
        
        chestComponent.opened = true;
        gameContext.getSoundFacadeRef().playEvent(gameContext, chestComponent.id);

        chestDrawable.model = chestComponent.modelOpenedChest;
        gameContext.getWindowFacadeRef().updateModel(gameContext, chestId);

        // Remove interrogation
        gameContext.eraseEntityByID(chestComponent.interrogation);
    }
}

