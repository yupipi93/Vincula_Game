#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
#include <tools/Storage.hpp>

class AIDirectorSystem : public System {
public:
    AIDirectorSystem(GameEngine &gameContext);
    ~AIDirectorSystem();

    void update(GameEngine &gameContext) const override;

private:

    bool determinateEnemyCreation(GameEngine &gameContext, uint8_t currentBiome, DirectorDataComponent &directorData, uint32_t playerId) const;
    void  createEnemy(GameEngine &gameContext, DirectorDataComponent &directorData, uint32_t currentBiome, float posSpawnx, float posSpawnz) const;
    void  calculateSpawnPosition(GameEngine &gameContext, SituationComponent &playerSituation, float &posSpawnx, float &posSpawnz) const;
    GameObjectType calculateEnemyTypeByProbability(std::vector<enemyPercent> enemysInBiome) const;
};