#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class BoxesSystem : public System {
public:
    BoxesSystem();
    ~BoxesSystem();

    void update(GameEngine &gameContext) const override;
private:
	void drawInfoHUD(GameEngine &gameContext) const;
	void createInfoBoxes(GameEngine &gameContext, uint32_t entitySensored) const;
	void removeInfoBoxes(GameEngine &gameContext, uint32_t entitySensored) const;
	bool existsInfoBoxWithParent(GameEngine &gameContext, uint32_t parentId) const;
	void updateInfoBoxesApparition(GameEngine &gameContext) const;
	void updateInfoBoxesRotation(GameEngine &gameContext) const;

	void removeMessageBoxesByLifeTime(GameEngine &gameContext) const;
	void updateMessageBoxesRotation(GameEngine &gameContext) const;

	void removeNotificationsByLifeTime(GameEngine &gameContext) const;
	void updateNotificationApparition(GameEngine &gameContext) const;
	void updateNotificationsRotation(GameEngine &gameContext) const;

};