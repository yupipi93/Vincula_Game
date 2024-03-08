#include "InteractionSystem.hpp"

#include <eng/GameEngine.hpp>

#include <tools/Utils.hpp>
//console.log


//#####################################################
//             == INTERACTION SYSTEM ==
//  - Checks if player is interacting. If so, checks
//      all interactions possible
//
//   Using Componets:
//      - Input      (Read)
//#####################################################

InteractionSystem::InteractionSystem() {}

InteractionSystem::~InteractionSystem() {}

void InteractionSystem::update(GameEngine &gameContext) const {
    uint32_t playerId = gameContext.playerId;
    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(playerId);

    playerInteraction.resetActions();

    checkInteractions(gameContext, playerId, playerInteraction);
}

void InteractionSystem::checkInteractions(GameEngine &gameContext, uint32_t &playerId, InteractionComponent &playerInteraction) const {
    InputComponent        &playerInput             = gameContext.entityMan.getComponent<InputComponent>(playerId);
    BoundingComponent     &playerSensor            = gameContext.entityMan.getComponent<BoundingComponent>(playerId);
    SituationComponent    &playerSituation         = gameContext.entityMan.getComponent<SituationComponent>(playerId);


    float    minDistance = 999999;
    uint32_t minId       = 0;

    for(size_t i = 0; i < playerSensor.entitiesColliding.size(); i++) {
        //Entity entity = gameContext.getEntity(playerSensor.entitiesColliding[i]);
        uint32_t entityId = playerSensor.entitiesColliding[i];
        EntityType entType = gameContext.getEntity(entityId).getType();
        SituationComponent &entitySituation = gameContext.entityMan.getComponent<SituationComponent>(entityId);


        if(entType == DOOR || entType == KEY || entType == MELEEWEAPON || entType == DISTANCEWEAPON || entType == BANK_DEPOSITOR || entType == BANK_EXTRACTOR || entType == PALANTIR) {
            float dist = Utils::calculateDistance(playerSituation.x, entitySituation.x, playerSituation.z, entitySituation.z);
            if (dist <= minDistance) {
                minDistance = dist;
                minId = entityId;
            }
        } else if(entType == CHEST) {
            ChestComponent &chestComp = gameContext.entityMan.getComponent<ChestComponent>(entityId);
            if(!chestComp.opened) {
                float dist = Utils::calculateDistance(playerSituation.x, entitySituation.x, playerSituation.z, entitySituation.z);
                if (dist <= minDistance) {
                    minDistance = dist;
                    minId = entityId;
                }
            }
        } else if(entType == NPC) {
            DialogueComponent &dialogueComp = gameContext.entityMan.getComponent<DialogueComponent>(entityId);
            if(dialogueComp.wantToTalk) {
                float dist = Utils::calculateDistance(playerSituation.x, entitySituation.x, playerSituation.z, entitySituation.z);
                if (dist <= minDistance) {
                    minDistance = dist;
                    minId = entityId;
                }
            }
        }
    }

    if (minId) {
        playerInteraction.entityInteractibleId = minId;
    }

    if(minId && playerInput.interacting) {
        EntityType eiType = gameContext.getEntity(minId).getType();
        switch(eiType) {
        case DOOR:
            playerInteraction.openingDoor = true;
            break;

        case KEY:
            playerInteraction.keyInteracting = minId;
            break;

        case CHEST:
            playerInteraction.coinChestIdInteracting = minId;
            break;

        case NPC:
            playerInteraction.dialogueInteracting = minId;
            break;

        case MELEEWEAPON:
        case DISTANCEWEAPON:
            gameContext.playerHasWeapon = true;
            playerInteraction.weaponIdInteracting = minId;
            break;

        case PALANTIR:
            playerInteraction.palantirInteracting = minId;
            break;
        }
    }
}




