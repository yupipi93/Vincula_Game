#include "InfoNotificationSystem.hpp"

#include <tools/Utils.hpp>
#include <eng/GameEngine.hpp>


//console.log

//#####################################################
//                == INFO ACTION SYSTEM ==
//  -
//
//#####################################################


InfoNotificationSystem::InfoNotificationSystem() {}

InfoNotificationSystem::~InfoNotificationSystem() {}

void InfoNotificationSystem::update(GameEngine &gameContext) const {
    bool someActiveNotification = false;

    for (int i = 0; i < gameContext.activeInfoNotifications.size(); i++) {
        if (gameContext.activeInfoNotifications[i]) {
            someActiveNotification = true;
            break;
        }
    }


    if(someActiveNotification && gameContext.notificationTime > 0.f)
        gameContext.notificationTime -= gameContext.getDeltaTime();
    else
        Utils::resetInfoNotifications(gameContext);
}