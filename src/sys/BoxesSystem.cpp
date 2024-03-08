#include "BoxesSystem.hpp"
#include <eng/GameEngine.hpp>

//console.log


//#####################################################
//                == BOXES SYSTEM ==
//
//   Using Componets:
//      - Collidable (Read and Write)
//      - Situation  (Read)
//      - Velocity   (Read)
//#####################################################

BoxesSystem::BoxesSystem() {}

BoxesSystem::~BoxesSystem() {}


void BoxesSystem::update(GameEngine &gameContext) const {
    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(gameContext.playerId);

    //INFOBOXES
    removeInfoBoxes(gameContext, playerInteraction.entityInteractibleId);
    if(playerInteraction.entityInteractibleId != 0) {
    
        drawInfoHUD(gameContext);
        /*if(!existsInfoBoxWithParent(gameContext, playerInteraction.entityInteractibleId)) {
            createInfoBoxes(gameContext, playerInteraction.entityInteractibleId);
        }*/
    }

    updateInfoBoxesApparition(gameContext);
    updateInfoBoxesRotation(gameContext);

    // MESSAGEBOXES
    updateMessageBoxesRotation(gameContext);

    // NOTIFICATIONS
    removeNotificationsByLifeTime(gameContext);
    updateNotificationApparition(gameContext);
    //updateNotificationsRotation(gameContext);
}

// INFO BOXES
void BoxesSystem::drawInfoHUD(GameEngine &gameContext) const{

}



void BoxesSystem::createInfoBoxes(GameEngine &gameContext, uint32_t entitySensored) const {
    if(gameContext.entityMan.existsComponent<SituationComponent>(entitySensored)) {
        SituationComponent &sensoredSituation = gameContext.entityMan.getComponent<SituationComponent>(entitySensored);

        uint32_t infoBoxId = gameContext.entityMan.createInfoBox(gameContext, sensoredSituation.x, sensoredSituation.y + 10, sensoredSituation.z, 90);
        InfoBoxComponent &infoBox = gameContext.entityMan.getComponent<InfoBoxComponent>(infoBoxId);
        infoBox.parentEntityId = entitySensored;
    }
}


bool BoxesSystem::existsInfoBoxWithParent(GameEngine &gameContext, uint32_t parentId) const {
    bool exists = false;

    auto &infoBoxes = gameContext.entityMan.getComponents<InfoBoxComponent>();
    for(std::size_t i = 0; i < infoBoxes.size(); ++i) {
        if(infoBoxes[i].parentEntityId == parentId) {
            exists = true;
        }
    }

    return exists;
}

void BoxesSystem::removeInfoBoxes(GameEngine &gameContext, uint32_t entitySensored) const {
    auto &infoBoxes = gameContext.entityMan.getComponents<InfoBoxComponent>();

    for(std::size_t i = 0; i < infoBoxes.size(); ++i) {
        if(entitySensored == 0) {
            gameContext.eraseEntityByID(infoBoxes[i].id);
        } else if(infoBoxes[i].parentEntityId != entitySensored || infoBoxes[i].parentEntityId == 0) {
            gameContext.eraseEntityByID(infoBoxes[i].id);
        }
    }
}

void BoxesSystem::updateInfoBoxesApparition(GameEngine &gameContext) const {
    auto &infoBoxes = gameContext.entityMan.getComponents<InfoBoxComponent>();

    for(std::size_t i = 0; i < infoBoxes.size(); ++i) {
        DrawableComponent  &infoBoxDrawable  = gameContext.entityMan.getComponent<DrawableComponent>(infoBoxes[i].id);
        SituationComponent &infoBoxSituation = gameContext.entityMan.getComponent<SituationComponent>(infoBoxes[i].id);
        SituationComponent &parentSituation  = gameContext.entityMan.getComponent<SituationComponent>(infoBoxes[i].parentEntityId);

        /*INCREMENT SITUATION.Y
        Cambiar cuando haya modelos para que compruebe (Situation.Y < padreSituation.Y + Altura del padre + 20)*/
        if(infoBoxSituation.y < (parentSituation.y + 20)) {
            infoBoxSituation.y += 90 * gameContext.getDeltaTime();

            if(infoBoxSituation.y > (parentSituation.y + 20)) {
                infoBoxSituation.y = (parentSituation.y + 20);
            }
        }

        //INCREMENT SCALE
        if(infoBoxDrawable.scaleX < 0.9) {
            infoBoxDrawable.scaleX += 5 * gameContext.getDeltaTime();
            infoBoxDrawable.scaleY += 5 * gameContext.getDeltaTime();
            infoBoxDrawable.scaleZ += 5 * gameContext.getDeltaTime();

            if(infoBoxDrawable.scaleX > 0.9) {
                infoBoxDrawable.scaleX = infoBoxDrawable.scaleY = infoBoxDrawable.scaleZ = 0.9;
            }
        }

        gameContext.getWindowFacadeRef().updateEntities(gameContext, {infoBoxes[i].id});
    }
}

void BoxesSystem::updateInfoBoxesRotation(GameEngine &gameContext) const {
    auto &infoBoxes = gameContext.entityMan.getComponents<InfoBoxComponent>();

    for(std::size_t i = 0; i < infoBoxes.size(); ++i) {
        gameContext.getWindowFacadeRef().updateBoxLookAtCamera(gameContext, infoBoxes[i].id, gameContext.cameraId);
    }
}

// MESSAGE BOXES
void BoxesSystem::updateMessageBoxesRotation(GameEngine &gameContext) const {
    auto &messageBoxes = gameContext.entityMan.getComponents<MessageBoxComponent>();

    for(std::size_t i = 0; i < messageBoxes.size(); ++i) {
        gameContext.getWindowFacadeRef().updateBoxLookAtCamera(gameContext, messageBoxes[i].id, gameContext.cameraId);
    }
}

void BoxesSystem::removeMessageBoxesByLifeTime(GameEngine &gameContext) const {
    auto &allMessageBoxes = gameContext.entityMan.getComponents<MessageBoxComponent>();
    std::vector<uint32_t> messageBoxesIdToDelete;

    for(auto &messageBox : allMessageBoxes) {
        messageBox.lifeTime -= gameContext.getDeltaTime();
        if(messageBox.lifeTime <= 0) {
            messageBoxesIdToDelete.emplace_back(messageBox.id);
        }
    }

    for (size_t i = 0; i < messageBoxesIdToDelete.size(); ++i) {
        gameContext.eraseEntityByID(messageBoxesIdToDelete[i]);
    }
}

// NOTIFICATIONS
void BoxesSystem::updateNotificationsRotation(GameEngine &gameContext) const {
    auto &notifications = gameContext.entityMan.getComponents<NotificationComponent>();

    for(std::size_t i = 0; i < notifications.size(); ++i) {
        gameContext.getWindowFacadeRef().updateBoxLookAtCamera(gameContext, notifications[i].id, gameContext.cameraId);
    }
}

void BoxesSystem::updateNotificationApparition(GameEngine &gameContext) const {
    auto &notifications = gameContext.entityMan.getComponents<NotificationComponent>();

    for(std::size_t i = 0; i < notifications.size(); ++i) {
        /*
        DrawableComponent  &notificationDrawable  = gameContext.entityMan.getComponent<DrawableComponent>(notifications[i].id);

        //INCREMENT SCALE
        if(notificationDrawable.scaleY < 1.5) { 
            notificationDrawable.scaleX += 10 * gameContext.getDeltaTime();
            notificationDrawable.scaleY += 10 * gameContext.getDeltaTime();
            notificationDrawable.scaleZ += 10 * gameContext.getDeltaTime();

            if(notificationDrawable.scaleY > 1.5) {
                notificationDrawable.scaleX = 2.5;
                notificationDrawable.scaleY = 1.5;
                notificationDrawable.scaleZ = 1;
            }
        }*/

        gameContext.getWindowFacadeRef().updateEntities(gameContext, {notifications[i].id});
    }
}

void BoxesSystem::removeNotificationsByLifeTime(GameEngine &gameContext) const {
    auto &allNotifications = gameContext.entityMan.getComponents<NotificationComponent>();
    std::vector<uint32_t> notificationsIdToDelete;

    for(auto &notification : allNotifications) {
        notification.lifeTime -= gameContext.getDeltaTime();
        if(notification.lifeTime <= 0) {
            notificationsIdToDelete.emplace_back(notification.id);
        }
    }

    for (size_t i = 0; i < notificationsIdToDelete.size(); ++i) {
        gameContext.eraseEntityByID(notificationsIdToDelete[i]);
    }
}

