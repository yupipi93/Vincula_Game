#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class ObstacleSystem : public System {
public:
    ObstacleSystem();
    ~ObstacleSystem();

    void update(GameEngine &gameContext) const override;

private:
    void adminObstacles(GameEngine &gameContext, uint32_t terrainId) const;

    void generateObstaclesX(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation, float posObjX) const;
    void generateObstaclesZ(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation, float posObjZ) const;
    
    void createObstacleInBioma(GameEngine &gameContext, float posX, float posY, float posZ) const;
    void deleteObstacles(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation) const;

    //LODO (TEMPORALMENTE DESCARTADO)
    /*
    void     addWeaponToObstacleIfMust(GameEngine &gameContext, GameObjectType obstacleType, uint32_t obstacleId) const;
    void     updateObstaclesWeapons(GameEngine &gameContext) const; 
    uint32_t meleeObstacleAttack(GameEngine &gameContext, MeleeWeaponComponent &meleeWeapon, SituationComponent &obstacleSituation, uint32_t attackerId, float lifeTime) const;
    */
};