#include "SecureSystem.hpp"

#include <tools/Utils.hpp>
#include <math.h>

#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == SECURE SYSTEM ==
//  - Check if Player is in a secure zone and updates
//      what it must
//
//   Using Componets:
//#####################################################


SecureSystem::SecureSystem() {}

SecureSystem::~SecureSystem() {}

void SecureSystem::update(GameEngine &gameContext) const {

    uint32_t playerId = gameContext.playerId;
    uint32_t playerInSecureZoneId = checkPlayerIsSecure(gameContext, playerId);


    setActiveTexture(gameContext, playerInSecureZoneId);


}
void SecureSystem::setActiveTexture(GameEngine &gameContext, uint32_t playerInSecureZoneId) const {
    if(playerInSecureZoneId) {
        //setActiveModel();
        SecureZoneComponent  &secureZoneComp {gameContext.entityMan.getComponent<SecureZoneComponent>(playerInSecureZoneId)};
        if(!secureZoneComp.isActive) {
            secureZoneComp.isActive = true;

            DrawableComponent &drawableComp {gameContext.entityMan.getComponent<DrawableComponent>(playerInSecureZoneId)};
            std::string aux = drawableComp.model;
            drawableComp.model = drawableComp.modelSecondary;
            drawableComp.modelSecondary = aux;
            gameContext.getWindowFacadeRef().updateModel(gameContext, playerInSecureZoneId);
        }
    }

    //get all secureZoneComp
    auto &allSecureZones {gameContext.entityMan.getComponents<SecureZoneComponent>()};
    for(auto &secureZoneComp : allSecureZones) {
        if(secureZoneComp.isActive && !playerInSecureZoneId) {
            secureZoneComp.isActive = false;


            DrawableComponent &drawableComp {gameContext.entityMan.getComponent<DrawableComponent>(secureZoneComp.id)};
            std::string aux = drawableComp.modelSecondary;
            drawableComp.modelSecondary = drawableComp.model;
            drawableComp.model = aux;
            gameContext.getWindowFacadeRef().updateModel(gameContext, secureZoneComp.id);

        }
    }

}




uint32_t SecureSystem::checkPlayerIsSecure(GameEngine &gameContext, uint32_t playerId) const {
    CollisionComponent  &playerColidable    {gameContext.entityMan.getComponent<CollisionComponent>(playerId)};
    AtributesComponent     &playerAtrib       {gameContext.entityMan.getComponent<AtributesComponent>(playerId)};

    playerAtrib.isSecure = false;

    for (auto &entitySensedId : playerColidable.entitiesColliding)
        if (gameContext.getEntity(entitySensedId).getType() == SECUREZONE) {
            playerAtrib.isSecure = true;
            return entitySensedId;
        }


    return 0;

}