#include "InventorySystem.hpp"
//console.log

#include <eng/GameEngine.hpp>


//#####################################################
//                == INVENTORY SYSTEM ==
//  - ?????????????????????
//  - ?????????????????????
//
//   Using Componets:
//      - ?????                 (Read)
//      - ?????                 (Read)
//#####################################################

InventorySystem::InventorySystem() {
    /*
    bowDefaultPosition       = {-100, 0, -80};
    blowgunDefaultPosition   = {-60, 0, -80};
    plasmagunDefaultPosition = {-20, 0, -80};
    swordDefaultPosition     = {20, 0, -80};
    maceDefaultPosition      = {60, 0, -80};
    spearDefaultPosition     = {100, 0, -80};
    */
}

InventorySystem::~InventorySystem() {}
void InventorySystem::update(GameEngine &gameContext) const {
    uint32_t playerId = gameContext.playerId;
    InventoryComponent   &playerInventory   = gameContext.entityMan.getComponent<InventoryComponent>(playerId);
    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(playerId);

    checkCatchingWeapons(gameContext, playerInventory, playerInteraction);
    checkCatchingKey(gameContext, playerInventory, playerInteraction);
}

void InventorySystem::checkCatchingWeapons(GameEngine &gameContext, InventoryComponent &playerInventory, InteractionComponent &playerInteraction) const {
    if(playerInteraction.weaponIdInteracting != 0) { //Esta interactuando con un arma
        SituationComponent &newWeaponSituation = gameContext.entityMan.getComponent<SituationComponent>(playerInteraction.weaponIdInteracting);
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, 6);

        if(playerInventory.primaryWeaponId == 0) { //###if primary empty
            playerInventory.primaryWeaponId = playerInteraction.weaponIdInteracting; //Primary = new weapon

        } else if(playerInventory.primaryWeaponId != 0 && playerInventory.secondaryWeaponId == 0) { //###If primary fulled and secondary empty
            playerInventory.secondaryWeaponId = playerInventory.primaryWeaponId;     //Secondary = primary
            playerInventory.primaryWeaponId = playerInteraction.weaponIdInteracting; //Primary = new weapon

        } else if(playerInventory.primaryWeaponId != 0 && playerInventory.secondaryWeaponId != 0) { //###If primary and secondary fulled
            SituationComponent &primarySituation = gameContext.entityMan.getComponent<SituationComponent>(playerInventory.primaryWeaponId);

            primarySituation.x = newWeaponSituation.x;
            primarySituation.y = newWeaponSituation.y;
            primarySituation.z = newWeaponSituation.z;

            DrawableComponent &drawableWeapon = gameContext.entityMan.getComponent<DrawableComponent>(playerInventory.primaryWeaponId);
            drawableWeapon.mustBeDrawn = true;
            gameContext.entityMan.createComponent<CollisionComponent>(playerInventory.primaryWeaponId);  //###CAMBIAR CUANDO HAYA BALAS PERFORANTES###

            playerInventory.primaryWeaponId = playerInteraction.weaponIdInteracting; //Primary = new weapon
        }

        weaponCatchedToDefaultPosition(gameContext, newWeaponSituation, playerInteraction.weaponIdInteracting);

        gameContext.getWindowFacadeRef().updateWeapons2DTexture(gameContext);
    }
}

void InventorySystem::weaponCatchedToDefaultPosition(GameEngine &gameContext, SituationComponent &weaponSit, uint32_t idWeapon) const {
    WeaponType weaponType;
    if(gameContext.entityMan.getComponents<MeleeWeaponComponent>().count(idWeapon)) {
        MeleeWeaponComponent &newWeaponComp = gameContext.entityMan.getComponent<MeleeWeaponComponent>(idWeapon);
        weaponType = newWeaponComp.weapon;
        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, 6);

        weaponSit.x = newWeaponComp.defaultPosition[0];
        weaponSit.y = newWeaponComp.defaultPosition[1];
        weaponSit.z = newWeaponComp.defaultPosition[2];


    } else if(gameContext.entityMan.getComponents<DistanceWeaponComponent>().count(idWeapon)) {
        DistanceWeaponComponent &newWeaponComp = gameContext.entityMan.getComponent<DistanceWeaponComponent>(idWeapon);
        weaponType = newWeaponComp.weapon;

        weaponSit.x = newWeaponComp.defaultPosition[0];
        weaponSit.y = newWeaponComp.defaultPosition[1];
        weaponSit.z = newWeaponComp.defaultPosition[2];

    }

    //gameContext.getWindowFacadeRef().eraseEntity(idWeapon);
    DrawableComponent &drawableWeapon = gameContext.entityMan.getComponent<DrawableComponent>(idWeapon);
    drawableWeapon.mustBeDrawn = false;
    gameContext.entityMan.eraseComponent<CollisionComponent>(idWeapon);

    gameContext.getWindowFacadeRef().updateEntities(gameContext, {idWeapon});

    eraseIluminationLight(gameContext, idWeapon);
}


void InventorySystem::checkCatchingKey(GameEngine &gameContext, InventoryComponent &playerInventory, InteractionComponent &playerInteraction) const {

    if (playerInventory.showKeysTime > 0)
        playerInventory.showKeysTime -= gameContext.getDeltaTime();

    if(playerInteraction.keyInteracting != 0) {
        bool alreadycollected = false;
        for (size_t i = 0; i < playerInventory.keysCollected.size(); ++i) {
            if(playerInventory.keysCollected[i] == gameContext.entityMan.getComponent<KeyComponent>(playerInteraction.keyInteracting).keyType) {
                alreadycollected = true;
            }
        }
        if(!alreadycollected) {
            KeyComponent &keyComponent = gameContext.entityMan.getComponent<KeyComponent>(playerInteraction.keyInteracting);
            playerInventory.keysCollected.push_back(keyComponent.keyType);
            playerInventory.showKeysTime = 2.f;
            gameContext.eraseEntityByID(playerInteraction.keyInteracting);
            playerInteraction.resetActions();

        }
    }
}


void InventorySystem::eraseIluminationLight(GameEngine &gameContext, uint32_t entityId) const {
    if(gameContext.entityMan.existsComponent<IluminationComponent>(entityId)) {
        IluminationComponent &iluminationComp = gameContext.entityMan.getComponent<IluminationComponent>(entityId);

        // Delete the light
        if(iluminationComp.pointLightId != 0) {
            gameContext.eraseEntityByID(iluminationComp.pointLightId);
        }

        // Delete the component
        gameContext.entityMan.eraseComponent<IluminationComponent>(entityId);
    }
}