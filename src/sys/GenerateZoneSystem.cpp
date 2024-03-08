#include "GenerateZoneSystem.hpp"
#include <tools/Utils.hpp>
//console.log

//#####################################################
//               == GENERATE ZONE SYSTEM ==
//  - ????????????????????
//  - ????????????????????
//  - ????????????????????
//
//   Using Componets:
//      - ??????    (Read - Write)
//      - ??????    (Read - Write)
//      - ??????    (Write)
//
//#####################################################

GenerateZoneSystem::GenerateZoneSystem() {}

GenerateZoneSystem::~GenerateZoneSystem() {}

void GenerateZoneSystem::update(GameEngine &gameContext) const {
    float terrainPosW;
    float terrainPosE;
    float terrainPosN;
    float terrainPosS;

    Utils::calculateTerrainLimits(terrainPosW, terrainPosE, terrainPosN, terrainPosS, gameContext);

    auto &allEnemies = gameContext.entityMan.getComponents<EnemyComponent>();
    deleteEnemies(gameContext, allEnemies, terrainPosW, terrainPosE, terrainPosN, terrainPosS);

    auto &allCoins = gameContext.entityMan.getComponents<CoinComponent>();
    deleteCoins(gameContext, allCoins, terrainPosW, terrainPosE, terrainPosN, terrainPosS);

    auto &allMeleeWeapons = gameContext.entityMan.getComponents<MeleeWeaponComponent>();
    meleeWeaponOutOfGenerateZone(gameContext, allMeleeWeapons, terrainPosW, terrainPosE, terrainPosN, terrainPosS);

    auto &allDistanceWeapons = gameContext.entityMan.getComponents<DistanceWeaponComponent>();
    distanceWeaponOutOfGenerateZone(gameContext, allDistanceWeapons, terrainPosW, terrainPosE, terrainPosN, terrainPosS);


    //OBSTACLES se eliminan en ObstacleSystem porque solo se eliminan cuando el terreno se mueve (es decir, menos comprobaciones)
}

void GenerateZoneSystem::meleeWeaponOutOfGenerateZone(GameEngine &gameContext, Storage<MeleeWeaponComponent> &allMeleeWeapons, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const {

    for(auto &meleeWeapon : allMeleeWeapons) {
        SituationComponent   &meleeWeaponSit  = gameContext.entityMan.getComponent<SituationComponent>(meleeWeapon.id);
        MeleeWeaponComponent &meleeWeaponComp = gameContext.entityMan.getComponent<MeleeWeaponComponent>(meleeWeapon.id);

        if(meleeWeaponSit.x < terrainPosW || meleeWeaponSit.x > terrainPosE || meleeWeaponSit.z > terrainPosN || meleeWeaponSit.z < terrainPosS) {
            meleeWeaponSit.x = meleeWeaponComp.defaultPosition[0];
            meleeWeaponSit.y = meleeWeaponComp.defaultPosition[1];
            meleeWeaponSit.z = meleeWeaponComp.defaultPosition[2];
        }
    }
}

void GenerateZoneSystem::distanceWeaponOutOfGenerateZone(GameEngine &gameContext, Storage<DistanceWeaponComponent> &allDistanceWeapons, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const {

    for(auto &distanceWeapon : allDistanceWeapons) {
        SituationComponent      &distanceWeaponSit     = gameContext.entityMan.getComponent<SituationComponent>(distanceWeapon.id);
        DistanceWeaponComponent &distanceWeaponComp = gameContext.entityMan.getComponent<DistanceWeaponComponent>(distanceWeapon.id);

        if(distanceWeaponSit.x < terrainPosW || distanceWeaponSit.x > terrainPosE || distanceWeaponSit.z > terrainPosN || distanceWeaponSit.z < terrainPosS) {
            distanceWeaponSit.x = distanceWeaponComp.defaultPosition[0];
            distanceWeaponSit.y = distanceWeaponComp.defaultPosition[1];
            distanceWeaponSit.z = distanceWeaponComp.defaultPosition[2];
        }
    }
}

void GenerateZoneSystem::deleteEnemies(GameEngine &gameContext, Storage<EnemyComponent> &allEnemies, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const {
    uint32_t AIDirectorId = gameContext.aIDirectorId;
    DirectorDataComponent &directorData = gameContext.entityMan.getComponent<DirectorDataComponent>(AIDirectorId);

    std::vector<uint32_t> enemiesToErase;

    for(auto &enemy : allEnemies) {
        SituationComponent &enemySit = gameContext.entityMan.getComponent<SituationComponent>(enemy.id);

        if( enemySit.x < terrainPosW || enemySit.x > terrainPosE || enemySit.z > terrainPosN || enemySit.z < terrainPosS) {
            enemiesToErase.emplace_back(enemy.id);

            auto type = gameContext.getEntity(enemy.id).getGameObjectType();
            uint8_t biome = getBiomeByEnemyType(type);
            --directorData.enemiesSpawned[biome];
        }
    }

    for(size_t i = 0; i < enemiesToErase.size(); ++i) {
        gameContext.eraseEntityByID(enemiesToErase[i]);
    }
}

uint8_t GenerateZoneSystem::getBiomeByEnemyType(GameObjectType type) const {
    uint8_t biome = 0;

    switch (type) {
    // case OTHER_OF_BIOME_1:
    // NO BREAK!!
    case ENEMY_PIG:
    case ENEMY_RODENT:
        return 1;
        break;
    case ENEMY_PINEAPPLE:
        return 2;
        break;
    case ENEMY_SPIDER:
        return 3;
        break;
    case ENEMY_MONKEY:
        return 4;
        break;
    case ENEMY_GHOST:
        return 5;
        break;
    }

    return biome;
}

void GenerateZoneSystem::deleteCoins(GameEngine &gameContext, Storage<CoinComponent> &allCoins, float terrainPosW, float terrainPosE, float terrainPosN, float terrainPosS) const {
    std::vector<uint32_t> coinsToErase;

    for(auto &coin : allCoins) {
        SituationComponent &coinSit = gameContext.entityMan.getComponent<SituationComponent>(coin.id);

        if( coinSit.x < terrainPosW || coinSit.x > terrainPosE || coinSit.z > terrainPosN || coinSit.z < terrainPosS) {
            coinsToErase.emplace_back(coin.id);
        }
    }

    for(size_t i = 0; i < coinsToErase.size(); ++i) {
        gameContext.eraseEntityByID(coinsToErase[i]);
    }
}
