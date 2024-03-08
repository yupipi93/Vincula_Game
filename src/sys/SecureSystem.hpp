#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class SecureSystem : public System {
public:
    SecureSystem();
    ~SecureSystem();

    void update(GameEngine &gameContext) const override;

private:
	void setActiveTexture(GameEngine &gameContext, uint32_t playerInSecureZoneId) const;
    uint32_t checkPlayerIsSecure(GameEngine &gameContext, uint32_t playerId) const;


};