#include "AIDirectorSystem.hpp"

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>
//console.log


//#####################################################
//                == AIDirector SYSTEM ==
//  - Creates enemies depending by the position of the player
//
//   Using Componets:
//      - Situation      (Read)
//      - DirectorData   (Read)
//      - Velocity       (Read)
//#####################################################

AIDirectorSystem::AIDirectorSystem(GameEngine &gameContext) {}

AIDirectorSystem::~AIDirectorSystem() {}


void AIDirectorSystem::update(GameEngine &gameContext) const {
    // -------------- GET DIRECTOR DATA COMPONENT AND PLAYER SITUATION, VELOCITY AND STATS

    uint32_t AIDirectorId = gameContext.aIDirectorId;
    DirectorDataComponent &directorData = gameContext.entityMan.getComponent<DirectorDataComponent>(AIDirectorId);
    uint32_t playerId = gameContext.playerId;
    SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(playerId);
    VelocityComponent &playerVelocity = gameContext.entityMan.getComponent<VelocityComponent>(playerId);

    // -------------- CALCULATE SPAWN POSITION
    float posSpawnx;
    float posSpawnz;
    calculateSpawnPosition(gameContext, playerSituation, posSpawnx, posSpawnz);

    // -------------- CALCULATE CURRENT Biome
    uint8_t currentBiome = Utils::calculateBiome(gameContext, posSpawnx, posSpawnz);

    // -------------- DETERMINATE ENEMY CREATION
    bool spawn = determinateEnemyCreation(gameContext, currentBiome, directorData, playerId);

    if (spawn) {
        createEnemy(gameContext, directorData, currentBiome, posSpawnx, posSpawnz);
        directorData.spawnCurrentTimer[currentBiome] = directorData.spawnTimer[currentBiome];
        ++directorData.enemiesSpawned[currentBiome];
    } else {
        float &timer = directorData.spawnCurrentTimer[currentBiome];
        timer -= gameContext.getDeltaTime();
        if (timer < 0)
            timer = 0;
    }
}

bool AIDirectorSystem::determinateEnemyCreation(GameEngine &gameContext, uint8_t currentBiome, DirectorDataComponent &directorData, uint32_t playerId) const {
    bool spawn = false;
    AtributesComponent &playerAtrib {gameContext.entityMan.getComponent<AtributesComponent>(playerId)};

    if (currentBiome && directorData.spawnCurrentTimer[currentBiome] <= 0
            && directorData.enemiesSpawned[currentBiome] < directorData.maxEnemiesSpawned[currentBiome]
            && !playerAtrib.isSecure)
        spawn = true;

    return spawn;
}

void AIDirectorSystem::calculateSpawnPosition(GameEngine &gameContext, SituationComponent &playerSituation, float &posSpawnx, float &posSpawnz) const {
    float west, east, north, south;
    float view_west, view_east, view_north, view_south;
    Utils::calculateTerrainLimits(west, east, north, south, gameContext);
    Utils::calculateVisibleZoneLimits(view_west, view_east, view_north, view_south, gameContext);
    view_west -= 10;
    view_north += 10;
    view_south -= 10;
    view_east += 10;
    /*

    size_X: 410
    size_view_X: 320
    min_view_X: 45
    max_view_X: 365
    size_Z: 340
    size_view_Z: 240
    min_view_Z: 50
    max_view_Z: 290

         __ __ __ __ __ __ __ __ __ __170
        |__|__|__|__|__|__|__|__|__|
        |__|__|__|__|__|__|__|__|__|_120
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|00
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|_-120
        |__|__|__|__|__|__|__|__|__|
        |__|__|__|__|__|__|__|__|__|__-170
        |     |      00      |     |
        |   -160            160    |
      -205                        205


        |__|__|__|__|__|__|__|__|__|size_Z
        |__|__|__|__|__|__|__|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|max_view_Z
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|
        |__|__|xx|xx|xx|xx|xx|__|__|min_view_Z
        |__|__|__|__|__|__|__|__|__|
        |__|__|__|__|__|__|__|__|__|0
        |     |              |     |
        | min_view_X    max_view_X |
        0                       size_X
    */

    int size_X = east - west;
    int size_view_X = view_east - view_west;
    int min_view_X = (size_X - size_view_X) >> 1;
    int max_view_X = size_X - min_view_X;

    int size_Z = north - south;
    int size_view_Z = view_north - view_south;
    int min_view_Z = (size_Z - size_view_Z) >> 1;
    int max_view_Z = size_Z - min_view_Z;


    int quadX = rand() % size_X;
    int quadZ;

    if (quadX >= min_view_X && quadX <= max_view_X) {

        int aux = rand() % 2;
        if (aux)
            quadZ = size_Z - (rand() % min_view_Z);
        else
            quadZ = rand() % min_view_Z;

    } else {
        quadZ = rand() % size_Z;
    }

    quadX -= (size_X >> 1);
    quadZ -= (size_Z >> 1);

    posSpawnx = quadX + playerSituation.x;
    posSpawnz = quadZ + playerSituation.z;
}

void AIDirectorSystem::createEnemy(GameEngine &gameContext, DirectorDataComponent &directorData, uint32_t currentBiome, float posSpawnx, float posSpawnz) const {

    GameObjectType enemyType;
    switch (currentBiome) {
    case 1:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome1);
        break;

    case 2:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome2);
        break;

    case 3:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome3);
        break;

    case 4:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome4);
        break;

    case 5:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome5);
        break;

    default:
        enemyType = calculateEnemyTypeByProbability(directorData.enemysInBiome1);
        break;

    }
    uint32_t enemyId = gameContext.entityMan.createEnemy(gameContext, posSpawnx, 0, posSpawnz, 0.f, enemyType);
}


GameObjectType AIDirectorSystem::calculateEnemyTypeByProbability(std::vector<enemyPercent> enemysInBiome) const {
    GameObjectType enemyType;

    bool enemyFound = false;
    uint8_t compare = 0;
    uint8_t numP = rand() % 100 + (0);
    for(int i = 0; i < enemysInBiome.size() && !enemyFound; i++) {
        compare += enemysInBiome[i].percent;
        if(numP < compare) {
            enemyType = enemysInBiome[i].enemyType;
            enemyFound = true;
        }
    }

    return enemyType;
}
