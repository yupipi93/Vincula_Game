#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InventorySystem : public System {
public:
    InventorySystem();
    ~InventorySystem();

    void update(GameEngine &gameContext) const override;
private:
	void checkCatchingWeapons(GameEngine &gameContext, InventoryComponent& playerInventory, InteractionComponent& playerInteraction) const;
	void checkCatchingKey(GameEngine &gameContext, InventoryComponent& playerInventory, InteractionComponent& playerInteraction) const;
	void weaponCatchedToDefaultPosition(GameEngine &gameContext, SituationComponent &weaponSit, uint32_t idWeapon) const;
	void eraseIluminationLight(GameEngine &gameContext, uint32_t entityId) const;
};