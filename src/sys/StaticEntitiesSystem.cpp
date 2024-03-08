#include "StaticEntitiesSystem.hpp"

#include <math.h>

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

#include <enum/EnumsIncluder>

//console.log


//#####################################################
//              == STATIC ENTITIES SYSTEM ==
//  - Generate Wall
//  - Create Base
//  - Create Weapon Updater
//  - Create Stats Updater
//  - Create Terrain
//  - Create TerrainBlocks
//  - Reset  Terrain and blocks
//
//   Using Componets:
//      - Sensor                            (Read - Write) //Temp
//      - All of terrain and its blocks     (Read - Write)
//#####################################################


StaticEntitiesSystem::StaticEntitiesSystem() {}

StaticEntitiesSystem::~StaticEntitiesSystem() {}

void StaticEntitiesSystem::init(GameEngine &gameContext) const {

    //######## STATIC ENTITIES ########//

    createMap(gameContext); // needs to be the first

    createBank(gameContext);

    createHud(gameContext);

    createTerrain(gameContext);

    createPlayer(gameContext);//need first create map

    createCamera(gameContext);

}

void StaticEntitiesSystem::postInit(GameEngine &gameContext) const {
    createVillage(gameContext);
    createNPCImproverAndEnvironment(gameContext);
    createWallControls(gameContext);
    createAIDirector(gameContext);
    createObstacleData(gameContext);
    generateWall(gameContext);
    generateBiomeStructures(gameContext);
    createWeapons(gameContext); //###Temporally in this system
    createSource(gameContext);
    createBaseNPCs(gameContext);
    createMusicEvents(gameContext);
    createSun(gameContext);
    createMouseLight(gameContext);
}

void StaticEntitiesSystem::createMusicEvents(GameEngine &gameContext) const {
    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, MUSIC_AMBIENT);
    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, AMBIENT_SFX);
    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, SOUNT_HEART);

    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, PLAYER_ACTIONS);
    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, WIN_SOUND_SHOOT);
    gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, GAMEOVER_SOUND_SHOOT);
    //gameContext.entityMan.createSoundEvent(gameContext, 0.f, 0.f, 0.f, 0.f, COMPASS_SOUND);



};
void StaticEntitiesSystem::createVillage(GameEngine &gameContext) const {
    gameContext.entityMan.createVillageObstacle(gameContext, 120.f, 0.f, 85.f, -38.f, VILLAGE_OBSTACLE_SCARECROW);
    gameContext.entityMan.createVillageObstacle(gameContext, -115.f, 0.f, -100.f, 38.f, VILLAGE_OBSTACLE_SCARECROW);
    gameContext.entityMan.createVillageObstacle(gameContext, 0.f, 0.f, 0.f, 0.f, VILLAGE_OBSTACLE_FENCE);
    gameContext.entityMan.createVillageObstacle(gameContext, 0.f, 0.f, -300.f, 0.f, VILLAGE_OBSTACLE_WINDMILL);
    //gameContext.entityMan.createVillageObstacle(gameContext, 0.f, 0.f, 150.f, 0.f, VILLAGE_OBSTACLE_WELL);
    gameContext.entityMan.createVillageObstacle(gameContext, -240.f, 0.f, 300.f, 0.f, VILLAGE_OBSTACLE_WATERMILL);
    gameContext.entityMan.createVillageObstacle(gameContext, -240.f, 0.f, 0.f, 0.f, VILLAGE_OBSTACLE_HOUSE1);
    gameContext.entityMan.createVillageObstacle(gameContext, 350.f, 0.f, 48.f, 180.f, VILLAGE_OBSTACLE_HOUSE1);
    gameContext.entityMan.createVillageObstacle(gameContext, 0.f, 0.f, 290.f, 0.f, VILLAGE_OBSTACLE_MEDICTEND);

    //END OF B1
    gameContext.entityMan.createVillageObstacle(gameContext, 0.f, 0.f, 0.f, 0.f, VILLAGE_OBSTACLE_DANGER);

    //TEMP: Armas en el pueblo

    if (gameContext.debuging) {
        gameContext.entityMan.createMeleeWeapon(gameContext, -50, 3.5, 50, 0, MELEE_STICK);
        gameContext.entityMan.createDistanceWeapon(gameContext, 50, 3.5, 50, 0, DISTANCE_SLINGSHOT);
        gameContext.entityMan.createMeleeWeapon(gameContext, 0, 3.5, -70, 0, MELEE_MACE);
        gameContext.entityMan.createDistanceWeapon(gameContext, 50, 3.5, -50, 0, DISTANCE_BLOWGUN);
        gameContext.entityMan.createMeleeWeapon(gameContext, 70, 3.5, 0, 0, MELEE_SWORD);
        gameContext.entityMan.createDistanceWeapon(gameContext, -70, 3.5, 0, 45, DISTANCE_BOW);
    }
}
void StaticEntitiesSystem::createMap(GameEngine &gameContext) const {
    gameContext.entityMan.createMap         (gameContext, 0.f, 0.f, 0.f, 0.f);

    auto &map = gameContext.entityMan.getComponent<MapComponent>(gameContext.mapId);

    // FILL MAP WITH OUTER ZONES
    float radius = map.MAX_MAP_DIVISION / 2;
    for (size_t row = 0; row < map.MAX_MAP_DIVISION; ++row)
        for (size_t column = 0; column < map.MAX_MAP_DIVISION; ++column) {
            auto fixedRow = radius - row;
            auto fixedCol = radius - column;
            float distance = sqrt(fixedRow * fixedRow + fixedCol * fixedCol) - 1;
            if (distance > radius)
                map.discoverArea(map.MAX_MAP_DIVISION * row + column);
        }
}

void StaticEntitiesSystem::createCamera(GameEngine &gameContext) const {
    gameContext.entityMan.createCamera      (gameContext, 0.f, 0.f, 0.f, 0.f);
}

void StaticEntitiesSystem::createBank(GameEngine &gameContext) const {
    float pos = 150.f;
    float actionSeparation = 50.f;
    gameContext.entityMan.createBank        (gameContext, 0.f, 0.f,  pos, 0.f);
    gameContext.entityMan.createBankDepositor(gameContext, -actionSeparation, 0.f, pos, 0.f);
    gameContext.entityMan.createBankExtractor(gameContext, actionSeparation, 0.f, pos, 0.f);
    gameContext.entityMan.createMessageBox  (gameContext, 0.f, 80.f, pos, 0, MESSAGEBOX_BANK);
}

void StaticEntitiesSystem::createNPCImproverAndEnvironment(GameEngine &gameContext) const {
    gameContext.entityMan.createNPC(gameContext, 150, 0, 0, 180, NPC_SHAMAN_B0_STATSUPDATER);
    gameContext.entityMan.createDetectionSign(gameContext, 150, 55, 0, 0, DETECTIONSIGN_IMPROVER);

    gameContext.entityMan.createVillageObstacle(gameContext, 150, 0, 0, 0, VILLAGE_OBSTACLE_SHAMAN_ENVIRONMENT);
    //gameContext.entityMan.createMessageBox  (gameContext, 150.f, 70.f, 0.f, 90.f, MESSAGEBOX_UPDATER);
}

void StaticEntitiesSystem::createHud(GameEngine &gameContext) const {
    gameContext.entityMan.createHUD         (gameContext, 0.f, 0.f, 0.f, 0.f);
}

void StaticEntitiesSystem::createWallControls(GameEngine &gameContext) const {
    gameContext.entityMan.createControlsFoor(gameContext, 0.f, 0.1, -20.f, 0.f);
}
void StaticEntitiesSystem::createPlayer(GameEngine &gameContext) const {
    gameContext.entityMan.createPlayer      (gameContext, -240.f, 0.f, -190.f, 0.f);
    MapComponent &map = gameContext.entityMan.getComponent<MapComponent>(gameContext.mapId);
    map.discoverEntity(gameContext.playerId);
}
void StaticEntitiesSystem::createBaseNPCs(GameEngine &gameContext) const {
    //NPCs in BASE
    float posX;
    float posZ;
    uint32_t npcId;
    uint32_t interrogationId;

    posX = -255;
    posZ = -205;
    npcId = gameContext.entityMan.createNPC(gameContext, posX, 0, posZ, 0, NPC_OLDMAN_B0_STICK);
    interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);

    DialogueComponent &dialogue = gameContext.entityMan.getComponent<DialogueComponent>(npcId);
    dialogue.interrogation = interrogationId;



    posX = -25;
    posZ = 225;
    npcId = gameContext.entityMan.createNPC(gameContext, posX, 0, posZ, 0, NPC_MOTHER_B0_INFORMATION);
    interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);

    DialogueComponent &dialogue2 = gameContext.entityMan.getComponent<DialogueComponent>(npcId);
    dialogue2.interrogation = interrogationId;



    posX = -50;
    posZ = -50;
    npcId = gameContext.entityMan.createNPC(gameContext, posX, 0, posZ, 45, NPC_GUITARIST_B0_COMPASS);
    interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);

    DialogueComponent &dialogue3 = gameContext.entityMan.getComponent<DialogueComponent>(npcId);
    dialogue3.interrogation = interrogationId;
}

void StaticEntitiesSystem::createSource(GameEngine &gameContext) const {
    uint32_t terrainGeneratorId = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainGeneratorId);
    uint16_t baseSize  = terrainData.baseSize;

    gameContext.entityMan.createSource(gameContext, 0.f, 0.f, 0.f, 0.f);

    uint32_t villageZoneId = gameContext.entityMan.createSecureZone(gameContext, 0.f, 1.f, 0.f, 0.f, SECURE_ZONE_TOWN);
}

void StaticEntitiesSystem::createAIDirector(GameEngine &gameContext) const {
    gameContext.entityMan.createAIDirector(gameContext, 0.f, 0.f, 0.f, 0.f);
}

void StaticEntitiesSystem::createObstacleData(GameEngine &gameContext) const {
    gameContext.entityMan.createObstacleData(gameContext, 0.f, 0.f, 0.f, 0.f);
}

void StaticEntitiesSystem::createWeapons(GameEngine &gameContext) const {
    //##MELEE
    /*gameContext.entityMan.createMeleeWeapon(gameContext, 70, 0, 50, 0.f, MELEE_SWORD);
    gameContext.entityMan.createMeleeWeapon(gameContext, 70, 0, 10, 0.f, MELEE_MACE);
    gameContext.entityMan.createMeleeWeapon(gameContext, 70, 0, -30, 0.f, MELEE_SPEAR);*/

    //##DISTANCE
    /*gameContext.entityMan.createDistanceWeapon(gameContext, -100, 0, 80, 0.f, DISTANCE_BLOWGUN);
    gameContext.entityMan.createDistanceWeapon(gameContext, -100, 0, 50, 0.f, DISTANCE_SLINGSHOT);
    gameContext.entityMan.createMeleeWeapon(gameContext, -100, 0, 120, 0.f, MELEE_MACE);*/
}

void StaticEntitiesSystem::createSun(GameEngine &gameContext) const {
    gameContext.entityMan.createLight(gameContext, 0.f, 0.f, 0.f, 0.f, LIGHT_DIRECTIONAL);
}


void StaticEntitiesSystem::update(GameEngine &gameContext) const {}

void StaticEntitiesSystem::generateWall(GameEngine &gameContext) const {


    uint32_t terrainGeneratorId = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainGeneratorId);
    uint16_t biomeSize = terrainData.biomeSize;
    uint8_t lastBiome = terrainData.lastBiome;
    uint8_t wallSize = terrainData.wallSize;
    uint8_t wallHeight = terrainData.wallHeight;
    uint8_t wallCollidableSize = terrainData.wallCollidableSize;
    uint8_t collidablesPerWall = terrainData.collidablesPerWall;


    // Wall Drawables creation

    int radius = (terrainData.lastBiome) * terrainData.biomeSize + terrainData.baseSize;
    float diam = 2 * 3.14159 * radius;
    float numCubes = diam / wallSize;
    float rotVariation = (2 * 3.14159) / numCubes;

    uint32_t posIterationDoor = rand() % (int)numCubes;

    //posIterationDoor = numCubes;


    float firstRot = rand() % 360;
    float currRot = firstRot;
    float x, z;

    for(int i = 0; i <= (int)numCubes; ++i) {
        currRot += rotVariation;
        x = radius * cos(currRot);
        z = radius * sin(currRot);

        float rotDeg = Utils::radToDeg(currRot);

        if(i == posIterationDoor) {
            gameContext.entityMan.createDoor(gameContext, x, 0, z, rotDeg);
        } else {
            gameContext.entityMan.createWall(gameContext, x, 0, z, rotDeg);
        }


    }


    // Wall collidables creation
    //radius = biomeSize * (lastBiome + 1);
    radius += 10;
    diam = 2 * 3.14159 * radius;
    numCubes = diam / wallCollidableSize;
    rotVariation = (2 * 3.14159) / numCubes;

    currRot = firstRot;



    for(int i = 0; i <= (int)numCubes; ++i) {
        currRot += rotVariation;
        x = radius * cos(currRot);
        z = radius * sin(currRot);

        float rotDeg = Utils::radToDeg(currRot);
        gameContext.entityMan.createWallCollidable(gameContext, x, 0, z, rotDeg);
    }

}


void StaticEntitiesSystem::generateBiomeStructures(GameEngine &gameContext) const {
    // Vector to see where we have created structures
    std::vector<std::vector<bool>> usedQuadrants; // [biome] [Quadrant]

    // Vector to stablish which structures we have to create
    std::vector<std::vector<GameObjectType>> entToGenerate; // [biome] [GameObjectType]

    // Initialize vectors
    setEntitiesToCreate(gameContext, usedQuadrants, entToGenerate);

    // Generate the structures in its positions
    setEntitiesInPosition(gameContext, usedQuadrants, entToGenerate);
}



void StaticEntitiesSystem::setEntitiesToCreate(GameEngine &gameContext, std::vector< std::vector<bool> > &usedQuadrants, std::vector< std::vector<GameObjectType> > &entToGenerate) const {
    // Manually fill
    // First biome with structures (PRADERA PLACENTERA)
    entToGenerate.emplace_back(std::initializer_list<GameObjectType> {INTERESTPOINT_B1_1, INTERESTPOINT_B1_2, INTERESTPOINT_B1_3, EGG_BIOME1});
    // (BOSQUE PINOSO)
    entToGenerate.emplace_back(std::initializer_list<GameObjectType> {INTERESTPOINT_B2_1, INTERESTPOINT_B2_2, INTERESTPOINT_B2_3, EGG_BIOME2});
    // (CIENAGA VENENOSA)
    entToGenerate.emplace_back(std::initializer_list<GameObjectType> {INTERESTPOINT_B3_1, INTERESTPOINT_B3_2, INTERESTPOINT_B3_3, EGG_BIOME3});
    // (SELVA TROPICAL)
    entToGenerate.emplace_back(std::initializer_list<GameObjectType> {INTERESTPOINT_B4_1, INTERESTPOINT_B4_2, INTERESTPOINT_B4_3, EGG_BIOME4});
    // (LA NADA)
    entToGenerate.emplace_back(std::initializer_list<GameObjectType> {INTERESTPOINT_B5_1, INTERESTPOINT_B5_2, INTERESTPOINT_B5_3, EGG_BIOME5});

    // Automatically fill the usedQuadrantsVector
    usedQuadrants.resize(entToGenerate.size());
    for (size_t biome = 0; biome < entToGenerate.size(); ++biome)
        for (auto _ : entToGenerate[biome])
            usedQuadrants[biome].emplace_back(false);
}

void StaticEntitiesSystem::setEntitiesInPosition(GameEngine &gameContext, std::vector< std::vector<bool> > &usedQuadrants, std::vector< std::vector<GameObjectType> > &entToGenerate) const {
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(gameContext.terrainId);
    uint16_t baseSize  = terrainData.baseSize;
    uint16_t biomeSize = terrainData.biomeSize;
    uint8_t INTEREST_POINT_SIZE = 250;
    uint16_t INTEREST_POINT_HALF_SIZE_SQARE = (INTEREST_POINT_SIZE / 2) * (INTEREST_POINT_SIZE / 2);

    for (uint8_t biome = 0; biome < entToGenerate.size(); ++biome) {
        float randomRotationInBiomeQuadrants = rand() % 90;
        for (auto &entityToGen : entToGenerate[biome]) {
            // get a free quadrant
            auto quadrantsForThisBiome = entToGenerate[biome].size();
            uint8_t quadrant  = rand() % quadrantsForThisBiome; // [0, quadrantsForThisBiome)
            while (usedQuadrants[biome][quadrant]) {
                ++quadrant;
                quadrant %= quadrantsForThisBiome; // [0, quadrantsForThisBiome)
            }
            // stablish that quadrant as used
            usedQuadrants[biome][quadrant] = true;

            // get positions for that quadrant
            float quadrantAngleRange        = 360 / quadrantsForThisBiome;
            float quadrantAngle             = (quadrantAngleRange) * quadrant + quadrantAngleRange / 2 + randomRotationInBiomeQuadrants;
            float quadrantAngleRangeMin     = quadrantAngle - quadrantAngleRange / 4;
            float quadrantAngleRangeMax     = quadrantAngle + quadrantAngleRange / 4;


            // the position where to create the estructure will be between the min and max
            // quadrant angle range
            float posX = 0;
            float posZ = 0;
            bool colides = false;
            do {
                colides = false;

                float positionAngle = (rand() % ((int)(quadrantAngleRangeMax - quadrantAngleRangeMin))) + quadrantAngleRangeMin;

                positionAngle = (int)positionAngle % 360;


                // and in a position between the start and end of the biome - the size of the IP
                float posDistanceRangeMin   = baseSize + biomeSize * biome + sqrt(2 * INTEREST_POINT_HALF_SIZE_SQARE);
                float posDistanceRangeMax   = posDistanceRangeMin + biomeSize - 2 * sqrt(2 * INTEREST_POINT_HALF_SIZE_SQARE);

                float posDistance           = (rand() % ((int)(posDistanceRangeMax - posDistanceRangeMin))) + posDistanceRangeMin;

                // once we have the angle and the distance, we can stablish the posX and posZ
                auto positionAngleAux   = (int)positionAngle % 90;

                posX = cos(positionAngleAux) * posDistance;
                posX = abs(posX);

                posZ = sin(positionAngleAux) * posDistance;
                posZ = abs(posZ);

                if ((positionAngle >= 90 && positionAngle <= 180) || (positionAngle >= 180 && positionAngle <= 270))
                    posZ *= -1;

                if ((positionAngle >= 270 && positionAngle <= 360) || (positionAngle >= 180 && positionAngle <= 270))
                    posX *= -1;

                //    static bool     isCollisionAB(float x1, float y1, float z1, float sizeX1, float sizeY1, float sizeZ1, SituationComponent &situation, BoundingComponent &bounding);

                auto &ipDatas = gameContext.entityMan.getComponents<IPDataComponent>();
                for (auto &ipData : ipDatas) {
                    auto &situation = gameContext.entityMan.getComponent<SituationComponent>(ipData.id);
                    auto &bounding = gameContext.entityMan.getComponent<CollisionComponent>(ipData.secureZoneId);
                    if (Utils::isCollisionAB(posX, 0, posZ, INTEREST_POINT_SIZE, 0, INTEREST_POINT_SIZE, situation, bounding)) {
                        colides = true;
                        break;
                    }
                }

            } while (colides);
            createEntitiesInPosition(gameContext, posX, posZ, biome + 1, entityToGen);
        }
    }
    //terminate();
}

void StaticEntitiesSystem::createEntitiesInPosition(GameEngine &gameContext, float posX, float posZ, int currentBiome, GameObjectType entToGenerate) const {
    uint32_t interestPId = 0;
    uint32_t zoneId = 0;
    uint32_t npcId = 0;
    uint32_t interrogationId = 0;

    switch(entToGenerate) {
    // BIOME 1:
    case INTERESTPOINT_B1_1:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ - 70, 0, INTERESTPOINT_B1_1_P1);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 100, 35, posZ + 100, 0, DETECTIONSIGN_INTERROGATION);
        createCoinChest(gameContext, posX + 100, 0, posZ + 100, 0, currentBiome, CHEST_COIN_CAR, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B1_2:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 60, 0, posZ - 90, 0, INTERESTPOINT_B1_2_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 60, 0, posZ + 70, 0, INTERESTPOINT_B1_2_P2);
        //NPC
        npcId = gameContext.entityMan.createNPC (gameContext, posX + 100, 0, posZ + 100, 0, NPC_FARMER_B1_SLINGSHOT);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 100, 50, posZ + 100, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B1_3:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 100, 35, posZ + 100, 0, DETECTIONSIGN_INTERROGATION);
        createCoinChest(gameContext, posX + 100, 0, posZ + 100, 0, currentBiome, CHEST_COIN_CAR, interrogationId);
        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case EGG_BIOME1:
        //NPC
        npcId = gameContext.entityMan.createNPC (gameContext, posX + 40, 0, posZ + 40, 0, NPC_GENTLEMAN_B1_KEY);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 40, 52, posZ + 40, 0, DETECTIONSIGN_INTERROGATION);
        gameContext.entityMan.createKeyEgg(gameContext, posX, 0, posZ, 0, entToGenerate);
        break;

    //BIOME 2:
    case INTERESTPOINT_B2_1: {
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 30, 0, posZ + 70, 0, INTERESTPOINT_B2_1_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 30, 0, posZ + 60, 0, INTERESTPOINT_B2_1_P2);
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 80, 0, posZ, 0, INTERESTPOINT_B2_1_P3);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ - 80, 0, INTERESTPOINT_B2_1_P4);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 80, 0, posZ, 0, INTERESTPOINT_B2_1_P5);
        //NPC
        npcId = gameContext.entityMan.createNPC (gameContext, posX, 0, posZ - 60, 0, NPC_COWBOY_B2_MAP);
        //npcId = gameContext.entityMan.createNPC (gameContext, -100, 0, 100, 0, NPC_COWBOY_B2_MAP);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 50, posZ - 60, 0, DETECTIONSIGN_INTERROGATION);

        //PALANTIR
        uint32_t id = gameContext.entityMan.createPalantir(gameContext, posX - 100, 0, posZ - 100, 0.f);
        gameContext.compassPointsTo = id;

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
    }
    break;

    case INTERESTPOINT_B2_2:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 100, 35, posZ - 100, 0, DETECTIONSIGN_INTERROGATION);
        createWeaponChest(gameContext, posX - 100, 0, posZ - 100, CHEST_WEAPON_MACE, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B2_3:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 90, 0, posZ, 0, INTERESTPOINT_B2_3_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ + 90, 0, INTERESTPOINT_B2_3_P2);
        //NPC
        npcId = gameContext.entityMan.createNPC       (gameContext, posX, 0, posZ - 70, 0, NPC_PIRATE_B2_COINS);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ - 70, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case EGG_BIOME2:
        gameContext.entityMan.createKeyEgg(gameContext, posX, 0, posZ, 0, entToGenerate);
        break;

    // BIOME 3
    case INTERESTPOINT_B3_1:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);

        //LATRINE CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 102, posZ, 0, DETECTIONSIGN_INTERROGATION);
        createCoinChest(gameContext, posX, 0, posZ, 0, currentBiome, CHEST_COIN_LATRINE, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B3_2:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //NPC
        npcId = gameContext.entityMan.createNPC(gameContext, posX + 60, 0, posZ + 60, 225, NPC_INDIAN_B3_BLOWGUN);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 60, 45, posZ + 60, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B3_3:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ, 0, INTERESTPOINT_B3_3_P1);
        
        //NPC
        npcId = gameContext.entityMan.createNPC(gameContext, posX - 100, 0, posZ - 100, 0, NPC_FUMIGATOR_B3_COINS);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 100, 45, posZ - 100, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case EGG_BIOME3:
        gameContext.entityMan.createKeyEgg(gameContext, posX, 0, posZ, 0, entToGenerate);
        break;

    // BIOME 4
    case INTERESTPOINT_B4_1:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 82, 0, posZ, 90, INTERESTPOINT_B4_1_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ - 82, 0, INTERESTPOINT_B4_1_P2);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ + 82, 0, INTERESTPOINT_B4_1_P3);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 100, 35, posZ, 0, DETECTIONSIGN_INTERROGATION);
        createCoinChest(gameContext, posX - 100, 0, posZ, 0, currentBiome, CHEST_COIN_CASTLE, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B4_2:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 7  * 10, 0, posZ + 8   * 10, 0, INTERESTPOINT_B4_2_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 6  * 10, 0, posZ + 8.5 * 10, 0, INTERESTPOINT_B4_2_P2);
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 10 * 10, 0, posZ - 5   * 10, 0, INTERESTPOINT_B4_2_P3);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ - 10  * 10, 0, INTERESTPOINT_B4_2_P4);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 10 * 10, 0, posZ - 4   * 10, 0, INTERESTPOINT_B4_2_P5);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 5, 40, posZ - 70, 0, DETECTIONSIGN_INTERROGATION);
        createWeaponChest(gameContext, posX, 0, posZ - 70, CHEST_WEAPON_BOW_SKELETON, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B4_3:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //NPC
        npcId = gameContext.entityMan.createNPC(gameContext, posX - 100, 0, posZ - 100, 0, NPC_ROBOT_B4_INFORMATION);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 100, 40, posZ - 100, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case EGG_BIOME4:
        gameContext.entityMan.createKeyEgg(gameContext, posX, 0, posZ, 0, entToGenerate);
        break;

    // BIOME 5
    case INTERESTPOINT_B5_1:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 100, 0, posZ + 100, 0, INTERESTPOINT_B5_1_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 100, 0, posZ + 100, 0, INTERESTPOINT_B5_1_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 100, 0, posZ - 100, 0, INTERESTPOINT_B5_1_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 100, 0, posZ - 100, 0, INTERESTPOINT_B5_1_P1);
        //NPC
        npcId = gameContext.entityMan.createNPC(gameContext, posX - 50, 0, posZ - 100, 0, NPC_SKELETON_B5_COINS);
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX - 50, 45, posZ - 100, 0, DETECTIONSIGN_INTERROGATION);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B5_2:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ, 0, entToGenerate);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX, 35, posZ - 70, 0, DETECTIONSIGN_INTERROGATION);
        createWeaponChest(gameContext, posX, 0, posZ - 70, CHEST_WEAPON_SWORD, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case INTERESTPOINT_B5_3:
        interestPId = gameContext.entityMan.createInterestPoint(gameContext, posX, 0, posZ + 90.f, 0, entToGenerate);
        //PARTS
        gameContext.entityMan.createInterestPointPart(gameContext, posX - 85, 0, posZ - 85, 0, INTERESTPOINT_B5_3_P1);
        gameContext.entityMan.createInterestPointPart(gameContext, posX + 85, 0, posZ - 85, 0, INTERESTPOINT_B5_3_P2);
        gameContext.entityMan.createInterestPointPart(gameContext, posX, 0, posZ - 40, 0, INTERESTPOINT_B5_3_P3);
        //CHEST
        interrogationId = gameContext.entityMan.createDetectionSign(gameContext, posX + 60, 35, posZ + 50, 0, DETECTIONSIGN_INTERROGATION);
        createCoinChest(gameContext, posX + 60, 0, posZ + 50, 0, currentBiome, CHEST_COIN_BAG, interrogationId);

        zoneId = gameContext.entityMan.createSecureZone(gameContext, posX, 0, posZ, SECURE_ZONE_IP);
        break;

    case EGG_BIOME5:
        gameContext.entityMan.createKeyEgg(gameContext, posX, 0, posZ, 0, entToGenerate);
        break;
    }

    if (interestPId && zoneId) {
        IPDataComponent &ipData = gameContext.entityMan.getComponent<IPDataComponent>(interestPId);
        ipData.secureZoneId = zoneId;
    }

    if (npcId && interrogationId) {
        DialogueComponent &dialogue = gameContext.entityMan.getComponent<DialogueComponent>(npcId);
        dialogue.interrogation = interrogationId;
    }

}


void StaticEntitiesSystem::createMouseLight(GameEngine &gameContext) const {
    gameContext.entityMan.mouseShape(gameContext, 0.f, 0.f, 0.f, 0.f, NONE);
    uint32_t lightId = gameContext.entityMan.createLight(gameContext, 0.f, 0.f, 0.f, 0.f, LIGHT_MOUSE);
    auto &lightComp = gameContext.entityMan.getComponent<LightComponent>(lightId);

    // Set point light data
    //1.f, 0.014f, 0.0007f
    lightComp.constant   = 1.f;
    lightComp.linear     = 0.045f;
    lightComp.quadratic  = 0.0075f;

    lightComp.color[0]   = 1.5;
    lightComp.color[1]   = 1.5;
    lightComp.color[2]   = 1.5;

    if(!gameContext.getWindowFacadeRef().existsNode(lightId)) {
        gameContext.getWindowFacadeRef().createPointLight(gameContext, lightId);
    }
}


void StaticEntitiesSystem::createCoinChest(GameEngine &gameContext, float posX, float posY, float posZ, float rot, size_t currentBiome, GameObjectType chestType,  uint32_t interrogationId) const {
    uint32_t chestId = gameContext.entityMan.createChest(gameContext, posX, posY, posZ, rot, chestType);
    auto &chestComp = gameContext.entityMan.getComponent<ChestComponent>(chestId);
    chestComp.interrogation = interrogationId;
}

void StaticEntitiesSystem::createWeaponChest(GameEngine &gameContext, float posX, float posY, float posZ, GameObjectType chestType, uint32_t interrogationId) const {
    uint32_t chestId = gameContext.entityMan.createChest(gameContext, posX, posY, posZ, 0.f, chestType);
    auto &chestComp = gameContext.entityMan.getComponent<ChestComponent>(chestId);

    chestComp.interrogation = interrogationId;
}




void StaticEntitiesSystem::createTerrain(GameEngine &gameContext) const {
    uint32_t terrainId = gameContext.entityMan.createTerrain(gameContext, 0, 0, 0, 0.f);
    auto &terrainDataComp = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    auto &terrainDataSit = gameContext.entityMan.getComponent<SituationComponent>(terrainId);

    uint8_t width     = terrainDataComp.terrainWidth;
    uint8_t height    = terrainDataComp.terrainHeight;
    float   cubeSize  = terrainDataComp.cubeSize;

    // position of the terrain, the left up corner
    float x = (width * cubeSize / 2) + cubeSize;
    float z = (height * cubeSize / 2) + cubeSize;

    terrainDataSit.x = x;
    terrainDataSit.y = 10;//THIS IS TO FIX UP 50 TO 60 terrainSize
    terrainDataSit.z = z;


    createCubes(gameContext);
}

void StaticEntitiesSystem::createCubes(GameEngine &gameContext) const {
    uint32_t terrainId = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);

    float desviationY = rand() % 5 + (-2.5); //entre -2.5 y -2.5
    float posX = 0.f;
    //float posY = terrainSituation.y + desviationY;
    float posY = terrainSituation.y - terrainData.cubeSize;
    float posZ = terrainSituation.z;
    std::string texture {};

    // now create all the cubes that fill the terrain
    for(int z = 0; z < terrainData.terrainHeight; ++z) {
        posX = terrainSituation.x;
        posZ -= terrainData.cubeSize;
        for(int x = 0; x < terrainData.terrainWidth; ++x) {
            texture = getCubeTexture(gameContext, posX, posZ);


            //float x, float y, float z, float r, std::string mod , std::string text, uint8_t *col
            //gameContext.CreateTerrainCube(posX, posY, posZ, 0.f, "", texture, nullptr);

            uint32_t cubeId = gameContext.entityMan.createTerrainCube(gameContext, posX, posY, posZ, 0.f);
            auto &drawableComp = gameContext.entityMan.getComponent<DrawableComponent>(cubeId);
            drawableComp.texture = texture;
            drawableComp.scaleX = terrainData.cubeSize / 10;
            drawableComp.scaleY = terrainData.cubeSize / 10;
            drawableComp.scaleZ = terrainData.cubeSize / 10;

            gameContext.getWindowFacadeRef().updateTextures(gameContext, {cubeId});

            desviationY = rand() % 5 + (-2.5); //entre -2.5 y -2.5
            //posY = terrainSituation.y + desviationY;
            posX += terrainData.cubeSize;
        }
    }
}

void StaticEntitiesSystem::resetTerrain(GameEngine &gameContext) const {
    std::vector<uint32_t> terrainCubesToErase;
    auto &terrainCubes = gameContext.entityMan.getComponents<TerrainCubeComponent>();
    uint32_t terrainId = gameContext.terrainId;

    // Delete all cubes
    for(auto &cube : terrainCubes) {
        terrainCubesToErase.emplace_back(cube.id);
    }
    for(size_t i = 0; i < terrainCubesToErase.size(); ++i) {
        gameContext.eraseEntityByID(terrainCubesToErase[i]);

    }

    // Delete terrain
    gameContext.eraseEntityByID( terrainId );

    // Create new terrain and cubes
    createTerrain(gameContext);
}

std::string StaticEntitiesSystem::getCubeTexture(GameEngine &gameContext, const float posX, const float posZ) const {
    uint8_t biome = 0;
    std::string texture {};

    biome = Utils::calculateBiome(gameContext, posX, posZ);
    if (biome > 10)
        biome = 10;

    uint8_t pn = rand() % 100 + 1;
    if(pn < 25)
        pn = 1;
    else if(pn >= 25 && pn < 50)
        pn = 2;
    else if(pn >= 50 && pn < 75)
        pn = 3;
    else if(pn >= 75)
        pn = 4;

    texture = Utils::getBiomeTexturePath(biome, pn);


    return texture;
}