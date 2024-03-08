#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InfoNotificationSystem : public System {
public:
    InfoNotificationSystem();
    ~InfoNotificationSystem();

    void update(GameEngine &gameContext) const override;

private:
	void resetInfoNotifications(GameEngine &gameContext) const;
};