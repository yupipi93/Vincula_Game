#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class DeathSystem : public System {
public:
    DeathSystem();
    ~DeathSystem();

    void update(GameEngine &gameContext) const override;

private:
    std::vector<uint32_t> getDeathEntities(GameEngine &gameContext) const;
    void actionPostMortem(GameEngine &gameContext, uint32_t entityId, std::vector<uint8_t> &enemiesSpawned) const;
    void deleteEntity(GameEngine &gameContext, uint32_t entityId) const;
    void playerActionPostMortem(GameEngine &gameContext, uint32_t playerId) const;
    void eggActionPostMortem(GameEngine &gameContext, uint32_t eggId) const;

};