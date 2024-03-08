#pragma once

#include "System.hpp"
#include <tools/Storage.hpp>

class GenerateZoneSystem : public System {
public:
    GenerateZoneSystem();
    ~GenerateZoneSystem();

    void update(GameEngine &gameContext) const override;

private:
    void deleteEnemies(GameEngine &gameContext, Storage<EnemyComponent> &allEnemies, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const;
    uint8_t getBiomeByEnemyType(GameObjectType type) const;
    void deleteCoins(GameEngine &gameContext, Storage<CoinComponent> &allCoins, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const;
    void meleeWeaponOutOfGenerateZone(GameEngine &gameContext, Storage<MeleeWeaponComponent> &allMeleeWeapons, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const;
    void distanceWeaponOutOfGenerateZone(GameEngine &gameContext, Storage<DistanceWeaponComponent> &allMeleeWeapons, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const;
};