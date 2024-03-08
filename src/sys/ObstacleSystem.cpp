#include "ObstacleSystem.hpp"

#include <tools/Utils.hpp>
//console.log

#include <eng/GameEngine.hpp>

constexpr uint8_t SPAWN_RATIO = 15;

//#####################################################
//                == OBSTACLE SYSTEM ==
//  - Creates and destroys obstacles while you move
//
//   Using Componets:
//      - TerrainDataComponent  (Read)
//      - SituationComponent    (Read - Write)
//
//#####################################################

ObstacleSystem::ObstacleSystem() {}

ObstacleSystem::~ObstacleSystem() {}


void ObstacleSystem::update(GameEngine &gameContext) const {

    // Get the terrain data
    uint32_t             terrainId    = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);

    // Check if the terrain cubes have moved
    if(terrainData.movedLeft || terrainData.movedRight || terrainData.movedUp || terrainData.movedDown) {
        adminObstacles(gameContext, terrainId);
    }

    //LODO (TEMPORALMENTE DESCARTADO)
    //updateObstaclesWeapons(gameContext);

}

void ObstacleSystem::adminObstacles(GameEngine &gameContext, uint32_t terrainId) const {
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    // Position where the obstacle has to be created
    float posObjToCreate = 0.f;
    float posObjToDelete = 0.f;

    if(terrainData.movedLeft) {
        posObjToCreate = terrainSituation.x - (terrainData.chunkWidthIncrement * terrainData.cubeSize);
        generateObstaclesX(gameContext, terrainData, terrainSituation, posObjToCreate);

    } else if(terrainData.movedRight) {
        posObjToCreate = terrainSituation.x + ((terrainData.chunkWidthIncrement + terrainData.terrainWidth - 1) * terrainData.cubeSize);
        generateObstaclesX(gameContext, terrainData, terrainSituation, posObjToCreate);

    } else if(terrainData.movedUp) {
        posObjToCreate = terrainSituation.z + (terrainData.chunkHeightIncrement * terrainData.cubeSize);
        generateObstaclesZ(gameContext, terrainData, terrainSituation, posObjToCreate);

    } else if(terrainData.movedDown) {
        posObjToCreate = terrainSituation.z - ((terrainData.terrainHeight + terrainData.chunkHeightIncrement) * terrainData.cubeSize);
        generateObstaclesZ(gameContext, terrainData, terrainSituation, posObjToCreate);
    }

    deleteObstacles(gameContext, terrainData, terrainSituation);
}

void ObstacleSystem::generateObstaclesX(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation, float posObjX) const {
    uint8_t randomToSpawn = 0;
    float   posX = posObjX;
    float   posY = rand() % 2 + (-0.5); //entre -0.5 y 1.5;
    float   posZ = terrainSituation.z + ((terrainData.chunkHeightIncrement - 1) * terrainData.cubeSize);
    int     nrows = terrainData.terrainHeight + terrainData.chunkHeightIncrement * 2;

    int obstacleSize = 10;
    int randomAuxMin = (-(terrainData.cubeSize / 2)) + obstacleSize;
    int randomAuxInc = terrainData.cubeSize - (obstacleSize);

    for(uint16_t row = 0; row < nrows; ++row) {
        randomToSpawn = rand() % 100 + (0); //entre 0 y 100
        if(randomToSpawn <= SPAWN_RATIO) {
            float posXRandom = posX + (rand() % randomAuxInc + randomAuxMin);
            float posZRandom = posZ + (rand() % randomAuxInc + randomAuxMin);
            createObstacleInBioma(gameContext, posXRandom, posY, posZRandom);
        }

        //posY  = rand() % 2 + (-0.5);
        posZ -= terrainData.cubeSize;
    }
}

void ObstacleSystem::generateObstaclesZ(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation, float posObjZ) const {
    uint8_t randomToSpawn = 0;
    float   posX = terrainSituation.x - (terrainData.chunkWidthIncrement * terrainData.cubeSize);
    float   posY = rand() % 2 + (-0.5); //entre -0.5 y 1.5;
    float   posZ = posObjZ;
    int     ncols = terrainData.terrainWidth + terrainData.chunkWidthIncrement * 2;

    int obstacleSize = 10;
    int randomAuxMin = (-(terrainData.cubeSize / 2)) + obstacleSize;
    int randomAuxInc = terrainData.cubeSize - (obstacleSize);

    for(uint16_t col = 0; col < ncols; ++col) {
        randomToSpawn = rand() % 100 + (0); //entre 0 y 100
        if(randomToSpawn <= SPAWN_RATIO) {
            float posXRandom = posX + (rand() % randomAuxInc + randomAuxMin);
            float posZRandom = posZ + (rand() % randomAuxInc + randomAuxMin);
            createObstacleInBioma(gameContext, posXRandom, posY, posZRandom);
        }

        //posY  = rand() % 2 + (-0.5);
        posX += terrainData.cubeSize;
    }
}


void ObstacleSystem::deleteObstacles(GameEngine &gameContext, TerrainDataComponent &terrainData, SituationComponent &terrainSituation) const {
    std::vector<uint32_t>   obstaclesToErase;
    auto                    &allObstacles       = gameContext.entityMan.getComponents<ObstacleComponent>();

    float terrainPosW = 0.f, terrainPosE = 0.f, terrainPosN = 0.f, terrainPosS = 0.f;

    Utils::calculateTerrainLimits(terrainPosW, terrainPosE, terrainPosN, terrainPosS, gameContext);

    for(auto &obstacle : allObstacles) {
        SituationComponent &obstacleSit = gameContext.entityMan.getComponent<SituationComponent>(obstacle.id);

        if( obstacleSit.x < terrainPosW || obstacleSit.x > terrainPosE
                || obstacleSit.z > terrainPosN || obstacleSit.z < terrainPosS ) {
            obstaclesToErase.emplace_back(obstacle.id);
        }
    }

    for(size_t i = 0; i < obstaclesToErase.size(); ++i) {
        gameContext.eraseEntityByID(obstaclesToErase[i]);
    }
}

void ObstacleSystem::createObstacleInBioma(GameEngine &gameContext, float posX, float posY, float posZ) const {
    // TEMP: Check if the obstacle is going to create in a Secure Zone and EGGS
    bool collidingWithSomething = false;
    auto &ipDatas = gameContext.entityMan.getComponents<SecureZoneComponent>();
    for (auto &ipDataComp : ipDatas) {
        auto &situation = gameContext.entityMan.getComponent<SituationComponent>(ipDataComp.id);
        auto &collidable = gameContext.entityMan.getComponent<CollisionComponent>(ipDataComp.id);
        if (Utils::isCollisionAB(posX, posY, posZ, 40, 40, 40, situation, collidable)) {
            collidingWithSomething = true;
            break;
        }
    }

    if (!collidingWithSomething) {
        auto &eggsDatas = gameContext.entityMan.getComponents<EggDataComponent>();
        for (auto &eggDataComp : eggsDatas) {
            auto &situation = gameContext.entityMan.getComponent<SituationComponent>(eggDataComp.id);
            auto &collidable = gameContext.entityMan.getComponent<CollisionComponent>(eggDataComp.id);
            if (Utils::isCollisionAB(posX, posY, posZ, 40, 40, 40, situation, collidable)) {
                collidingWithSomething = true;
                break;
            }
        }
    }

    if (!collidingWithSomething) {
        ObstacleDataComponent &obstacleData = gameContext.entityMan.getComponent<ObstacleDataComponent>(gameContext.obstacleDataId);
        uint8_t currentBiome = Utils::calculateBiome(gameContext, posX, posZ);

        //SE PODRIA SUSTITUIR EL SWITCH POR UN VECTOR DE VECTORES DE TIPO <obstacleBiome" EN EL OBSTACLEDATACOMPONENT
        //CURRENT BIOME INDICARIA LA POSICION DEL VECTOR DE VECTORES A RECORRER :)

        size_t numObstacles;
        uint8_t randObstacle;
        uint32_t obstacleId;
        switch(currentBiome) {
        case 1:
            numObstacles = obstacleData.obstaclesBiome1.size();
            randObstacle = rand() % numObstacles + (0);
            gameContext.entityMan.createObstacle(gameContext, posX, posY, posZ, 0.f, obstacleData.obstaclesBiome1[randObstacle]);
            break;
        case 2:
            numObstacles = obstacleData.obstaclesBiome2.size();

            randObstacle = rand() % numObstacles + (0);
            obstacleId = gameContext.entityMan.createObstacle(gameContext, posX, posY, posZ, 0.f, obstacleData.obstaclesBiome2[randObstacle]);

            //LODO (TEMPORALMENTE DESCARTADO)
            //addWeaponToObstacleIfMust(gameContext, obstacleData.obstaclesBiome2[randObstacle], obstacleId);
            break;
        case 3:
            numObstacles = obstacleData.obstaclesBiome3.size();

            // TEMP: las signals salen menos veces que los arboles
            randObstacle = rand() % 101 + (0);
            if (randObstacle <= 90) { // 90% de veces arboles
                randObstacle = 0;
                float auxRand = rand() % numObstacles;
                if (auxRand)
                    randObstacle++;
            } else {
                randObstacle = 2;
            }
            //randObstacle = rand() % numObstacles + (0);
            gameContext.entityMan.createObstacle(gameContext, posX, posY, posZ, 0.f, obstacleData.obstaclesBiome3[randObstacle]);
            break;
        case 4:
            numObstacles = obstacleData.obstaclesBiome4.size();
            randObstacle = rand() % numObstacles + (0);
            gameContext.entityMan.createObstacle(gameContext, posX, posY, posZ, 0.f, obstacleData.obstaclesBiome4[randObstacle]);
            break;
        case 5:
            numObstacles = obstacleData.obstaclesBiome5.size();
            randObstacle = rand() % numObstacles + (0);
            gameContext.entityMan.createObstacle(gameContext, posX, posY, posZ, 0.f, obstacleData.obstaclesBiome5[randObstacle]);
            break;
        }
    }
}

//LODO QUE RALENTIZA (TEMPORALMENTE DESCARTADO)
/*
void ObstacleSystem::addWeaponToObstacleIfMust(GameEngine &gameContext, GameObjectType obstacleType, uint32_t obstacleId) const {
    if(obstacleType == GAMEOBJECTTYPE del Obstaculo Lodo) {
        CollisionComponent   &collisionComp   = gameContext.entityMan.getComponent<CollisionComponent>(obstacleId);
        MeleeWeaponComponent &meleeWeaponComp = gameContext.entityMan.createComponent<MeleeWeaponComponent>(obstacleId);
        // DAMAGES AND WEAPONS
        meleeWeaponComp.damage                  = 1000;
        meleeWeaponComp.damageRange             = 25;
        meleeWeaponComp.criticalPercent         = 0; // CHANGE
        meleeWeaponComp.effectPercent           = 100;
        meleeWeaponComp.biome                   = 2;
        meleeWeaponComp.attackSize              = collisionComp.sizeX + 1; // CHANGE TO IT COLLIDABLE SIZE
        //meleeWeaponComp.sizeSeparationAttacks   = 0;
        meleeWeaponComp.posOrigin               = 0;
        //meleeWeaponComp.numAttacks              = 1;
        meleeWeaponComp.maxCoolDown             = 1; // CHANGE WHEN LUNGE DONE
        //meleeWeaponComp.coolDown                = 0; // CHANGE WHEN LUNGE DONE
        meleeWeaponComp.effect                  = SLOWDOWN;
        //meleeWeaponComp.weapon                  = UNKNOWN;
    }
}

void ObstacleSystem::updateObstaclesWeapons(GameEngine &gameContext) const {
    auto &allObstacles = gameContext.entityMan.getComponents<ObstacleComponent>();
    for(auto &obstacle : allObstacles) {
        if(gameContext.entityMan.existsComponent<MeleeWeaponComponent>(obstacle.id)) {
            MeleeWeaponComponent &meleeWeapon       = gameContext.entityMan.getComponent<MeleeWeaponComponent>(obstacle.id);
            SituationComponent   &obstacleSituation = gameContext.entityMan.getComponent<SituationComponent>(obstacle.id);
            meleeObstacleAttack(gameContext, meleeWeapon, obstacleSituation, obstacle.id, 0);
        }
    }
}

uint32_t ObstacleSystem::meleeObstacleAttack(GameEngine &gameContext, MeleeWeaponComponent &meleeWeapon, SituationComponent &obstacleSituation, uint32_t attackerId, float lifeTime) const {
    uint32_t idAttackRet {0};

    EffectType effect  = meleeWeapon.effect;
    float finalDamage  = meleeWeapon.damage;
    float rot          = obstacleSituation.rotation;
    float rotRad       = Utils::degToRad(rot);


    float attackSize   = meleeWeapon.attackSize;
    float sizeSepAttac = meleeWeapon.sizeSeparationAttacks;
    uint8_t posOrigin  = meleeWeapon.posOrigin;
    uint8_t numAttacks = meleeWeapon.numAttacks;
    uint8_t biome      = meleeWeapon.biome;

    for(uint8_t i = 0; i < numAttacks; i++) {
        float posX         = obstacleSituation.x + (posOrigin + (i * (attackSize + sizeSepAttac))) * cos(rotRad);
        float posY         = obstacleSituation.y;
        float posZ         = obstacleSituation.z + (posOrigin + (i * (attackSize + sizeSepAttac))) * sin(rotRad);

        uint32_t idAttack = gameContext.entityMan.createAttack(gameContext, posX, posY, posZ, rot, ATTACK_MELEE);

        idAttackRet = idAttack;

        CollisionComponent  &collidableAttackerComp = gameContext.entityMan.getComponent<CollisionComponent>(attackerId);
        DrawableComponent   &drawableComp   = gameContext.entityMan.getComponent<DrawableComponent>(idAttack);
        drawableComp.model      = "";
        drawableComp.texture    = "./media/weapons/sword2.png";
        drawableComp.scaleX     = collidableAttackerComp.sizeX / 10;
        drawableComp.scaleY     = collidableAttackerComp.sizeY / 10;
        drawableComp.scaleZ     = collidableAttackerComp.sizeZ / 10;


        CollisionComponent  &collidableComp = gameContext.entityMan.getComponent<CollisionComponent>(idAttack);
        collidableComp.sizeX = collidableAttackerComp.sizeX;
        collidableComp.sizeY = collidableAttackerComp.sizeY;
        collidableComp.sizeZ = collidableAttackerComp.sizeZ;
        collidableComp.type  = KINEMATICMELEE;

        VelocityComponent   &velocityComp   = gameContext.entityMan.getComponent<VelocityComponent>(idAttack);
        velocityComp.speed = 0;

        AttackComponent     &attackComp     = gameContext.entityMan.getComponent<AttackComponent>(idAttack);
        attackComp.attackerId       = attackerId;
        attackComp.damage           = finalDamage;
        attackComp.lifeTime         = lifeTime;
        attackComp.biome            = biome;
        uint8_t ep = rand() % 100 + 1;
        if(ep <= meleeWeapon.effectPercent)
            attackComp.effectType = effect;

        gameContext.getWindowFacadeRef().updateTextures(gameContext, {idAttack});
        gameContext.getWindowFacadeRef().updateEntities(gameContext, {idAttack});
    }

    return idAttackRet;
}

*/
