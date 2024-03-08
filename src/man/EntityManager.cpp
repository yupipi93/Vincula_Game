#include "EntityManager.hpp"
#include <enum/CollisionType.hpp>

#include <math.h>
#include <random>
#include <eng/GameEngine.hpp>


const bool SHOW_MELEEATTACKS     = false;

const bool DISABLED_VISIBLE_ZONE = false;

const bool ENABLED_AMBIENT_MUSIC = true;

EntityManager::EntityManager() {
    srand(time(NULL)); // initialize the random seed

}

EntityManager::~EntityManager() {}



/*
uint32_t EntityManager::createEntity (GameEngine &gameContext, EntityType eType, float x, float y, float z, GameObjectType goType) {
    uint32_t entityId = 0;
    switch(eType) {
    case PLAYER: {
        entityId = createPlayer(gameContext, x, y, z, goType);
        break;
    }
    case ENEMY: {
        entityId = createEnemy(gameContext, x, y, z, goType);
        break;
    }
    }//END SWITCH
    return entityId;
}
*/


uint32_t EntityManager::createPlayer(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    TerrainDataComponent    &terrainDataComp    = getComponent<TerrainDataComponent>(gameContext.terrainId);

    uint32_t entityId = Entity::getCurrentId();
    gameContext.playerId = entityId;


    createComponent<DashComponent>(entityId);
    createComponent<InputComponent>(entityId);
    createComponent<InteractionComponent>(entityId);
    createComponent<AIMoveToComponent>(entityId);
    createComponent<AtributesComponent>(entityId);


    LevelComponent          &levelComp          = createComponent<LevelComponent>(entityId);
    VelocityComponent       &velocityComp       = createComponent<VelocityComponent>(entityId);
    StatsComponent          &statsComp          = createComponent<StatsComponent>(entityId);
    SituationComponent      &situation          = createComponent<SituationComponent>(entityId);
    WalletComponent         &walletComp         = createComponent<WalletComponent>(entityId);
    InventoryComponent      &inventory          = createComponent<InventoryComponent>(entityId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);
    BoundingComponent       &sensorComp         = createComponent<BoundingComponent>(entityId);
    CoinDetectorComponent   &coinDetectComp     = createComponent<CoinDetectorComponent>(entityId);
    SoundComponent          &soundComp              {createComponent<SoundComponent>(entityId)};


    //######### DATA ########//
    statsComp.currentHealth = 1;

    situation.x = x;
    situation.y = y;
    situation.z = z;
    situation.rotation = r;

    velocityComp.speed = 80.f;
    velocityComp.speedNormal = 80.f;

    drawableComp.model = "./media/player/Animations/idleNoWeapon/idleNoWeapon";

    collidableComp.sizeX = 10;
    collidableComp.sizeY = 10;
    collidableComp.sizeZ = 10;
    collidableComp.type = DYNAMIC;

    sensorComp.sizeX = 40;
    sensorComp.sizeY = 100;
    sensorComp.sizeZ = 40;

    coinDetectComp.sizeX = 80;
    coinDetectComp.sizeY = 10;
    coinDetectComp.sizeZ = 80;

    if(gameContext.debuging) {
        levelComp.dashUnlocked      = true;
        levelComp.compassUnlocked   = true;
        levelComp.mapUnlocked       = true;
        gameContext.playerHasWeapon = true;
    }


    inventory.setMaxKeys(terrainDataComp.lastBiome);
    //    inventory.keysCollected.push_back(KEY1);
    //    inventory.keysCollected.push_back(KEY2);
    //    inventory.keysCollected.push_back(KEY3);
    //    inventory.keysCollected.push_back(KEY4);
    //    inventory.keysCollected.push_back(KEY5);

    soundComp.eventName = "Player/Player";

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### TIME FRAME DEFAULT ########//
    gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, entityId, 1.f / 12.f);
    //######### LISTENER ########//
    gameContext.getSoundFacadeRef().setListenerId(entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (PLAYER));
    return entityId;
}


uint32_t EntityManager::createEnemy(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    uint32_t easterEgg = rand() % 50; // 0 - 50


    createComponent<AIMoveToComponent>(entityId);

    EnemyComponent              &enemyComp              {createComponent<EnemyComponent>(entityId)};
    DistanceWeaponComponent     &distanceWeaponComp     {createComponent<DistanceWeaponComponent>(entityId)};
    MeleeWeaponComponent        &meleeWeaponComp        {createComponent<MeleeWeaponComponent>(entityId)};
    WalletComponent             &walletComp             {createComponent<WalletComponent>(entityId)};
    SituationComponent          &situationComp          {createComponent<SituationComponent>(entityId)};
    CollisionComponent          &collidableComp         {createComponent<CollisionComponent>(entityId)};
    BoundingComponent           &sensorComp             {createComponent<BoundingComponent>(entityId)};
    CoinDetectorComponent       &coinDetectComp         {createComponent<CoinDetectorComponent>(entityId)};
    VelocityComponent           &velComp                {createComponent<VelocityComponent>(entityId)};
    DrawableComponent           &drawableComp           {createComponent<DrawableComponent>(entityId)};
    StatsComponent              &statsComp              {createComponent<StatsComponent>(entityId)};
    SoundComponent              &soundComp              {createComponent<SoundComponent>(entityId)};
    AIComponent                 &aiComp                 {createComponent<AIComponent>(entityId)};
    AIDifuseStateComponent      &aiDifuseStateComp      {createComponent<AIDifuseStateComponent>(entityId)};
    AIDifuseActionComponent     &aiDifuseActionComp     {createComponent<AIDifuseActionComponent>(entityId)};

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    collidableComp.type = DYNAMIC;

    uint32_t PLAYER_COLLIDABLE_SIZE {10};
    uint32_t COLLIDABLE_SIZE {15};
    collidableComp.sizeX = COLLIDABLE_SIZE;
    collidableComp.sizeY = COLLIDABLE_SIZE;
    collidableComp.sizeZ = COLLIDABLE_SIZE;

    uint32_t SENSOR_SIZE {250};
    sensorComp.sizeX = SENSOR_SIZE;
    sensorComp.sizeY = 0;
    sensorComp.sizeZ = SENSOR_SIZE;

    coinDetectComp.sizeX = 100;
    coinDetectComp.sizeY = 10;
    coinDetectComp.sizeZ = 100;

    drawableComp.model = "";


    uint8_t coins  {0};
    uint8_t health {0};
    uint8_t damage {0};

    switch(goType) {
    case ENEMY_PIG:
        aiComp.enemyType = PIG;

        collidableComp.sizeX = 20;
        collidableComp.sizeY = 20;
        collidableComp.sizeZ = 20;


        aiComp.hostileTo = {};

        coins   = rand() % 2 + 1; // 1 - 3
        health  = rand() % 2 + 5; // 5 - 7

        velComp.speed           = 25;
        velComp.speedNormal     = 25;
        velComp.speedUpped      = 60;


        enemyComp.healthHeight = 20.f;


        drawableComp.model = "./media/enemies/pig/Pig.obj";
        //drawableComp.texture = "./media/enemies/pig/pig.png";

        // FUZZY LOGIC
        // OFFENSIVE when (healty OR (injured AND (stronger OR poorer)))
        // DEFENSIVE when (dying OR (injured AND (looser OR richer)))
        //  HEALTH
        aiDifuseStateComp.dyingX0           = 0;
        aiDifuseStateComp.dyingX1           = 100;
        aiDifuseStateComp.injuredX0         = 101;
        aiDifuseStateComp.injuredX1         = 101;
        aiDifuseStateComp.injuredX2         = 101;
        aiDifuseStateComp.injuredX3         = 101;
        aiDifuseStateComp.healthyX0         = 101;
        aiDifuseStateComp.healthyX1         = 101;




        if(easterEgg == 0) {
            soundComp.eventName =  "Enemies/Pig_MAD";
        } else {
            soundComp.eventName =  "Enemies/Pig";
        }


        break;

    case ENEMY_RODENT:
        aiComp.hostileTo = {PLAYER};
        aiComp.enemyType = CHICK;

        coins   = rand() % 1 + 4; // 4 - 5
        health  = rand() % 1 + 4; // 4 - 5

        velComp.speed           = 60;
        velComp.speedNormal     = 60;
        velComp.speedUpped      = 110;


        enemyComp.healthHeight = 20.f;

        drawableComp.model = "./media/enemies/rodent/StillChick.obj";
        //drawableComp.texture = "./media/enemies/rodent/rodent.png";

        sensorComp.sizeX = 150;
        sensorComp.sizeY = 150;
        sensorComp.sizeZ = 150;

        // FUZZY LOGIC
        // OFFENSIVE when (healty OR (injured AND (stronger OR poorer)))
        // DEFENSIVE when (dying OR (injured AND (looser OR richer)))
        //  HEALTH
        aiDifuseStateComp.dyingX0           = 0;
        aiDifuseStateComp.dyingX1           = 100;
        aiDifuseStateComp.injuredX0         = 101;
        aiDifuseStateComp.injuredX1         = 101;
        aiDifuseStateComp.injuredX2         = 101;
        aiDifuseStateComp.injuredX3         = 101;
        aiDifuseStateComp.healthyX0         = 101;
        aiDifuseStateComp.healthyX1         = 101;

        soundComp.eventName =  "Enemies/Chiken";

        break;

    case ENEMY_PINEAPPLE:
        aiComp.enemyType = PINEAPPLE;

        COLLIDABLE_SIZE = 24;
        collidableComp.sizeX = COLLIDABLE_SIZE;
        collidableComp.sizeY = COLLIDABLE_SIZE;
        collidableComp.sizeZ = COLLIDABLE_SIZE;

        coins   = rand() % 3 + 5; // 5 - 7
        health  = rand() % 4 + 12; // 12 - 15

        velComp.speed           = 50;
        velComp.speedNormal     = 50;
        velComp.speedUpped      = 250;

        enemyComp.healthHeight = 40.f;

        drawableComp.model = "./media/enemies/pineapple/pinecone.obj";
        //drawableComp.texture = "./media/enemies/pineapple/pineapple.png";

        // DAMAGES AND WEAPONS
        damage  = 15; // 15 - 20
        meleeWeaponComp.damage                  = 1;
        meleeWeaponComp.damageRange             = 33;
        meleeWeaponComp.criticalPercent         = 0; // CHANGE
        meleeWeaponComp.effectPercent           = 100;
        meleeWeaponComp.biome                   = 2;
        meleeWeaponComp.attackSize              = COLLIDABLE_SIZE + 1; // CHANGE TO IT COLLIDABLE SIZE
        //meleeWeaponComp.sizeSeparationAttacks   = 0;
        meleeWeaponComp.posOrigin               = 1;
        //meleeWeaponComp.numAttacks              = 1;
        meleeWeaponComp.maxCoolDown             = 0; // CHANGE WHEN LUNGE DONE
        //meleeWeaponComp.coolDown                = 0; // CHANGE WHEN LUNGE DONE
        meleeWeaponComp.effect                  = PARALYSIS;
        //meleeWeaponComp.weapon                  = UNKNOWN;


        // FUZZY LOGIC
        // MELEE
        aiDifuseActionComp.meleeX0 = -1;
        aiDifuseActionComp.meleeX1 = -1;

        // DISTANCE
        aiDifuseActionComp.distanceX0 = -1;
        aiDifuseActionComp.distanceX1 = -1;
        aiDifuseActionComp.distanceX2 = -1;
        aiDifuseActionComp.distanceX3 = -1;

        // LUNGE
        aiDifuseActionComp.lungeX0 = 30;
        aiDifuseActionComp.lungeX1 = 40;
        aiDifuseActionComp.lungeX2 = SENSOR_SIZE - 1;
        aiDifuseActionComp.lungeX3 = SENSOR_SIZE;

        // FOLLOW
        aiDifuseActionComp.followX0 = SENSOR_SIZE + 1;
        aiDifuseActionComp.followX1 = SENSOR_SIZE + 1;

        // KEEPDISTANCE
        aiDifuseActionComp.keepDistanceX0 = 0;
        aiDifuseActionComp.keepDistanceX1 = 10;
        aiDifuseActionComp.keepDistanceX2 = 20;
        aiDifuseActionComp.keepDistanceX3 = 30;

        soundComp.eventName =  "Enemies/Pineaple";

        break;

    case ENEMY_SPIDER:
        aiComp.enemyType = SPIDER;

        COLLIDABLE_SIZE = 19;
        collidableComp.sizeX = COLLIDABLE_SIZE;
        collidableComp.sizeY = COLLIDABLE_SIZE;
        collidableComp.sizeZ = COLLIDABLE_SIZE;

        coins   = rand() % 3 + 5; // 5 - 7
        health  = rand() % 6 + 15; // 15 - 20

        velComp.speed           = 90;
        velComp.speedNormal     = 90;
        velComp.speedUpped      = 150;

        aiComp.standByAction   = NO_ACTION;
        aiComp.defensiveAction = REGROUP;

        enemyComp.healthHeight = 20.f;

        drawableComp.model = "./media/enemies/spider/spiderIdle/spiderIdle";
        //drawableComp.texture = "./media/enemies/spider/spider.png";

        // DAMAGES AND WEAPONS
        damage  = 10; // 10 - 15
        meleeWeaponComp.damage                  = 1;
        meleeWeaponComp.damageRange             = 50;
        meleeWeaponComp.criticalPercent         = 0; // CHANGE
        meleeWeaponComp.effectPercent           = 0;
        meleeWeaponComp.biome                   = 3;
        meleeWeaponComp.attackSize              = 5;
        //meleeWeaponComp.sizeSeparationAttacks   = 0;
        meleeWeaponComp.posOrigin               = COLLIDABLE_SIZE; // CHANGE TO IT COLLIDABLE
        meleeWeaponComp.numAttacks              = 1;
        meleeWeaponComp.maxCoolDown             = 0.5;
        //meleeWeaponComp.coolDown                = 0;
        meleeWeaponComp.effect                  = POISON;
        //meleeWeaponComp.weapon                  = UNKNOWN;

        // FUZZY LOGIC
        //  HEALTH
        aiDifuseStateComp.dyingX0           = 0;
        aiDifuseStateComp.dyingX1           = 40;
        aiDifuseStateComp.injuredX0         = 35;
        aiDifuseStateComp.injuredX1         = 50;


        // MELEE
        aiDifuseActionComp.meleeX0 = 0;
        // I know this is fucking long, but it is necessary
        aiDifuseActionComp.meleeX1 = COLLIDABLE_SIZE / 2 + meleeWeaponComp.attackSize / 1.1 + PLAYER_COLLIDABLE_SIZE / 2;

        // DISTANCE
        aiDifuseActionComp.distanceX0 = -1;
        aiDifuseActionComp.distanceX1 = -1;
        aiDifuseActionComp.distanceX2 = -1;
        aiDifuseActionComp.distanceX3 = -1;

        // LUNGE
        aiDifuseActionComp.lungeX0 = -1;
        aiDifuseActionComp.lungeX1 = -1;
        aiDifuseActionComp.lungeX2 = -1;
        aiDifuseActionComp.lungeX3 = -1;

        // FOLLOW
        // I know this is fucking long, but it is necessary
        aiDifuseActionComp.followX0 = aiDifuseActionComp.meleeX1;
        aiDifuseActionComp.followX1 = SENSOR_SIZE;

        // KEEPDISTANCE
        aiDifuseActionComp.keepDistanceX0 = -1;
        aiDifuseActionComp.keepDistanceX1 = -1;
        aiDifuseActionComp.keepDistanceX2 = -1;
        aiDifuseActionComp.keepDistanceX3 = -1;

        soundComp.eventName =  "Enemies/Spider";

        break;

    case ENEMY_MONKEY:
        SENSOR_SIZE = 350;
        sensorComp.sizeX = SENSOR_SIZE;
        sensorComp.sizeY = 0;
        sensorComp.sizeZ = SENSOR_SIZE;
        aiComp.enemyType = MONKEY;

        distanceWeaponComp.eventName = "Weapons/Banana-Shoot";

        coins   = rand() % 6 + 10; // 10 - 15
        health  = rand() % 6 + 25; // 25 - 30

        velComp.speed           = 70;
        velComp.speedNormal     = 70;
        velComp.speedUpped      = 150;

        enemyComp.healthHeight = 60.f;

        //drawableComp.texture = "./media/enemies/monkey/monkey.png";
        drawableComp.model = "./media/enemies/monkey/monkeyIdle/monkeyIdle";

        // DAMAGES AND WEAPONS
        damage  = 20; // 20 - 25
        distanceWeaponComp.damage                   = 1;
        distanceWeaponComp.damageRange              = 25;
        distanceWeaponComp.criticalPercent          = 0;
        distanceWeaponComp.effectPercent            = 20;
        distanceWeaponComp.biome                    = 4;
        distanceWeaponComp.attackSize               = 3;
        //distanceWeaponComp.sizeSeparationAttacks    = 0;
        distanceWeaponComp.posOrigin                = 30;
        //distanceWeaponComp.numAttacks               = 0;
        distanceWeaponComp.maxCoolDown              = 1;
        //distanceWeaponComp.coolDown                 = 0;
        distanceWeaponComp.effect                   = SMARTBULLET;
        //distanceWeaponComp.weapon                  = UNKNOWN;
        distanceWeaponComp.bulletModel              = "./media/enemies/monkey/banana.obj";
        distanceWeaponComp.bulletEffectModel        = "./media/enemies/monkey/bananaFollow.obj";
        distanceWeaponComp.bulletSpeed              = 300;
        distanceWeaponComp.bulletLifeTime           = 2;
        distanceWeaponComp.collisionType            = KINEMATIC;
        distanceWeaponComp.rotative                 = true;

        // FUZZY LOGIC
        // MELEE
        aiDifuseActionComp.meleeX0 = -1;
        aiDifuseActionComp.meleeX1 = -1;

        // DISTANCE
        aiDifuseActionComp.distanceX0 = 80;
        aiDifuseActionComp.distanceX1 = 90;
        aiDifuseActionComp.distanceX2 = SENSOR_SIZE / 2 - 1;
        aiDifuseActionComp.distanceX3 = SENSOR_SIZE / 2;

        // LUNGE
        aiDifuseActionComp.lungeX0 = -1;
        aiDifuseActionComp.lungeX1 = -1;
        aiDifuseActionComp.lungeX2 = -1;
        aiDifuseActionComp.lungeX3 = -1;

        // FOLLOW
        aiDifuseActionComp.followX0 = aiDifuseActionComp.distanceX0;
        aiDifuseActionComp.followX1 = SENSOR_SIZE;

        // KEEPDISTANCE
        aiDifuseActionComp.keepDistanceX0 = 0;
        aiDifuseActionComp.keepDistanceX1 = aiDifuseActionComp.distanceX0;
        aiDifuseActionComp.keepDistanceX2 = aiDifuseActionComp.distanceX0;
        aiDifuseActionComp.keepDistanceX3 = aiDifuseActionComp.distanceX0;

        soundComp.eventName =  "Enemies/Monkey";

        break;

    case ENEMY_GHOST:
        aiComp.enemyType = GHOST;

        distanceWeaponComp.eventName = "Weapons/Ghost-Shoot";

        COLLIDABLE_SIZE = 15;

        collidableComp.sizeX = COLLIDABLE_SIZE;
        collidableComp.sizeY = COLLIDABLE_SIZE;
        collidableComp.sizeZ = COLLIDABLE_SIZE;
        SENSOR_SIZE = 400;
        sensorComp.sizeX = SENSOR_SIZE;
        sensorComp.sizeY = 0;
        sensorComp.sizeZ = SENSOR_SIZE;


        coins   = 100;
        health  = rand() % 31 + 320; // 320 - 350

        velComp.speed           = 40;
        velComp.speedNormal     = 40;
        velComp.speedUpped      = 300;

        enemyComp.healthHeight = 60.f;

        //drawableComp.texture = "./media/enemies/ghost/ghost.png";
        drawableComp.model = "./media/enemies/ghost/ghostIdle/ghostIdle";
        drawableComp.transparency = true;

        // DAMAGES AND WEAPONS
        damage                                  = 45; // 45 - 50
        meleeWeaponComp.damage                  = 2;
        meleeWeaponComp.damageRange             = 11;
        meleeWeaponComp.criticalPercent         = 0; // CHANGE
        meleeWeaponComp.effectPercent           = 10;
        meleeWeaponComp.biome                   = 5;
        meleeWeaponComp.attackSize              = collidableComp.sizeX + 1; // CHANGE TO IT COLLIDABLE SIZE
        //meleeWeaponComp.sizeSeparationAttacks   = 0;
        meleeWeaponComp.posOrigin               = 5; // CHANGE TO IT COLLIDABLE
        meleeWeaponComp.numAttacks              = 1;
        meleeWeaponComp.maxCoolDown             = 0.8;
        //meleeWeaponComp.coolDown                = 0;
        meleeWeaponComp.effect                  = SLOWDOWN;
        //meleeWeaponComp.weapon                  = UNKNOWN;
        distanceWeaponComp.bulletModel              = "./media/enemies/ghost/tornado.obj";
        distanceWeaponComp.bulletEffectModel        = "./media/enemies/ghost/tornado.obj";
        distanceWeaponComp.damage                   = 0.45;
        distanceWeaponComp.damageRange              = 20;
        distanceWeaponComp.criticalPercent          = 0;
        distanceWeaponComp.effectPercent            = 100;
        distanceWeaponComp.biome                    = 5;
        distanceWeaponComp.attackSize               = 5;
        //distanceWeaponComp.sizeSeparationAttacks    = 0;
        //distanceWeaponComp.posOrigin                = 0;
        //distanceWeaponComp.numAttacks               = 0;
        distanceWeaponComp.maxCoolDown              = 0.75;
        //distanceWeaponComp.coolDown                 = 0;
        distanceWeaponComp.effect                   = MAGNIFYBULLET;
        //distanceWeaponComp.weapon                  = UNKNOWN;
        distanceWeaponComp.bulletModel              = "";
        distanceWeaponComp.bulletSpeed              = 400;
        distanceWeaponComp.bulletLifeTime           = 5;
        distanceWeaponComp.collisionType            = DRILLCOLL;

        // FUZZY LOGIC
        // MELEE
        aiDifuseActionComp.meleeX0 = 0;
        // I know this is fucking long, but it is necessary
        aiDifuseActionComp.meleeX1 = COLLIDABLE_SIZE / 2 + meleeWeaponComp.attackSize / 1.1 + PLAYER_COLLIDABLE_SIZE / 2;

        // DISTANCE
        aiDifuseActionComp.distanceX0 = 140;
        aiDifuseActionComp.distanceX1 = 150;
        aiDifuseActionComp.distanceX2 = 160;
        aiDifuseActionComp.distanceX3 = 170;

        // LUNGE
        aiDifuseActionComp.lungeX0 = aiDifuseActionComp.meleeX1;
        aiDifuseActionComp.lungeX1 = aiDifuseActionComp.meleeX1;
        aiDifuseActionComp.lungeX2 = 110;
        aiDifuseActionComp.lungeX3 = 120;

        // FOLLOW
        aiDifuseActionComp.followX0 = aiDifuseActionComp.meleeX1;
        aiDifuseActionComp.followX1 = SENSOR_SIZE;

        // KEEPDISTANCE
        aiDifuseActionComp.keepDistanceX0 = -1;
        aiDifuseActionComp.keepDistanceX1 = -1;
        aiDifuseActionComp.keepDistanceX2 = -1;
        aiDifuseActionComp.keepDistanceX3 = -1;

        soundComp.eventName =  "Enemies/Spectrum";

        break;

    }

    //EXTRA INDIVIDUAL COMPONENTS
    /*
    if(goType == ENEMY_GHOST) {
        LevitationComponent &levitationComp = createComponent<LevitationComponent>(entityId);
        levitationComp.initialHeight    = y + 6;
        levitationComp.levitationMargin = 6;
        levitationComp.levitationSpeed  = 8;
    }*/

    walletComp.coins        = coins;
    statsComp.damage        = damage;
    statsComp.maxHealth     = health;
    statsComp.currentHealth = health;

    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    //entityMap.emplace(entityId, (ENEMY, goType));
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(ENEMY, goType));
    //situations.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(Entity::getCurrentId()));

    return entityId;

}



uint32_t EntityManager::createVillageObstacle(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    if(goType != VILLAGE_OBSTACLE_DANGER) { //skip erase by visible system
        gameContext.villageIds.push_back(entityId);
    }

    SituationComponent &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent &collidableComp = createComponent<CollisionComponent>(entityId);
    DrawableComponent  &drawableComp   = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    //float randRot = rand() % 180;

    situationComp.rotation = r;
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    float colliderSize = 29.f;
    collidableComp.sizeX = colliderSize;
    collidableComp.sizeY = colliderSize;
    collidableComp.sizeZ = colliderSize;
    collidableComp.type = STATIC;

    switch(goType) {
    case VILLAGE_OBSTACLE_SHAMAN_ENVIRONMENT:
        drawableComp.model = "./media/base/improver/shaman_environmentRune.obj";
        //colliderSize = 20.f;
        //collidableComp.sizeX = colliderSize;
        //collidableComp.sizeY = colliderSize;
        //collidableComp.sizeZ = colliderSize;
        collidableComp.type = DRILLCOLL;
        drawableComp.transparency = true;

        break;
    case VILLAGE_OBSTACLE_MEDICTEND:
        drawableComp.model = "./media/obstacles/B0/medicTend.obj";
        collidableComp.sizeX = 70;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = 60;

        break;
    case VILLAGE_OBSTACLE_BARREL:
        drawableComp.model = "./media/obstacles/B0/barrelOpen.obj";
        colliderSize = 20.f;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;
    case VILLAGE_OBSTACLE_BARREL2:
        drawableComp.model = "./media/obstacles/B0/barrelClose.obj";
        colliderSize = 20.f;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case VILLAGE_OBSTACLE_SCARECROW:
        drawableComp.model = "./media/obstacles/B0/scarecrow.obj";
        collidableComp.sizeX = 10.f;
        collidableComp.sizeY = 10.f;
        collidableComp.sizeZ = 10.f;

        break;
    case VILLAGE_OBSTACLE_WEAPON_HOLDER:
        drawableComp.model = "./media/obstacles/B0/weaponHolder.obj";

        break;
    case VILLAGE_OBSTACLE_HOUSE1:
        drawableComp.model = "./media/obstacles/B0/house1.obj";
        collidableComp.sizeX = 80.f;
        collidableComp.sizeY = 10.f;
        collidableComp.sizeZ = 130.f;

        break;
    case VILLAGE_OBSTACLE_WINDMILL:
        drawableComp.model = "./media/obstacles/B0/windmill.obj";
        collidableComp.sizeX = 95.f;
        collidableComp.sizeY = 10.f;
        collidableComp.sizeZ = 75.f;

        break;
    case VILLAGE_OBSTACLE_WATERMILL:
        drawableComp.model = "./media/obstacles/B0/watermill.obj";
        collidableComp.sizeX = 95.f;
        collidableComp.sizeY = 10.f;
        collidableComp.sizeZ = 90.f;

        break;
    case VILLAGE_OBSTACLE_FENCE:
        drawableComp.model = "./media/obstacles/B0/fence.obj";

        break;

    case VILLAGE_OBSTACLE_WELL:
        drawableComp.model = "./media/obstacles/B0/well.obj";
        collidableComp.sizeX = 40.f;
        collidableComp.sizeY = 10.f;
        collidableComp.sizeZ = 60.f;

        break;

    case VILLAGE_OBSTACLE_DANGER: {
        /*
        LevitationComponent &levitationComp = createComponent<LevitationComponent>(entityId);
        levitationComp.initialHeight    = y;
        levitationComp.levitationMargin = 5;
        levitationComp.levitationSpeed  = 7;
        situationComp.y = y - levitationComp.levitationMargin;
        */
        drawableComp.model        = "./media/obstacles/B0/exclamation_100.obj";
        drawableComp.transparency = true;
        collidableComp.sizeX = 0;
        collidableComp.sizeY = 0;
        collidableComp.sizeZ = 0;
    }


    break;
    }


    //######### RENDER ########//
    //if(DISABLED_VISIBLE_ZONE)
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (OBSTACLE));


    return entityId;
}

uint32_t EntityManager::createMap(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.mapId = entityId;

    createComponent<MapComponent>(entityId);

    entityMap.emplace(entityId, (MAP));
    return entityId;
}


uint32_t EntityManager::createCamera           (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.cameraId = entityId;

    createComponent<SituationComponent>(entityId);
    createComponent<VelocityComponent>(entityId);
    //SoundComponent          &soundComp          = createComponent<SoundComponent>(entityId);

    //soundComp.eventName =  "Ambient/LoopAmbient";


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createCamera(gameContext, entityId);
    ////######### SOUND ########//
    //if (ENABLED_AMBIENT_MUSIC)
    //    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (CAMERA));

    return entityId;
}

uint32_t EntityManager::createSource             (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.sourceId = entityId;


    createComponent<WalletComponent>(entityId);
    createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);
    BoundingComponent       &sensorComp         = createComponent<BoundingComponent>(entityId);
    SoundComponent          &soundComp          = createComponent<SoundComponent>(entityId);

    //######### DATA ########//
    drawableComp.model        = "./media/base/source/source.obj";
    //drawableComp.texture = "./media/base/source/source.png";

    collidableComp.sizeX = 80;
    collidableComp.sizeY = 80;
    collidableComp.sizeZ = 80;
    collidableComp.type = STATIC;

    drawableComp.scaleX = 1;
    drawableComp.scaleY = 1;
    drawableComp.scaleZ = 1;
    drawableComp.transparency = true;

    sensorComp.sizeX = 400;
    sensorComp.sizeY = 400;
    sensorComp.sizeZ = 400;


    soundComp.eventName =  "Base/Source";


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (SOURCE));


    return entityId;
}

uint32_t EntityManager::createBank             (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.bankId = entityId;

    createComponent<WalletComponent>(entityId);
    SituationComponent      &situationComp      = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);
    CoinDetectorComponent   &coinDetectComp     = createComponent<CoinDetectorComponent>(entityId);
    SoundComponent          &soundComp          = createComponent<SoundComponent>(entityId);

    //######### DATA ########//
    drawableComp.model = "./media/base/bank/bank.obj";
    drawableComp.scaleX = 1;
    drawableComp.scaleY = 1;
    drawableComp.scaleZ = 1;

    collidableComp.sizeX = 40.f;
    collidableComp.sizeY = 10.f;
    collidableComp.sizeZ = 60.f;
    collidableComp.type = STATIC;


    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;




    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (BANK));

    return entityId;
}


uint32_t EntityManager::createBankDepositor         (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.bankDepositorId = entityId;

    SituationComponent      &situationComp      = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    BoundingComponent       &sensorComp         = createComponent<BoundingComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    drawableComp.model = "./media/base/bank/bankDepositor.obj";
    drawableComp.modelSecondary = "./media/base/bank/bankDepositorActive.obj";

    drawableComp.scaleX = 3;
    drawableComp.scaleY = 3;
    drawableComp.scaleZ = 3;
    drawableComp.transparency = true;
    drawableComp.hasShadow = false;

    float size = 5;
    collidableComp.sizeX = size * 10;
    collidableComp.sizeY = size * 10;
    collidableComp.sizeZ = size * 10;
    collidableComp.type = DRILLCOLL;

    sensorComp.sizeX = size * 10;
    sensorComp.sizeZ = size * 10;





    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (BANK_DEPOSITOR));

    return entityId;
}


uint32_t EntityManager::createBankExtractor         (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.bankExtractorId = entityId;

    SituationComponent      &situationComp      = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    BoundingComponent       &sensorComp         = createComponent<BoundingComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    drawableComp.model = "./media/base/bank/bankExtractor.obj";
    drawableComp.modelSecondary = "./media/base/bank/bankExtractorActive.obj";


    drawableComp.scaleX = 3;
    drawableComp.scaleY = 3;
    drawableComp.scaleZ = 3;
    drawableComp.transparency = true;
    drawableComp.hasShadow = false;



    float size = 5;
    sensorComp.sizeX = size * 10;
    sensorComp.sizeZ = size * 10;

    collidableComp.sizeX = size * 10;
    collidableComp.sizeY = size * 10;
    collidableComp.sizeZ = size * 10;
    collidableComp.type = DRILLCOLL;



    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (BANK_EXTRACTOR));

    return entityId;
}


uint32_t EntityManager::createWall             (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    createComponent<WallComponent>(entityId);
    SituationComponent          &situationComp      = createComponent<SituationComponent>(entityId);
    /**/TerrainDataComponent    &terrainDataComp    = getComponent<TerrainDataComponent>(gameContext.terrainId);
    DrawableComponent           &drawableComp       = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    drawableComp.model = "";
    drawableComp.texture = "./media/wall/wall.png";

    drawableComp.scaleY = terrainDataComp.wallHeight / 10;
    drawableComp.scaleZ = terrainDataComp.wallSize / 10;

    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    situationComp.rotation = r;



    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (WALL));

    return entityId;
}
uint32_t EntityManager::createWallCollidable   (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent      &situationComp  = createComponent<SituationComponent>(entityId);
    TerrainDataComponent    &terrainData    = getComponent<TerrainDataComponent>(gameContext.terrainId);
    CollisionComponent      &collidableComp = createComponent<CollisionComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    collidableComp.sizeX = terrainData.wallCollidableSize;
    collidableComp.sizeY = terrainData.wallHeight;
    collidableComp.sizeZ = terrainData.wallCollidableSize;
    collidableComp.type = STATIC;


    //######### CREATE ########//
    entityMap.emplace(entityId, (WALLCOLLISION));

    return entityId;
}
uint32_t EntityManager::createDoor             (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.doorId = entityId;

    SituationComponent      &situationComp      = createComponent<SituationComponent>(entityId);
    CollisionComponent      &collidableComp     = createComponent<CollisionComponent>(entityId);
    TerrainDataComponent    &terrainDataComp    = getComponent<TerrainDataComponent>(gameContext.terrainId);
    DrawableComponent       &drawableComp       = createComponent<DrawableComponent>(entityId);
    SoundComponent          &soundComp          = createComponent<SoundComponent>(entityId);
    //createComponent<IPDataComponent>(entityId);


    //######### DATA ########//
    drawableComp.model = "./media/wall/door.obj";
    //drawableComp.texture = "./media/door/door.jpg";
    //drawableComp.scaleY = terrainDataComp.wallHeight / 10;
    //drawableComp.scaleZ = terrainDataComp.wallSize / 10;

    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    situationComp.rotation = r;

    collidableComp.sizeX = 10;
    collidableComp.sizeY = 1;
    collidableComp.sizeZ = 10;
    collidableComp.type = STATIC;

    soundComp.eventName = "Door/Door Chantic";

    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (DOOR));

    return entityId;
}
uint32_t EntityManager::createAIDirector       (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.aIDirectorId = entityId;

    DirectorDataComponent &directorComponent = gameContext.entityMan.createComponent<DirectorDataComponent>(gameContext.aIDirectorId);

    // DATA
    directorComponent.spawnTimer.reserve(6);
    directorComponent.spawnCurrentTimer.reserve(6);
    directorComponent.enemiesSpawned.reserve(6);
    directorComponent.maxEnemiesSpawned.reserve(6);

    directorComponent.addSpawn(0, 0); // Biome 0 (BASE)
    directorComponent.addSpawn(0.3, 20); // Biome 1 (PLEASANT MEADOW)
    directorComponent.addSpawn(0.3, 20); // Biome 2 (PINE FOREST)
    directorComponent.addSpawn(0.2, 20); // Biome 3 (POISONOUS SWAMP)
    directorComponent.addSpawn(0.6, 20); // Biome 4 (RAIN FOREST)
    directorComponent.addSpawn(1, 20); // Biome 5 (THE VOID)

    //######### CREATE ########//
    entityMap.emplace(entityId, (AIDIRECTOR));

    return entityId;

}

uint32_t EntityManager::createObstacleData      (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.obstacleDataId = entityId;

    createComponent<ObstacleDataComponent>(entityId);

    //######### CREATE ########//
    entityMap.emplace(entityId, (OBSTACLEDATA));

    return entityId;

}
uint32_t EntityManager::createCoin             (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    VelocityComponent   &velocityComp   = createComponent<VelocityComponent>(entityId);
    CoinComponent       &coinComp       = createComponent<CoinComponent>(entityId);
    SoundComponent      &soundComp      = createComponent<SoundComponent>(entityId);


    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    //situationComp.rotation = 45;
    situationComp.rotation = rand() % 90;

    collidableComp.sizeX = 10;
    collidableComp.sizeY = 10;
    collidableComp.sizeZ = 10;

    collidableComp.type = KINEMATIC;

    velocityComp.speed = 240;



    switch(goType) {
    case COIN_1:
        soundComp.eventName =  "Coins/Coin_1";
        drawableComp.model = "./media/crystals/crystal1/crystal1.obj";
        //drawableComp.texture = "./media/crystals/crystal1/crystal-1.png";//GREEN
        coinComp.value = 1;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_5:
        soundComp.eventName =  "Coins/Coin_5";
        drawableComp.model = "./media/crystals/crystal5/crystal5.obj";
        //drawableComp.texture = "./media/crystals/crystal5/crystal-5.png";//BLUE
        coinComp.value = 5;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_20:
        soundComp.eventName =  "Coins/Coin_20";
        drawableComp.model = "./media/crystals/crystal20/crystal20.obj";
        //drawableComp.texture = "./media/crystals/crystal20/crystal-20.png";//RED
        coinComp.value = 20;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_50:
        soundComp.eventName =  "Coins/Coin_50";
        drawableComp.model = "./media/crystals/crystal50/crystal50.obj";
        //drawableComp.texture = "./media/crystals/crystal50/crystal-50.png";//PURPLE
        coinComp.value = 50;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_100:
        soundComp.eventName =  "Coins/Coin_100";
        drawableComp.model = "./media/crystals/crystal100/crystal100.obj";
        //drawableComp.texture = "./media/crystals/crystal100/crystal-100.png";//SILVER
        coinComp.value = 100;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_200:
        soundComp.eventName =  "Coins/Coin_200";
        drawableComp.model = "./media/crystals/crystal200/crystal200.obj";
        //drawableComp.texture = "./media/crystals/crystal200/crystal-200.png";//GOLD
        coinComp.value = 200;
        createComponent<AIMoveToComponent>(entityId);
        break;

    case COIN_500:
        soundComp.eventName =  "Coins/Coin_500";
        drawableComp.model = "./media/crystals/crystal500/crystal500.obj";
        //drawableComp.texture = "./media/crystals/crystal500/crystal-500.png";//RAIMBOW
        coinComp.value = 500;
        createComponent<AIMoveToComponent>(entityId);
        break;

    }





    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (COIN));



    return entityId;

}
uint32_t EntityManager::createChest        (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    ChestComponent      &chestComp      = createComponent<ChestComponent>(entityId);
    SoundComponent      &soundComp      = createComponent<SoundComponent>(entityId);


    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    //Collidable adjusted by defect to most normal model chest
    collidableComp.sizeX = 18;
    collidableComp.sizeY = 18;
    collidableComp.sizeZ = 18;
    collidableComp.type = STATIC;

    soundComp.eventName = "Chest/chest";

    switch(goType) {
    case CHEST_COIN_CAR:
        chestComp.chestType          = COINCHEST;
        chestComp.coins              = 10;
        chestComp.modelOpenedChest   = "./media/chests/ChestOpened.obj";
        drawableComp.model           = "./media/chests/ChestClosed.obj";
        break;
    case CHEST_COIN_LATRINE:
        chestComp.chestType          = COINCHEST;
        chestComp.coins              = 100;//*NumBiome
        chestComp.modelOpenedChest   = "./media/chests/latrineChestOpened.obj";
        drawableComp.model           = "./media/chests/latrineChestClosed.obj";
        collidableComp.sizeX = 42;
        collidableComp.sizeY = 42;
        collidableComp.sizeZ = 42;
        break;
    case CHEST_COIN_CASTLE:
        chestComp.chestType          = COINCHEST;
        chestComp.coins              = 200;
        chestComp.modelOpenedChest   = "./media/chests/ChestOpened.obj";
        drawableComp.model           = "./media/chests/ChestClosed.obj";
        break;
    case CHEST_COIN_BAG:
        chestComp.chestType          = COINCHEST;
        chestComp.coins              = 512;//*NumBiome
        chestComp.modelOpenedChest   = "./media/chests/bagChestOpened.obj";
        drawableComp.model           = "./media/chests/bagChestClosed.obj";
        collidableComp.sizeX = 10;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 10;
        break;
    case CHEST_WEAPON_MACE:
        chestComp.chestType          = WEAPONCHEST;
        chestComp.weaponType         = MACE;
        chestComp.modelOpenedChest = "./media/chests/ChestOpened.obj";
        drawableComp.model           = "./media/chests/ChestClosed.obj";
        break;
    case CHEST_WEAPON_SWORD:
        chestComp.chestType          = WEAPONCHEST;
        chestComp.weaponType         = SWORD;
        chestComp.modelOpenedChest = "./media/chests/ChestOpened.obj";
        drawableComp.model           = "./media/chests/ChestClosed.obj";
        break;
    case CHEST_WEAPON_BOW_SKELETON:
        chestComp.chestType          = WEAPONCHEST;
        chestComp.weaponType         = BOW;
        chestComp.modelOpenedChest = "./media/chests/skeletonChestOpened.obj";
        drawableComp.model           = "./media/chests/skeletonChestClosed.obj";
        collidableComp.sizeX = 30;
        collidableComp.sizeY = 20;
        collidableComp.sizeZ = 36;
        break;
    }


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (CHEST));

    return entityId;

}
uint32_t EntityManager::createObstacle         (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    ObstacleComponent  obstacleComp    = createComponent<ObstacleComponent>(entityId);
    createComponent<VelocityComponent>(entityId);//TEMP WTF?????
    SituationComponent &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent &collidableComp = createComponent<CollisionComponent>(entityId);
    DrawableComponent  &drawableComp   = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    float randRot = rand() % 180;
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    situationComp.rotation = randRot;

    collidableComp.sizeX = 10.f;
    collidableComp.sizeY = 10.f;
    collidableComp.sizeZ = 10.f;
    collidableComp.type = STATIC;

    obstacleComp.obstacleType = goType;


    float randomScale = rand() % 2 + 1.5;
    uint8_t treeFactor = 2;
    uint8_t rockFactor = 12;

    switch(goType) {
    case VILLAGE_OBSTACLE_SCARECROW:
        drawableComp.model = "./media/obstacles/B0/scarecrow.obj";
        /*collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;*/
        break;
    case OBSTACLE_B1_1:
        drawableComp.model = "./media/obstacles/B1/Tree1_1.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B1_2:
        drawableComp.model = "./media/obstacles/B1/Tree1_2.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;

    case OBSTACLE_B1_3:
        drawableComp.model = "./media/obstacles/B1/Tree1_3.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;

    case OBSTACLE_B1_4:
        drawableComp.model = "./media/obstacles/B1/Rock1_1.obj";
        collidableComp.sizeX = randomScale * rockFactor;
        collidableComp.sizeZ = randomScale * rockFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;

    case OBSTACLE_B1_5:
        drawableComp.model = "./media/obstacles/B1/Rock1_2.obj";
        collidableComp.sizeX = randomScale * rockFactor;
        collidableComp.sizeZ = randomScale * rockFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B1_6:
        drawableComp.model = "./media/obstacles/B1/Stump1_1.obj";
        collidableComp.sizeX = 10;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 10;
        drawableComp.scaleX = 3;
        drawableComp.scaleY = 3;
        drawableComp.scaleZ = 3;
        break;

    case OBSTACLE_B2_1:
        drawableComp.model = "./media/obstacles/B2/Tree2_1.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B2_2:
        drawableComp.model = "./media/obstacles/B2/Tree2_2.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B2_3:
        drawableComp.model = "./media/obstacles/B2/Clouds2_3.obj";
        collidableComp.sizeX = 0;
        collidableComp.sizeY = 0;
        collidableComp.sizeZ = 0;
        drawableComp.scaleX = 3;
        drawableComp.scaleY = 3;
        drawableComp.scaleZ = 3;
        drawableComp.transparency = true;
        drawableComp.hasShadow = false;
        break;
    case OBSTACLE_B2_4:
        drawableComp.model = "./media/obstacles/B2/Plant2_4.obj";
        collidableComp.sizeX = 1;
        collidableComp.sizeY = 1;
        collidableComp.sizeZ = 1;
        drawableComp.scaleX = 3;
        drawableComp.scaleY = 3;
        drawableComp.scaleZ = 3;
        drawableComp.transparency = true;
        drawableComp.hasShadow = false;


        break;
    case OBSTACLE_B3_1:
        drawableComp.model = "./media/obstacles/B3/Tree3_1.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B3_2:
        drawableComp.model = "./media/obstacles/B3/Rock3_1.obj";
        collidableComp.sizeX = randomScale * rockFactor;
        collidableComp.sizeZ = randomScale * rockFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B3_3:
        drawableComp.model = "./media/obstacles/B3/signal.obj";
        collidableComp.sizeX = 3;
        collidableComp.sizeZ = 3;
        drawableComp.scaleX = 2;
        drawableComp.scaleY = 2;
        drawableComp.scaleZ = 2;
        break;
    case OBSTACLE_B4_1:
        drawableComp.model = "./media/obstacles/B4/Rock4_1.obj";
        collidableComp.sizeX = randomScale * 10;
        collidableComp.sizeZ = randomScale * 10;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;
    case OBSTACLE_B4_2:
        drawableComp.model = "./media/obstacles/B4/Tree4_1.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        drawableComp.transparency = true;

        break;
    case OBSTACLE_B4_3:
        drawableComp.model = "./media/obstacles/B4/Plant4_3.obj";
        collidableComp.sizeX = 0;
        collidableComp.sizeY = 0;
        collidableComp.sizeZ = 0;
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;
        drawableComp.transparency = true;

        break;

    case OBSTACLE_B4_4:
        drawableComp.model = "./media/obstacles/B4/Statue4_4.obj";
        collidableComp.sizeX = 40;
        collidableComp.sizeY = 40;
        collidableComp.sizeZ = 40;
        drawableComp.scaleX = 4;
        drawableComp.scaleY = 4;
        drawableComp.scaleZ = 4;
        drawableComp.transparency = true;

        break;


    case OBSTACLE_B5_1:
        drawableComp.model = "./media/obstacles/B5/Tree5_1.obj";
        collidableComp.sizeX = randomScale * treeFactor;
        collidableComp.sizeZ = randomScale * treeFactor;
        drawableComp.scaleX = randomScale;
        drawableComp.scaleY = randomScale;
        drawableComp.scaleZ = randomScale;
        break;

    case OBSTACLE_B5_2:
        drawableComp.model = "./media/obstacles/B5/Block5_2.obj";
        collidableComp.sizeX = 30;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 30;
        drawableComp.scaleX = 5;
        drawableComp.scaleY = 5;
        drawableComp.scaleZ = 5;
        break;

    case OBSTACLE_B5_3:
        drawableComp.model = "./media/obstacles/B5/Obelisk5_3.obj";
        collidableComp.sizeX = 30;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 30;
        drawableComp.scaleX = 5;
        drawableComp.scaleY = 5;
        drawableComp.scaleZ = 5;
        break;

    case OBSTACLE_B5_4:
        drawableComp.model = "./media/obstacles/B5/Colum5_4.obj";
        collidableComp.sizeX = 30;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 30;
        drawableComp.scaleX = 5;
        drawableComp.scaleY = 5;
        drawableComp.scaleZ = 5;
        break;

    case OBSTACLE_B5_5:
        drawableComp.model = "./media/obstacles/B5/graveyard.obj";
        break;

    case OBSTACLE_B5_6:
        drawableComp.model = "./media/obstacles/B5/graveyard2.obj";
        break;
    }


    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (OBSTACLE));


    return entityId;
}
uint32_t EntityManager::createTerrain          (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.terrainId = entityId;

    SituationComponent      &situationComp   = createComponent<SituationComponent>(entityId);
    TerrainDataComponent    &terrainDataComp = createComponent<TerrainDataComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;


    //######### CREATE ########//
    entityMap.emplace(entityId, (TERRAIN));
    return entityId;
}
uint32_t EntityManager::createTerrainCube      (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();


    createComponent<TerrainCubeComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y + 50.f;
    situationComp.z = z;
    situationComp.rotation = 90;
    drawableComp.model = "";
    //drawableComp.texture = text; //Set in StaticEntities, avaible optimization with switch

    drawableComp.scaleX = 5;
    drawableComp.scaleY = 5;
    drawableComp.scaleZ = 5;


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "floor");
    //######### CREATE ########//
    entityMap.emplace(entityId, (TERRAINCUBE));

    return entityId;
}
uint32_t EntityManager::createInterestPoint    (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();


    createComponent<IPDataComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    SoundComponent      &soundComp          {createComponent<SoundComponent>(entityId)};
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);


    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    float colliderSize = 50;
    collidableComp.sizeY = 30;
    collidableComp.sizeZ = 50;
    collidableComp.sizeX = 50;
    collidableComp.type = STATIC;

    float drawableSize = 1;
    drawableComp.scaleX = drawableSize;
    drawableComp.scaleY = drawableSize;
    drawableComp.scaleZ = drawableSize;



    switch(goType) {
    case INTERESTPOINT_B1_1:
        soundComp.eventName =  "InterestPoints/IP0";
        drawableComp.model = "./media/interestPoints/B1/IP1_1/barrel_open.obj";
        colliderSize = 15;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B1_2:
        soundComp.eventName =  "InterestPoints/IP1";
        drawableComp.model = "./media/interestPoints/B1/IP1_2/barn.obj";
        colliderSize = 100;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize + 20;

        break;

    case INTERESTPOINT_B1_3:
        soundComp.eventName =  "InterestPoints/IP2";
        drawableComp.model = "./media/interestPoints/B1/IP1_3/mill.obj";
        colliderSize = 50;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_1:
        soundComp.eventName =  "InterestPoints/IP3";
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_fire.obj";
        colliderSize = 50;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_2:
        soundComp.eventName =  "InterestPoints/IP4";
        drawableComp.model = "./media/interestPoints/B2/IP2_2/waterTower.obj";
        colliderSize = 80;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_3:
        soundComp.eventName =  "InterestPoints/IP5";
        drawableComp.model = "./media/interestPoints/B2/IP2_3/WatchTower.obj";
        colliderSize = 50;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B3_1:
        soundComp.eventName =  "InterestPoints/IP6";
        drawableComp.model = "./media/interestPoints/B3/IP3_1/latrine.obj";
        collidableComp.sizeX = 38;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = 42;
        break;

    case INTERESTPOINT_B3_2:
        soundComp.eventName =  "InterestPoints/IP7";
        drawableComp.model = "./media/interestPoints/B3/IP3_2/Mask.obj";
        colliderSize = 40;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        drawableComp.transparency = true;
        break;

    case INTERESTPOINT_B3_3:
        soundComp.eventName =  "InterestPoints/IP8";
        drawableComp.model = "./media/interestPoints/B3/IP3_3/witchHouse.obj";
        colliderSize = 80;
        collidableComp.sizeX = colliderSize + 60;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B4_1:
        soundComp.eventName =  "InterestPoints/IP9";
        drawableComp.model = "./media/interestPoints/B4/IP4_1/castle.obj";
        colliderSize = 10;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B4_2:
        soundComp.eventName =  "InterestPoints/IP10";
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge.obj";
        colliderSize = 54;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize + 8;
        break;

    case INTERESTPOINT_B4_3:
        soundComp.eventName =  "InterestPoints/IP11";
        drawableComp.model = "./media/interestPoints/B4/IP4_3/Rock_Face.obj";
        colliderSize = 80;
        collidableComp.sizeX = colliderSize + 40;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B5_1:
        soundComp.eventName =  "InterestPoints/IP12";
        drawableComp.model = "./media/interestPoints/B5/IP5_1/mausoleum.obj";
        collidableComp.sizeX = 100;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = 74;
        break;

    case INTERESTPOINT_B5_2:
        soundComp.eventName =  "InterestPoints/IP13";
        drawableComp.model = "./media/interestPoints/B5/IP5_2/pyramid.obj";
        colliderSize = 100;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B5_3:
        soundComp.eventName =  "InterestPoints/IP14";
        drawableComp.model = "./media/interestPoints/B5/IP5_3/playerTomb.obj";
        collidableComp.sizeX = 20;
        collidableComp.sizeY = 100;
        collidableComp.sizeZ = 80;
        break;
    }

    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (INTERESTPOINT));

    return entityId;
}


uint32_t EntityManager::createInterestPointPart(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    IPPartComponent     &iPPartComp     = createComponent<IPPartComponent>(entityId);
    //######### DATA ########//
    situationComp.x         = x;
    situationComp.y         = y;
    situationComp.z         = z;
    situationComp.rotation  = r;

    float colliderSize = 10;
    float drawableSize = 1;

    collidableComp.sizeX = colliderSize;
    collidableComp.sizeY = colliderSize;
    collidableComp.sizeZ = colliderSize;
    collidableComp.type = STATIC;

    drawableComp.scaleX = drawableSize;
    drawableComp.scaleY = drawableSize;
    drawableComp.scaleZ = drawableSize;

    switch(goType) {

    case INTERESTPOINT_B1_1_P1:
        drawableComp.model = "./media/interestPoints/B1/IP1_1/woz.obj";
        colliderSize = 70;
        collidableComp.sizeX = colliderSize + 40;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;
        break;

    case INTERESTPOINT_B1_2_P1:
        drawableComp.model = "./media/interestPoints/B1/IP1_2/bale.obj";
        colliderSize = 20;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize + 10;

        break;

    case INTERESTPOINT_B1_2_P2:
        drawableComp.model = "./media/interestPoints/B1/IP1_2/bale_small.obj";
        colliderSize = 20;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize + 10;

        break;

    case INTERESTPOINT_B2_1_P1:
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_log.obj";
        colliderSize = 30;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_1_P2:
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_log2.obj";
        colliderSize = 30;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_1_P3:
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_tend.obj";
        colliderSize = 45;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_1_P4:
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_treeStump.obj";
        collidableComp.sizeX = 50;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 30;

        break;

    case INTERESTPOINT_B2_1_P5:
        drawableComp.model = "./media/interestPoints/B2/IP2_1/IP2_1_treeStump_small.obj";
        colliderSize = 15;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B2_3_P1:
        drawableComp.model = "./media/interestPoints/B2/IP2_3/WatchTower_fenceX.obj";

        colliderSize = 30;
        collidableComp.sizeX = colliderSize;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = 180;

        break;
    case INTERESTPOINT_B2_3_P2:
        drawableComp.model = "./media/interestPoints/B2/IP2_3/WatchTower_fenceZ.obj";
        colliderSize = 30;
        collidableComp.sizeX = 180;
        collidableComp.sizeY = colliderSize;
        collidableComp.sizeZ = colliderSize;

        break;

    case INTERESTPOINT_B3_3_P1:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        collidableComp.sizeX = 70;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 140;
        break;

    case INTERESTPOINT_B4_1_P1:
        drawableComp.scaleX = 0;
        drawableComp.scaleZ = 0;
        collidableComp.sizeX = 60;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 220;

        break;

    case INTERESTPOINT_B4_1_P2:
        drawableComp.scaleX = 0;
        drawableComp.scaleZ = 0;
        collidableComp.sizeX = 220;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 60;

        break;

    case INTERESTPOINT_B4_1_P3:
        drawableComp.scaleX = 0;
        drawableComp.scaleZ = 0;
        collidableComp.sizeX = 220;
        collidableComp.sizeY = 30;
        collidableComp.sizeZ = 60;

        break;

    case INTERESTPOINT_B4_2_P1:
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge_P1.obj";

        collidableComp.sizeX = 50;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 50;

        break;

    case INTERESTPOINT_B4_2_P2:
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge_P2.obj";

        collidableComp.sizeX = 40;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 40;

        break;

    case INTERESTPOINT_B4_2_P3:
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge_P3.obj";

        collidableComp.sizeX = 30;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 50;

        break;

    case INTERESTPOINT_B4_2_P4:
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge_P4.obj";

        collidableComp.sizeX = 78;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 48;

        break;

    case INTERESTPOINT_B4_2_P5:
        drawableComp.model = "./media/interestPoints/B4/IP4_2/Stonehenge_P5.obj";

        collidableComp.sizeX = 40;
        collidableComp.sizeY = 10;
        collidableComp.sizeZ = 80;

        break;

    case INTERESTPOINT_B5_1_P1:
        drawableComp.model = "./media/interestPoints/B5/IP5_1/mausoleum_P1.obj";

        collidableComp.sizeX = 15;
        collidableComp.sizeY = 15;
        collidableComp.sizeZ = 15;

        break;

    case INTERESTPOINT_B5_3_P1:
        drawableComp.model = "./media/interestPoints/B5/IP5_3/playerTomb_P1.obj";

        collidableComp.sizeX = 40;
        collidableComp.sizeY = 40;
        collidableComp.sizeZ = 40;

        break;

    case INTERESTPOINT_B5_3_P2:
        drawableComp.model = "./media/interestPoints/B5/IP5_3/playerTomb_P2.obj";

        collidableComp.sizeX = 40;
        collidableComp.sizeY = 40;
        collidableComp.sizeZ = 40;

        break;

    case INTERESTPOINT_B5_3_P3:
        drawableComp.model = "./media/interestPoints/B5/IP5_3/playerTomb_P3.obj";

        collidableComp.sizeX = 0;
        collidableComp.sizeY = 0;
        collidableComp.sizeZ = 0;
        collidableComp.type = DRILLCOLL;

        break;

    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (INTERESTPOINTPART));

    return entityId;
}



uint32_t EntityManager::createSecureZone (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t            entityId        = Entity::getCurrentId();
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SecureZoneComponent &dsecureZComp   = createComponent<SecureZoneComponent>(entityId);


    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    float COLLIDABLE_SIZE = 250;
    collidableComp.sizeX = COLLIDABLE_SIZE;
    collidableComp.sizeY = COLLIDABLE_SIZE;
    collidableComp.sizeZ = COLLIDABLE_SIZE;
    collidableComp.type  = DRILLCOLL;

    drawableComp.scaleX = COLLIDABLE_SIZE;
    drawableComp.scaleY = COLLIDABLE_SIZE;
    drawableComp.scaleZ = COLLIDABLE_SIZE;


    switch(goType) {
    case SECURE_ZONE_IP:
        drawableComp.model = "./media/secureZone/secureZoneDisabled.obj";
        drawableComp.modelSecondary = "./media/secureZone/secureZoneActive.obj";

        // Color of the light
        dsecureZComp.activeLightColor[0] = 0.15f;
        dsecureZComp.activeLightColor[1] = 1.f;
        dsecureZComp.activeLightColor[2] = 0.3f;
        break;

    case SECURE_ZONE_TOWN:
        drawableComp.model = "./media/secureZone/secureZoneDisabled.obj";
        drawableComp.modelSecondary = "./media/secureZone/secureZoneActive.obj";

        COLLIDABLE_SIZE = 400;
        collidableComp.sizeX    = COLLIDABLE_SIZE;
        collidableComp.sizeY    = COLLIDABLE_SIZE;
        collidableComp.sizeZ    = COLLIDABLE_SIZE;
        drawableComp.scaleX     = COLLIDABLE_SIZE;
        drawableComp.scaleY     = COLLIDABLE_SIZE;
        drawableComp.scaleZ     = COLLIDABLE_SIZE;
        drawableComp.transparency = true;

        // Color of the light
        dsecureZComp.activeLightColor[0] = 1.f;
        dsecureZComp.activeLightColor[1] = 0.18f;
        dsecureZComp.activeLightColor[2] = 0.1f;
        break;

    default:
        drawableComp.model = "./media/secureZone/secureZoneDisabled.obj";
        drawableComp.modelSecondary = "./media/secureZone/secureZoneActive.obj";

    }


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (SECUREZONE));

    //Create its cristal light
    uint32_t lightId = createLight(gameContext, x, 10.f, z, 0.f, LIGHT_CRISTAL);
    dsecureZComp.cristalLightId = lightId;

    return entityId;
}


uint32_t EntityManager::createKey              (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DrawableComponent    &drawableComp    = createComponent<DrawableComponent>(entityId);
    KeyComponent         &keyComp         = createComponent<KeyComponent>(entityId);
    SituationComponent   &situationComp   = createComponent<SituationComponent>(entityId);
    CollisionComponent   &collidableComp  = createComponent<CollisionComponent>(entityId);
    LevitationComponent  &levitationComp  = createComponent<LevitationComponent>(entityId);
    SoundComponent       &soundComp       = createComponent<SoundComponent>(entityId);
    IluminationComponent &iluminationComp = createComponent<IluminationComponent>(entityId);

    //######### DATA ########//
    levitationComp.initialHeight    = y + 5; //Fix under the floor
    levitationComp.levitationMargin = 5;
    levitationComp.levitationSpeed  = 8.5;

    situationComp.x = x;
    situationComp.y = y - levitationComp.levitationMargin;
    situationComp.z = z;

    collidableComp.sizeX = 30;
    collidableComp.sizeY = 180;
    collidableComp.sizeZ = 30;
    collidableComp.type = STATIC;

    float sizeKeyInEgg = 80;
    soundComp.eventName = "Key/Key";


    switch(goType) {
    case KEY_BIOME1:
        keyComp.keyType = KEY1;
        drawableComp.model = "./media/keys/key.obj";

        break;
    case KEY_BIOME2:
        keyComp.keyType = KEY2;
        drawableComp.model = "./media/keys/key.obj";
        collidableComp.sizeX = sizeKeyInEgg;
        collidableComp.sizeZ = sizeKeyInEgg;
        collidableComp.type = DRILLCOLL;


        break;
    case KEY_BIOME3:
        keyComp.keyType = KEY3;
        drawableComp.model = "./media/keys/key.obj";
        collidableComp.sizeX = sizeKeyInEgg;
        collidableComp.sizeZ = sizeKeyInEgg;
        collidableComp.type = DRILLCOLL;


        break;
    case KEY_BIOME4:
        keyComp.keyType = KEY4;
        drawableComp.model = "./media/keys/key.obj";
        collidableComp.sizeX = sizeKeyInEgg;
        collidableComp.sizeZ = sizeKeyInEgg;
        collidableComp.type = DRILLCOLL;


        break;
    case KEY_BIOME5:
        keyComp.keyType = KEY5;
        drawableComp.model = "./media/keys/key.obj";
        collidableComp.sizeX = sizeKeyInEgg;
        collidableComp.sizeZ = sizeKeyInEgg;
        collidableComp.type = DRILLCOLL;


        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (KEY));

    // Create ilumination light
    iluminationComp.pointLightId = createLight(gameContext, x, y, z, 0.f, LIGHT_ILUMINATION);


    return entityId;
}
uint32_t EntityManager::createKeyEgg           (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    WalletComponent     &walletComp     = createComponent<WalletComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    EggDataComponent    &eggComp        = createComponent<EggDataComponent>(entityId);
    StatsComponent      &statsComp      = createComponent<StatsComponent>(entityId);
    SoundComponent      &soundComp        {createComponent<SoundComponent>(entityId)};


    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    collidableComp.sizeX = 70;
    collidableComp.sizeY = 70;
    collidableComp.sizeZ = 70;
    collidableComp.type = STATIC;

    drawableComp.scaleX = 1;
    drawableComp.scaleY = 1;
    drawableComp.scaleZ = 1;

    soundComp.eventName = "Eggs/Egg";

    switch(goType) {
    case EGG_BIOME1:
        drawableComp.model = "./media/eggs/eggBrokenB1.obj";
        eggComp.biome = 1;
        statsComp.maxHealth = 0;
        statsComp.currentHealth = statsComp.maxHealth;
        eggComp.broken = true;
        //eggComp.modelEggBroken = "./media/eggs/brokenEgg.obj";
        walletComp.coins = 0;
        break;
    case EGG_BIOME2:
        drawableComp.model = "./media/eggs/eggB2.obj";
        eggComp.biome = 2;
        statsComp.maxHealth = 40;
        statsComp.currentHealth = statsComp.maxHealth;
        eggComp.broken = false;
        eggComp.modelEggBroken = "./media/eggs/eggBrokenB2.obj";
        walletComp.coins = 40;
        break;
    case EGG_BIOME3:
        drawableComp.model = "./media/eggs/eggB3.obj";
        eggComp.biome = 3;
        statsComp.maxHealth = 80;
        statsComp.currentHealth = statsComp.maxHealth;
        eggComp.broken = false;
        eggComp.modelEggBroken = "./media/eggs/eggBrokenB3.obj";
        walletComp.coins = 150;
        break;
    case EGG_BIOME4:
        drawableComp.model = "./media/eggs/eggB4.obj";
        eggComp.biome = 4;
        statsComp.maxHealth = 135;
        statsComp.currentHealth = statsComp.maxHealth;
        eggComp.broken = false;
        eggComp.modelEggBroken = "./media/eggs/eggBrokenB4.obj";
        walletComp.coins = 300;
        break;
    case EGG_BIOME5:
        drawableComp.model = "./media/eggs/eggB5.obj";
        eggComp.biome = 5;
        statsComp.maxHealth = 200;
        statsComp.currentHealth = statsComp.maxHealth;
        eggComp.broken = false;
        eggComp.modelEggBroken = "./media/eggs/eggBrokenB5.obj";
        walletComp.coins = 700;
        break;
    }


    //######### RENDER ########//
    if(DISABLED_VISIBLE_ZONE)
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (KEYEGG));

    return entityId;
}
uint32_t EntityManager::createWaypoint         (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    BoundingComponent   &sensorComp     = createComponent<BoundingComponent>(entityId);
    WaypointComponent   &wayComp        = createComponent<WaypointComponent>(entityId);
    //DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);// TEMP

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    sensorComp.sizeX = 1;
    sensorComp.sizeY = 1;
    sensorComp.sizeZ = 1;

    // TEMP:
    // SHOW WAYPOINT
    //drawableComp.texture = "media/waypoint.png";


    //######### RENDER ########//
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (WAYPOINT));

    return entityId;
}
uint32_t EntityManager::createHUD              (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {

    uint32_t entityId = Entity::getCurrentId();
    gameContext.HUDId = entityId;


    HUDComponent &hud = createComponent<HUDComponent>(entityId);

    //######### CREATE ########//
    entityMap.emplace(entityId, (HUDS));

    return entityId;
}

uint32_t EntityManager::createInfoBox(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    InfoBoxComponent    &infoBox        = createComponent<InfoBoxComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;


    drawableComp.scaleX = 0;
    drawableComp.scaleY = 0;
    drawableComp.scaleZ = 0;

    drawableComp.texture = "./media/boxes/infoBoxes/keyF.png";

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "plane");

    //######### CREATE ########//
    entityMap.emplace(entityId, (INFOBOX));


    return entityId;
}

uint32_t EntityManager::createNotification(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent    &situationComp = createComponent<SituationComponent>(entityId);
    DrawableComponent     &drawableComp  = createComponent<DrawableComponent>(entityId);
    NotificationComponent &notification  = createComponent<NotificationComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    switch(goType) {
    case NOTIFICATION_NOCOINS:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_noCoins.png";
        notification.lifeTime = 4.5;
        break;

    case NOTIFICATION_LEVEL_UP_SOUND:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_levelUp.png";
        notification.lifeTime = 5;
        break;

    case NOTIFICATION_BANK_DEPOSITOR:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_bankDepositor.png";
        notification.lifeTime = 5;
        break;

    case NOTIFICATION_BANK_EXTRACTOR:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_bankExtractor.png";
        notification.lifeTime = 5;
        break;

    case NOTIFICATION_FAILEGG:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_failEgg.png";
        notification.lifeTime = 5;
        break;

    case NOTIFICATION_FAILDOOR:
        drawableComp.scaleX = 0;
        drawableComp.scaleY = 0;
        drawableComp.scaleZ = 0;
        drawableComp.texture = "./media/boxes/notifications/notification_failDoor.png";
        notification.lifeTime = 5;
        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "plane");

    //######### CREATE ########//
    entityMap.emplace(entityId, (NOTIFICATION));


    return entityId;

}

uint32_t EntityManager::createControlsFoor(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.controlsFloorId = entityId;

    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    drawableComp.scaleX = 0;//16
    drawableComp.scaleY = 0;//0
    drawableComp.scaleZ = 0;//12
    drawableComp.texture = "./media/base/controls/controlsFloor.png";

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "floor");

    //######### CREATE ########//
    entityMap.emplace(entityId, (CONTROLSWALL));


    return entityId;
}

uint32_t EntityManager::createMessageBox(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    MessageBoxComponent &messageBox     = createComponent<MessageBoxComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;

    switch(goType) {
    case MESSAGEBOX_UPDATER:
        drawableComp.scaleX = 2;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;
        drawableComp.texture = "./media/boxes/messageBoxes/messageUpdater.png";
        break;
    case MESSAGEBOX_BANK:
        drawableComp.scaleX = 2;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;
        drawableComp.texture = "./media/boxes/messageBoxes/messageBank.png";
        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "plane");

    //######### CREATE ########//
    entityMap.emplace(entityId, (MESSAGEBOX));

    return entityId;
}

uint32_t EntityManager::createDetectionSign(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DetectionSignComponent  &detectionSignComp = createComponent<DetectionSignComponent>(entityId);
    SituationComponent      &situationComp     = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp      = createComponent<DrawableComponent>(entityId);

    //######### DATA ########//
    detectionSignComp.initialHeight    = y;
    detectionSignComp.levitationMargin = 3;
    detectionSignComp.levitationSpeed  = 2.5;
    detectionSignComp.rotationSpeed    = 75;

    situationComp.x = x;
    situationComp.y = y - detectionSignComp.levitationMargin;
    situationComp.z = z;

    switch(goType) {
    case DETECTIONSIGN_INTERROGATION:
        drawableComp.model        = "./media/detectionSigns/interrogationSign/DetectionSign_Exclamation.obj";
        drawableComp.transparency = true;
        break;

    case DETECTIONSIGN_EXCLAMATION:
        drawableComp.model        = "./media/detectionSigns/interrogationSign/DetectionSign_Exclamation.obj";
        drawableComp.transparency = true;
        break;

    case DETECTIONSIGN_IMPROVER:
        drawableComp.model        = "./media/detectionSigns/improverSign/DetectionSign_Improver.obj";
        drawableComp.transparency = true;
        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);

    //######### CREATE ########//
    entityMap.emplace(entityId, (DETECTIONSIGN));

    return entityId;
}

uint32_t EntityManager::createCompass(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    gameContext.compassId = entityId;

    CompassComponent     &Compass        = createComponent<CompassComponent>(entityId);
    DrawableComponent    &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent   &situationComp  = createComponent<SituationComponent>(entityId);
    VelocityComponent    &velocityComp   = createComponent<VelocityComponent>(entityId);
    SoundComponent       &soundComp      = createComponent<SoundComponent>(entityId);

    //######### DATA ########//
    soundComp.eventName = "Compass/Compass";


    situationComp.x        = x;
    situationComp.y        = y;
    situationComp.z        = z;
    situationComp.rotation = r;

    velocityComp.speed       = 60;
    velocityComp.speedNormal = 8;
    velocityComp.speedUpped  = 60;

    drawableComp.scaleX = 1;
    drawableComp.scaleY = 1;
    drawableComp.scaleZ = 1;
    //drawableComp.texture = "./media/compass/compass.png";
    drawableComp.model = "./media/compass/compass.obj";

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (COMPASS));

    return entityId;
}

uint32_t EntityManager::createAttack   (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DrawableComponent   &drawableComp   = createComponent<DrawableComponent>(entityId);
    SituationComponent  &situationComp  = createComponent<SituationComponent>(entityId);
    CollisionComponent  &collidableComp = createComponent<CollisionComponent>(entityId);
    VelocityComponent   &velocityComp   = createComponent<VelocityComponent>(entityId);
    AttackComponent     &attackComp     = createComponent<AttackComponent>(entityId);

    //######### DATA ########//
    situationComp.x        = x;
    situationComp.y        = y;
    situationComp.z        = z;
    situationComp.rotation = r;

    switch(goType) {
    case ATTACK_MELEE:
        if(!SHOW_MELEEATTACKS) {
            attackComp.attackType = ATTACKMELEE;
        } else {
            attackComp.attackType = ATTACKDISTANCE;
            gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
        }

        break;
    case ATTACK_DISTANCE:
        attackComp.attackType = ATTACKDISTANCE;
        drawableComp.transparency = true;
        //######### RENDER ########//
        gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
        break;
    }
    //######### CREATE ########//
    entityMap.emplace(entityId, (ATTACK));


    return entityId;
}

uint32_t EntityManager::createNPC   (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    DialogueComponent       &dialogueComp   = createComponent<DialogueComponent>(entityId);
    DrawableComponent       &drawableComp   = createComponent<DrawableComponent>(entityId);
    WalletComponent         &walletComp     = createComponent<WalletComponent>(entityId);
    SituationComponent      &situationComp  = createComponent<SituationComponent>(entityId);
    CoinDetectorComponent   &coinDetectComp = createComponent<CoinDetectorComponent>(entityId);
    CollisionComponent      &collidableComp = createComponent<CollisionComponent>(entityId);
    VelocityComponent       &velocityComp   = createComponent<VelocityComponent>(entityId);


    //######### DATA ########//
    situationComp.x        = x;
    situationComp.y        = y;
    situationComp.z        = z;
    situationComp.rotation = r;

    switch(goType) {
    case NPC_SHAMAN_B0_STATSUPDATER:
        gameContext.npcShamanImprover = entityId;
        drawableComp.model = "./media/npc/Shaman/NPC_ShamanPose.obj";
        //drawableComp.texture = "./media/npc/npc1.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 0;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        //walletComp.coins            = 0;
        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Hola, soy el chamán.@Puedo ayudarte a@canalizar tus energías."},
            {"Para poder hacerlo,@tráeme cristales de@energía."},
            {"Estos cristales están@dentro de todos los@seres de este mundo."},
            {"Subir el primer nivel@será más fácil que @subir el resto."},
            {"¿Subir al nivel 2?@Para ello necesitaré@" + std::to_string((int)pow(2, (0.6 * (1 + 5)))) + " cristales."},
            {"Tus habilidades han@alcanzado su máximo@potencial.", DEMANDCOINSTOIMPROVE},
            {"Tu poder podría@compararse al de los@dioses de este mundo."}
        };
        break;

    case NPC_OLDMAN_B0_STICK:
        gameContext.npcOldMan = entityId;
        drawableComp.model = "./media/npc/OldMan/Anim/NPC_OldMan.obj";
        //drawableComp.texture = "./media/npc/npc1.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 0;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        //walletComp.coins            = 0;
        //dialogueComp.coinsToReceive = 0;
        dialogueComp.weaponForSale  = {STICK};         //-------------------MAX NUMBER CHAR BETWEEN '@': 38 chars-------------------
        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Jovencito, ¿estás bien?"},
            {"Te has alejado de la@zona protegida por el@cristal de energía."},
            {"Pareces herido.@Deberías volver al@pueblo para curarte."},
            {"Aunque no me sorprende.@Se te ve un muchacho@muy escuálido."},
            {"Consigue algunos@cristales y visita al@Chamán del pueblo."},
            {"Toma, mi viejo palo.@Prueba a varear algún@puercornio con él.", GIVEWEAPON},
            {"Pero primero deberías@volver al cristal del@pueblo para curarte."},
            {"Recuerdo cuando tenía@tu edad ¡cómo esquivaba@piñas entonces!"}
        };
        break;

    case NPC_MOTHER_B0_INFORMATION:
        drawableComp.model  = "./media/npc/Mother/Anim/NPC_Mother.obj";
        //drawableComp.texture = "./media/npc/npc2.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 0;
        dialogueComp.wantToTalk     = true;    //if wantToTalk. you can interaction with him.
        dialogueComp.loopDialogue   = true;    //if loopDialogue, he repeats all the dialogue in loop.
        dialogueComp.uniqueSentence = false; //if uniqueSentence, his wantToTalk turn to true when dialogue has finished.
        //walletComp.coins            = 0;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Si necesitas restaurar@tu salud acércate al@cristal de vida."},
            {"Y, si alguna vez mueres@ahí afuera, volverás a@esta caseta."},
            {"Pero cuidado, ¡perderás@todos tus cristales de@energía al morir!"},
            {"Es importante guardar@los cristales que no@quieres perder."},
            {"No te procupes por tus@armas, alguien se@encargará de traerlas."},
            {"¿Has hablado ya con el@chamán? Tiene muchísimo@poder."},
            {"¡Puede transformar tus@cristales en fuerza!"}
        };
        break;

    case NPC_GUITARIST_B0_COMPASS:
        drawableComp.model   = "./media/npc/Guitarist/Anim/NPC_Guitarist.obj";
        //drawableComp.texture = "./media/npc/npc3.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 0;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        //dialogueComp.coinsToReceive = 25;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        //walletComp.coins            = 0;

        // TAKE CARE!! This vector is modified in dialogue system and its size shouldnt be touched

        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Existen distintos tipos@de cristales en este@mundo."},
            {"Los cristales de@protección alejan toda@clase de peligros."},
            {"El cristal de vida@suministra salud a todo@el que se aproxima."},
            {"Y los cristales de@energía nos permiten@mejorar las habilidades."},
            {"...Hmmm"},
            {"El otro día buscando@nuevos cristales@encontré esta brújula."},
            {"Tómala, no sé a@dónde apunta pero@quizá sea interesante.", UNLOCKCOMPASS},
            {"Avísame si lo descubres."},
            {"..."},
            //{"En noches cuando la@luna como plata se@eleva, la selva ilumina"},
            //{"y también la pradera...@...viejos sabios nos@guían al gran cristal."},
            {"Mira como brilla,@es tan bonito."}
            // When you have found the palantir, news sentences are added
            //{"Así que la brújula era@atraída por ese orbe."}
            //{"Quizá sea útil para@recordar la ubicación@de algo."}

        };
        dialogueComp.sentences.reserve(dialogueComp.sentences.size() + 2);
        break;

    case NPC_FARMER_B1_SLINGSHOT:
        drawableComp.model   = "./media/npc/Farmer/Anim/NPC_Farmer.obj";
        //drawableComp.texture = "./media/npc/npc4.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 1;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        dialogueComp.coinsToReceive = 25;
        dialogueComp.weaponForSale  = {SLINGSHOT};
        //walletComp.coins            = 0;
        dialogueComp.sentences = {
            //{"12345678901234567890123", REPORT}
            {"Todo aventurero que se@precie debe aprender a@dominar a sus enemigos."},
            {"Siempre viene bien tener@un arma que permita@guardar las distancias."},
            {"Te vendo un tirachinas.@@Necesitas " + std::to_string((int)dialogueComp.coinsToReceive) + " cristales."},
            {"Ten cuidado, una gran@responsabilidad requie-@re un gran...¿Cómo era?", DEMANDCOINS},
            {"¿Has probado a disparar@a un pollito? ¡Son muy@escurridizos!"}
        };
        break;

    case NPC_GENTLEMAN_B1_KEY:
        drawableComp.model  = "./media/npc/Gentleman/Anim/NPC_Gentleman.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 1;
        dialogueComp.wantToTalk     = true;     //if wantToTalk. you can interaction with him.
        dialogueComp.loopDialogue   = false;    //if loopDialogue, he repeats all the dialogue in loop.
        dialogueComp.uniqueSentence = false;    //if uniqueSentence, his wantToTalk turn to true when dialogue has finished.
        //walletComp.coins            = 0;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};

        // TAKE CARE!! This vector is modified in dialogue system and its size shouldnt be touched
        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Ya sé lo que viene@usted a contarme, joven."},
            {"Pero le diré una cosa,@de mujeres y dinero no@habla un caballero."},
            {"¡Y le aseguro que soy@muy bueno para los@negocios!"},
            {"El otro día le compré@esta llave a un@campesino afortunado."},
            {"¡Dijo que la encontró@en este huevo!...@Majareta..."},
            {"Pero usted parece más@necesitado que yo, tome,@se la regalo.", GIVEFRAGMENTKEY},
            {"Además, parece parte de@una colección, y no me@apetece buscar el resto."},
            {"¿Has completado ya la@colección de llaves?"}
            // When you have all keys, news sentences are added:
            //{"Vaya, parece que la@colección está completa.@Me pregunto qué abrirán."}
            //{"Quizá Bip-bop sepa algo@acerca de estas llaves;@debe estar en la sabana."}
        };
        dialogueComp.sentences.reserve(dialogueComp.sentences.size() + 2);
        break;

    case NPC_COWBOY_B2_MAP:
        gameContext.cowboyId = entityId;

        drawableComp.model  = "./media/npc/Cowboy/Anim/NPC_Cowboy.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 2;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        //dialogueComp.coinsToReceive = 25;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        //walletComp.coins            = 0;
        dialogueComp.sentences = {
            //          {"12345678901234567890123"}
            {"Pareces un poco@perdido."},
            {"¿Has visto ese orbe? es@un Orbe Vinculador.@Atrae a las brújulas."},
            {"Yo ya no lo necesito,@he memorizado mi mapa.@Te regalo ambas cosas", UNLOCKMAP},
            {"Es muy peligroso cruzar@tu puerta. Vas hacia el@Camino y si no cuidas..."},
            {"...tus pasos no sabes@hacia dónde te@arrastrarán."},
            {"¿No entiendes que este@camino atraviesa el@Bosque Pinoso,"},
            {"y que si no prestas@atención puede llevarte@a la Ciénaga Venenosa,"},
            {"y más lejos aún y a@sitios peores?"},
            {"Las sendas del mundo@son inciertas. Vigila@tus pasos."}
        };
        break;

    case NPC_PIRATE_B2_COINS:
        drawableComp.model   = "./media/npc/Pirate/NPC_Pirate.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 2;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        walletComp.coins            = 30;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        dialogueComp.sentences = {
            {"Arrr, ¡Soy rico!, ¿me@oyes?, arrr, ¡Soy rico!"},
            {"El otro día encontré,@arrr, un cofre repleto@de cristales preciosos."},
            {"Arrr, pero no puedo@cargar con, arrr, todos@hasta el pueblo, arrr."},
            {"Pesan demasiado, arrr,@toma un puñado, arrr.", GIVECOINS},
            {"Dejaré que, arrr, los@inviertas en mi próximo@negocio, arrr."},
            {"Sí... lo sé, arrr, el@dinero me hace mejor,@arrr, persona."},
            {"Arrr."}
        };
        break;

    case NPC_FUMIGATOR_B3_COINS:
        drawableComp.model  = "./media/npc/Fumigator/Anim/NPC_Fumigator.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 3;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        walletComp.coins            = 100;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        dialogueComp.sentences = {
            {"Ahora soy fumigador de@arañas, pero antes lo@era de brujas."},
            {"¿Ves alguna bruja por@aquí? De nada."},
            {"Me has caído bien, toma@estos cristales.", GIVECOINS},
            {"Adoro el olor a araña@aplastada."}
        };
        break;

    case NPC_INDIAN_B3_BLOWGUN:
        drawableComp.model   = "./media/npc/Indian/Anim/NPC_Indian.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 3;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        //walletComp.coins            = 0;
        dialogueComp.coinsToReceive = 350;
        dialogueComp.weaponForSale  = {BLOWGUN};
        dialogueComp.sentences = {
            //          {"12345678901234567890123"}
            {"Salam anak muda."},
            {"Kamu terlihat sangat@pendiam."},
            {"Saya menjual tabung@pembunuh untuk " + std::to_string((int)dialogueComp.coinsToReceive) + "@kristal."},
            {"Anda harus meniup,@bukan vakum.", DEMANDCOINS},
            {"Hati-hati."}
        };
        break;

    case NPC_ROBOT_B4_INFORMATION: {
        drawableComp.model  = "./media/npc/Robot/Anim/NPC_Robot.obj";
        //drawableComp.texture = "./media/npc/npc2.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 0;
        dialogueComp.wantToTalk     = true;    //if wantToTalk. you can interaction with him.
        dialogueComp.loopDialogue   = true;    //if loopDialogue, he repeats all the dialogue in loop.
        dialogueComp.uniqueSentence = false; //if uniqueSentence, his wantToTalk turn to true when dialogue has finished.
        //walletComp.coins            = 0;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        uint8_t numKeys = gameContext.entityMan.getComponent<InventoryComponent>(gameContext.playerId).MAX_KEYS;

        // TAKE CARE!! This vector is modified in dialogue system and its size shouldnt be touched
        dialogueComp.sentences = {
            //          {"12345678901234567890123", REPORT}
            {"Bip bop bip."},
            {"He visto una puerta muy@grande y extraña, en el@muro."},
            {"Bop bip bop."},
            {"No he podido forzarla.@¡Tenía " + std::to_string((int)numKeys) + " cerraduras!"}
            // When you have all keys, news sentences are added:
            //{"Quizá tenga algo que@ver con esa colección@de llaves que tienes."},

        };
        dialogueComp.sentences.reserve(dialogueComp.sentences.size() + 1);
    }
    break;

    case NPC_SKELETON_B5_COINS:
        drawableComp.model  = "./media/npc/Skeleton/NPC_Skeleton.obj";
        //drawableComp.texture = "./media/npc/npc5.png";
        drawableComp.scaleX = 1;
        drawableComp.scaleY = 1;
        drawableComp.scaleZ = 1;

        dialogueComp.biome          = 3;
        dialogueComp.wantToTalk     = true;
        dialogueComp.loopDialogue   = false;
        dialogueComp.uniqueSentence = false;
        walletComp.coins            = 512;
        //dialogueComp.coinsToReceive = 0;
        //dialogueComp.weaponForSale  = {UNKNOWN};
        dialogueComp.sentences = {
            {"¡Hey! Hola...@¿No tienes miedo de mí?"},
            {"Has sido el primero que@no ha salido corriendo@al verme. Gracias."},
            {"Te regalo esto.", GIVECOINS},
            {"La muerte es solo el@principio..."}
        };
        break;
    }

    //ADD LEVITATION COMPONENT TO SHAMAN
    if(goType == NPC_SHAMAN_B0_STATSUPDATER) {
        LevitationComponent &levitationComp = createComponent<LevitationComponent>(entityId);
        levitationComp.initialHeight    = y; //Fix under the floor
        levitationComp.levitationMargin = 5;
        levitationComp.levitationSpeed  = 5.5;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(std::piecewise_construct, std::forward_as_tuple(entityId), std::forward_as_tuple(NPC, goType));
    /*
        //######### ANIMATIONS ########//
        if(goType == NPC_MOTHER_B0_INFORMATION || goType == NPC_GUITARIST_B0_COMPASS
                || goType == NPC_FARMER_B1_SLINGSHOT || goType == NPC_GENTLEMAN_B1_KEY) {
            gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, entityId, 1.f / 20.f);
        } else if(goType == NPC_INDIAN_B3_BLOWGUN) {
            gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, entityId, 1.f / 13.f);
        } else if(goType == NPC_ROBOT_B4_INFORMATION || goType == NPC_COWBOY_B2_MAP) {
            gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, entityId, 1.f / 15.f);
        } else {
            gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, entityId, 1.f / 30.f);
        }

        gameContext.getWindowFacadeRef().setAnimationLoop(gameContext, entityId, true);
    */
    return entityId;
}

uint32_t EntityManager::createMeleeWeapon      (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent   &situationComp   = createComponent<SituationComponent>(entityId);
    LevitationComponent  &levitationComp  = createComponent<LevitationComponent>(entityId);
    SoundComponent       &soundComp       = createComponent<SoundComponent>(entityId);
    IluminationComponent &iluminationComp = createComponent<IluminationComponent>(entityId);



    //######### DATA ########//
    soundComp.eventName = "Weapons/Weapon";

    levitationComp.initialHeight    = y;
    levitationComp.levitationMargin = 3.5;
    levitationComp.levitationSpeed  = 6.5;

    situationComp.x = x;
    situationComp.y = y - levitationComp.levitationMargin;
    situationComp.z = z;
    situationComp.rotation = r;

    CollisionComponent &collidableComp = createComponent<CollisionComponent>(entityId);
    collidableComp.sizeX   = 30; // 10
    collidableComp.sizeY   = 10;
    collidableComp.sizeZ   = 30; // 10
    collidableComp.type    = STATIC;
    Drawable2DComponent &drawable2DComp = createComponent<Drawable2DComponent>(entityId);
    DrawableComponent &drawableComp = createComponent<DrawableComponent>(entityId);
    drawableComp.scaleX    = 1;
    drawableComp.scaleY    = 1;
    drawableComp.scaleZ    = 1;
    MeleeWeaponComponent &meleeComp = createComponent<MeleeWeaponComponent>(entityId);
    /*gameContext.entityMan.createMeleeWeapon(gameContext, 0, 0, -50, 0, MELEE_MACE);
        //gameContext.entityMan.createMeleeWeapon(gameContext, 0, 0, -50, 0, MELEE_STICK);
       */
    switch(goType) {
    case MELEE_SWORD:
        meleeComp.eventName = "Weapons/Sword";

        drawable2DComp.texture2D        = "./media/hud/weapons/sword.png";
        drawable2DComp.secondTexture2D  = "./media/hud/weapons/swordSecond.png";
        drawableComp.model              = "./media/weapons/sword/sword.obj";
        drawableComp.texture            = "./media/weapons/meleeWeapon1.png";
        meleeComp.damage                  = 2;  // DPS: 5
        meleeComp.damageRange             = 10;
        meleeComp.criticalPercent         = 20;
        meleeComp.effectPercent           = 100;
        meleeComp.attackSize              = 20;
        meleeComp.sizeSeparationAttacks   = 0;
        meleeComp.posOrigin               = 10;
        meleeComp.numAttacks              = 2;
        meleeComp.biome                   = 5;  //CHANGE TO ALL BIOMES?
        meleeComp.maxCoolDown             = 0.4; //Cool down of Action
        meleeComp.attackApparitionTime    = 0.2; //Cool down of attackAparition
        meleeComp.effect                  = HEALTHSTEAL; //CHANGE
        meleeComp.weapon                  = SWORD;
        meleeComp.defaultPosition[0]      = 70;
        meleeComp.defaultPosition[1]      = 0;
        meleeComp.defaultPosition[2]      = 0;
        break;
    case MELEE_MACE:
        meleeComp.eventName = "Weapons/Mace";


        drawable2DComp.texture2D        = "./media/hud/weapons/hammer.png";
        drawable2DComp.secondTexture2D  = "./media/hud/weapons/hammerSecond.png";
        drawableComp.model              = "./media/weapons/mace/hammer.obj";

        collidableComp.sizeX   = 30;
        collidableComp.sizeY   = 10;
        collidableComp.sizeZ   = 30;

        meleeComp.damage                  = 1.5; // DPS: 2.5
        meleeComp.damageRange             = 35;
        meleeComp.criticalPercent         = 35;
        meleeComp.effectPercent           = 50;
        meleeComp.attackSize              = 40;
        meleeComp.sizeSeparationAttacks   = 0;
        meleeComp.posOrigin               = 40;
        meleeComp.numAttacks              = 1;
        meleeComp.biome                   = 2;
        meleeComp.maxCoolDown             = 0.6; //Cool down of Action
        meleeComp.attackApparitionTime    = 0.5; //Cool down of attackAparition
        meleeComp.effect                  = PARALYSIS; //CHANGE
        meleeComp.weapon                  = MACE;
        /*0, 0, -50*/
        meleeComp.defaultPosition[0]      = 0;
        meleeComp.defaultPosition[1]      = 0;
        meleeComp.defaultPosition[2]      = -70;
        break;
    case MELEE_STICK:
        meleeComp.eventName = "Weapons/Stick";

        drawable2DComp.texture2D        = "./media/hud/weapons/stick.png";
        drawable2DComp.secondTexture2D  = "./media/hud/weapons/stickSecond.png";
        drawableComp.model              = "./media/weapons/stick/stick.obj";
        //drawableComp.model              = "./media/weapons/stick/stick.obj";
        //drawableComp.texture            = "./media/weapons/stickWeapon.png";
        meleeComp.damage                  = 0.4; // DPS: 2
        meleeComp.damageRange             = 20;
        meleeComp.criticalPercent         = 5;
        meleeComp.effectPercent           = 0;
        meleeComp.attackSize              = 16;
        meleeComp.sizeSeparationAttacks   = 0;
        meleeComp.posOrigin               = 6;
        meleeComp.numAttacks              = 2;
        meleeComp.biome                   = 0; //CHANGE
        meleeComp.maxCoolDown             = 0.2; //Cool down of Action
        meleeComp.attackApparitionTime    = 0.1; //Cool down of attackAparition
        meleeComp.effect                  = NOEFFECT; //CHANGE
        meleeComp.weapon                  = STICK;
        meleeComp.defaultPosition[0]      = -50;
        meleeComp.defaultPosition[1]      = 0;
        meleeComp.defaultPosition[2]      = 50;
        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (MELEEWEAPON));

    // Create ilumination light
    iluminationComp.pointLightId = createLight(gameContext, x, y, z, 0.f, LIGHT_ILUMINATION);


    //######### BARREL ########//
    if (!gameContext.weaponsCreated[goType]) {
        createVillageObstacle(gameContext, meleeComp.defaultPosition[0], meleeComp.defaultPosition[1], meleeComp.defaultPosition[2], 0.f, VILLAGE_OBSTACLE_BARREL2);
        gameContext.weaponsCreated[goType] = true;
    }

    return entityId;
}
uint32_t EntityManager::createDistanceWeapon   (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();


    SituationComponent      &situationComp   = createComponent<SituationComponent>(entityId);
    LevitationComponent     &levitationComp  = createComponent<LevitationComponent>(entityId);
    SoundComponent          &soundComp       = createComponent<SoundComponent>(entityId);
    CollisionComponent      &collidableComp  = createComponent<CollisionComponent>(entityId);
    Drawable2DComponent     &drawable2DComp  = createComponent<Drawable2DComponent>(entityId);
    DrawableComponent       &drawableComp    = createComponent<DrawableComponent>(entityId);
    DistanceWeaponComponent &distanceComp    = createComponent<DistanceWeaponComponent>(entityId);
    IluminationComponent    &iluminationComp = createComponent<IluminationComponent>(entityId);

    //######### DATA ########//
    soundComp.eventName = "Weapons/Weapon";


    levitationComp.initialHeight    = y;
    levitationComp.levitationMargin = 3.5;
    levitationComp.levitationSpeed  = 6.5;

    situationComp.x = x;
    situationComp.y = y - levitationComp.levitationMargin;
    situationComp.z = z;
    situationComp.rotation = r;

    collidableComp.sizeX   = 40;
    collidableComp.sizeY   = 60;
    collidableComp.sizeZ   = 40;
    collidableComp.type    = STATIC;


    drawableComp.scaleX    = 1;
    drawableComp.scaleY    = 1;
    drawableComp.scaleZ    = 1;

    /* gameContext.entityMan.createDistanceWeapon(gameContext, -50, 0, 0, 45, DISTANCE_BOW);

        gameContext.entityMan.createDistanceWeapon(gameContext, 50, 0, 50, 0, DISTANCE_SLINGSHOT);
        gameContext.entityMan.createDistanceWeapon(gameContext, 50, 0, -50, 0, DISTANCE_BLOWGUN);
    */
    switch(goType) {
    case DISTANCE_BOW:
        distanceComp.eventName = "Weapons/Bow";

        drawable2DComp.texture2D       = "./media/hud/weapons/bow.png";
        drawable2DComp.secondTexture2D = "./media/hud/weapons/bowSecond.png";
        drawableComp.model              = "./media/weapons/bow/bow.obj";
        //drawableComp.texture            = "./media/weapons/maceWeapon.png";
        distanceComp.damage                  = 1.2; // DPS: 2
        distanceComp.damageRange             = 20;
        distanceComp.criticalPercent         = 20;
        distanceComp.effectPercent           = 35;
        distanceComp.attackSize              = 7;
        distanceComp.sizeSeparationAttacks   = 0;
        distanceComp.posOrigin               = 10;
        distanceComp.numAttacks              = 1;
        distanceComp.biome                   = 4;
        distanceComp.maxCoolDown             = 0.6; //Cool down of Action
        distanceComp.attackApparitionTime    = 0.6; //Cool down of attackAparition
        distanceComp.effect                  = WINHEART;
        distanceComp.weapon                  = BOW;
        distanceComp.bulletModel             = "./media/weapons/bow/arrow.obj";
        distanceComp.bulletEffectModel       = "./media/weapons/bow/arrowEffect.obj";
        distanceComp.bulletSpeed             = 600;
        distanceComp.bulletLifeTime          = 2;
        distanceComp.collisionType           = KINEMATIC;
        distanceComp.defaultPosition[0]      = -70;
        distanceComp.defaultPosition[1]      = 0;
        distanceComp.defaultPosition[2]      = 0;
        break;
    case DISTANCE_BLOWGUN:
        distanceComp.eventName = "Weapons/Blowgun";

        drawable2DComp.texture2D       = "./media/hud/weapons/blowgun.png";
        drawable2DComp.secondTexture2D = "./media/hud/weapons/blowgunSecond.png";
        //drawableComp.model     = "./media/weapons/blowgun/cerbatana.obj";
        //drawableComp.texture   = "./media/weapons/blowgunWeapon.png";
        drawableComp.model              = "./media/weapons/blowgun/blowgun.obj";
        //drawableComp.texture            = "./media/weapons/maceWeapon.png";
        distanceComp.damage                  = 0.4; // DPS: 1
        distanceComp.damageRange             = 10;
        distanceComp.criticalPercent         = 20;
        distanceComp.effectPercent           = 60;
        distanceComp.attackSize              = 6;
        distanceComp.sizeSeparationAttacks   = 0;
        distanceComp.posOrigin               = 0;
        distanceComp.numAttacks              = 1;
        distanceComp.biome                   = 3;
        distanceComp.maxCoolDown             = 0.3; //Cool down of Action
        distanceComp.attackApparitionTime    = 0.2; //Cool down of attackAparition
        distanceComp.effect                  = POISON; //CHANGE
        distanceComp.weapon                  = BLOWGUN;
        distanceComp.bulletModel             = "./media/weapons/blowgun/dart.obj";
        distanceComp.bulletEffectModel       = "./media/weapons/blowgun/poisonedDart.obj";
        distanceComp.bulletSpeed             = 500;
        distanceComp.bulletLifeTime          = 0.4;
        distanceComp.collisionType           = KINEMATIC;
        distanceComp.defaultPosition[0]      = 50;
        distanceComp.defaultPosition[1]      = 0;
        distanceComp.defaultPosition[2]      = -50;
        break;
    case DISTANCE_SLINGSHOT:
        distanceComp.eventName = "Weapons/Slingshoot";

        drawable2DComp.texture2D       = "./media/hud/weapons/slingshot.png";
        drawable2DComp.secondTexture2D = "./media/hud/weapons/slingshotSecond.png";
        drawableComp.model              = "./media/weapons/slingshot/slingShot.obj";
        //drawableComp.texture   = "./media/weapons/slingshotWeapon.png";
        distanceComp.damage                  = 0.5; // DPS: 1
        distanceComp.damageRange             = 35;
        distanceComp.criticalPercent         = 25;
        distanceComp.effectPercent           = 25;
        distanceComp.attackSize              = 5;
        distanceComp.sizeSeparationAttacks   = 0;
        distanceComp.posOrigin               = 10;
        distanceComp.numAttacks              = 1;
        distanceComp.biome                   = 1;
        distanceComp.maxCoolDown             = 0.4; //Cool down of Action
        distanceComp.attackApparitionTime    = 0.4; //Cool down of attackAparition
        distanceComp.effect                  = SLOWDOWN; //CHANGE
        distanceComp.weapon                  = SLINGSHOT;
        distanceComp.bulletModel             = "./media/weapons/slingshot/rock.obj";
        distanceComp.bulletEffectModel             = "./media/weapons/slingshot/ice_rock.obj";
        distanceComp.bulletSpeed             = 300;
        distanceComp.bulletLifeTime          = 0.5;
        distanceComp.collisionType           = KINEMATIC;
        distanceComp.defaultPosition[0]      = 50;
        distanceComp.defaultPosition[1]      = 0;
        distanceComp.defaultPosition[2]      = 50;
        break;
    }

    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### SOUND ########//
    gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (DISTANCEWEAPON));

    // Create ilumination light
    iluminationComp.pointLightId = createLight(gameContext, x, y, z, 0.f, LIGHT_ILUMINATION);


    //######### BARREL ########//
    if (!gameContext.weaponsCreated[goType]) {
        createVillageObstacle(gameContext, distanceComp.defaultPosition[0], distanceComp.defaultPosition[1], distanceComp.defaultPosition[2], 0.f, VILLAGE_OBSTACLE_BARREL2);
        gameContext.weaponsCreated[goType] = true;
    }

    return entityId;
}

uint32_t EntityManager::createPalantir(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.palantirId = entityId;

    SituationComponent      &situationComp  = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp   = createComponent<DrawableComponent>(entityId);
    CollisionComponent      &collidableComp   = createComponent<CollisionComponent>(entityId);
    LevitationComponent     &levitationComp = createComponent<LevitationComponent>(entityId);
    //SoundComponent      &soundComp      = createComponent<SoundComponent>(entityId);

    //######### DATA ########//
    levitationComp.initialHeight    = y + 8; //Fix under the floor
    levitationComp.levitationMargin = 1.9;
    levitationComp.levitationSpeed  = 5;

    collidableComp.type = DYNAMIC;
    collidableComp.sizeX   = 15;
    collidableComp.sizeY   = 15;
    collidableComp.sizeZ   = 15;

    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    situationComp.rotation = r;

    drawableComp.scaleX    = 1;
    drawableComp.scaleY    = 1;
    drawableComp.scaleZ    = 1;
    drawableComp.model = "./media/compass/palantir.obj";
    drawableComp.hasShadow = true;
    drawableComp.transparency = true;




    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (PALANTIR));


    return entityId;
}

uint32_t EntityManager::mouseShape(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();
    gameContext.mouseShapeId = entityId;

    SituationComponent      &situationComp  = createComponent<SituationComponent>(entityId);
    DrawableComponent       &drawableComp   = createComponent<DrawableComponent>(entityId);
    BoundingComponent       &sensorComp = createComponent<BoundingComponent>(entityId);

    //######### DATA ########//
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;
    situationComp.rotation = r;

    drawableComp.scaleX    = 1;
    drawableComp.scaleY    = 1;
    drawableComp.scaleZ    = 1;
    drawableComp.model = "./media/mouseShape/mouseShape.obj";
    drawableComp.hasShadow = false;

    sensorComp.sizeX = 50;
    sensorComp.sizeY = 50;
    sensorComp.sizeZ = 50;


    //######### RENDER ########//
    gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (SHAPE_MOUSE));


    return entityId;
}

uint32_t EntityManager::createLight(GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SituationComponent &situationComp  = createComponent<SituationComponent>(entityId);
    LightComponent     &lightComp      = createComponent<LightComponent>(entityId);

    //######### DATA ########//

    situationComp.rotation = r;
    situationComp.x = x;
    situationComp.y = y;
    situationComp.z = z;


    switch(goType) {
    case LIGHT_DIRECTIONAL:
        gameContext.dirLightId = entityId;

        gameContext.getWindowFacadeRef().createDirectionalLight(gameContext, entityId);

        break;
    case LIGHT_POINT: // Not used now
        //gameContext.lightPointIds.push_back(entityId);

        break;
    case LIGHT_ILUMINATION: // Not used now
        lightComp.constant   = 1.f;
        lightComp.linear     = 0.030f;
        lightComp.quadratic  = 0.0050f;

        lightComp.color[0]   = 1.47f; // 247, 255, 87
        lightComp.color[1]   = 1.5f;
        lightComp.color[2]   = 0.84f;

        gameContext.getWindowFacadeRef().createPointLight(gameContext, entityId);

        break;
    case LIGHT_MOUSE:
        gameContext.mouseLightId = entityId;

        break;
    case LIGHT_CRISTAL:
        gameContext.lightCristalIds.push_back(entityId);

        // Set point light data
        //1.f, 0.014f, 0.0007f
        lightComp.constant   = 1.f;
        lightComp.linear     = 0.014f;
        lightComp.quadratic  = 0.0007f;

        lightComp.color[0]   = 0.15f;
        lightComp.color[1]   = 1.f;
        lightComp.color[2]   = 0.3f;

        break;
    }

    //######### RENDER ########//
    //if(DISABLED_VISIBLE_ZONE)
    //gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (LIGHT));

    return entityId;
}



uint32_t EntityManager::createSoundEvent   (GameEngine &gameContext, float x, float y, float z, float r, GameObjectType goType) {
    uint32_t entityId = Entity::getCurrentId();

    SoundComponent  &soundComp   {createComponent<SoundComponent>(entityId)};

    //######### DATA ########//

    soundComp.volume  = 1;
    soundComp.pitch   = 1;
    bool initPlaying = true;//if event is a shoot not init playing


    switch(goType) {
    case MUSIC_AMBIENT:
        soundComp.eventName = "Ambient/AmbientMusic";
        gameContext.ambietMusicId = entityId;
        soundComp.initialVolume = 0;
        soundComp.setVolume(0.8);
        if(!ENABLED_AMBIENT_MUSIC) {
            initPlaying = false;
        }

        break;
    case AMBIENT_SFX:
        soundComp.eventName = "Ambient/AmbientSFX";
        gameContext.ambientSFXId = entityId;
        soundComp.initialVolume = 0.8;
        if(!ENABLED_AMBIENT_MUSIC) {
            initPlaying = false;
        }

        break;
    case SOUNT_HEART:
        soundComp.eventName = "Player/Heart";
        gameContext.heartSoundId = entityId;
        soundComp.initialVolume = 0;
        soundComp.initialPitch = 0;

        break;


        break;

    case PLAYER_ACTIONS:
        soundComp.eventName = "Player/PlayerActions";
        gameContext.playerActionsEventId = entityId;

        break;

    case WIN_SOUND_SHOOT:
        soundComp.eventName = "GameStates/Win";
        gameContext.winEventId = entityId;
        initPlaying = false;

        break;

    case GAMEOVER_SOUND_SHOOT:
        soundComp.eventName = "GameStates/Game-Over";
        gameContext.gameOverEventId = entityId;
        initPlaying = false;

        break;


    }



    //######### SOUND ########//
    if (initPlaying)
        gameContext.getSoundFacadeRef().playEvent(gameContext, entityId);
    //######### CREATE ########//
    entityMap.emplace(entityId, (SOUNDEVENT));

    return entityId;
}







Entity &EntityManager::getEntity(uint32_t id) {
    return entityMap.at(id);
}

/* Tonto estabas buscando esto? que no hay que usarlo, pringao
std::vector<uint32_t> EntityManager::getEntitiesIdByType(EntityType type) {
    std::vector<uint32_t> entities;
    for (auto &pair : entityMap)
        if (pair.second.getType() == type)
            entities.emplace_back(pair.second.getId());


    return entities;
}
*/


void EntityManager::eraseEntityByID(uint32_t id) {
    // TODO Modifie this to not add news component all times
    eraseComponent<SituationComponent>              (id); //1
    eraseComponent<ChestComponent>                  (id); //2
    eraseComponent<CollisionComponent>              (id); //3
    eraseComponent<BoundingComponent>               (id); //4
    eraseComponent<CoinDetectorComponent>           (id); //5
    eraseComponent<DashComponent>                   (id); //6
    eraseComponent<VelocityComponent>               (id); //7
    eraseComponent<StatsComponent>                  (id); //8
    eraseComponent<AIComponent>                     (id); //9
    eraseComponent<DrawableComponent>               (id); //10
    eraseComponent<AIMoveToComponent>               (id); //11
    eraseComponent<AICombatComponent>               (id); //12
    eraseComponent<AIMoveTo3DComponent>             (id); //13
    eraseComponent<AIStandByComponent>              (id); //14
    eraseComponent<AIOffensiveComponent>            (id); //15
    eraseComponent<AIDefensiveComponent>            (id); //16
    eraseComponent<WalletComponent>                 (id); //17
    eraseComponent<CoinComponent>                   (id); //18
    eraseComponent<SoundComponent>                  (id); //19
    eraseComponent<InputComponent>                  (id); //20
    eraseComponent<DirectorDataComponent>           (id); //21
    eraseComponent<AttackComponent>                 (id); //22
    eraseComponent<TerrainDataComponent>            (id); //23
    eraseComponent<InventoryComponent>              (id); //25
    eraseComponent<InteractionComponent>            (id); //26
    eraseComponent<HUDComponent>                    (id); //27
    eraseComponent<MeleeWeaponComponent>            (id); //28
    eraseComponent<DistanceWeaponComponent>         (id); //29
    eraseComponent<KeyComponent>                    (id); //30
    eraseComponent<ProximityComponent>              (id); //31
    eraseComponent<TerrainCubeComponent>            (id); //32
    eraseComponent<ObstacleComponent>               (id); //33
    eraseComponent<WallComponent>                   (id); //34
    eraseComponent<IPDataComponent>                 (id); //35
    eraseComponent<EffectWinHeartComponent>         (id); //36
    eraseComponent<EffectPoisonComponent>           (id); //36
    eraseComponent<EffectParalysisComponent>        (id); //37
    eraseComponent<EffectSlowdownComponent>         (id); //38
    eraseComponent<EffectMagnifyBulletComponent>    (id); //39
    eraseComponent<WaypointComponent>               (id); //40
    eraseComponent<EggDataComponent>                (id); //41
    eraseComponent<InfoBoxComponent>                (id); //42
    eraseComponent<MessageBoxComponent>             (id); //43
    eraseComponent<NotificationComponent>           (id); //44
    eraseComponent<CompassComponent>                (id); //45
    eraseComponent<EnemyComponent>                  (id); //46
    eraseComponent<Drawable2DComponent>             (id); //47
    eraseComponent<ObstacleDataComponent>           (id); //48
    eraseComponent<AIDifuseActionComponent>         (id); //49
    eraseComponent<AIDifuseStateComponent>          (id); //50
    eraseComponent<AIActionThreatenComponent>       (id); //51
    eraseComponent<AIActionMeleeComponent>          (id); //52
    eraseComponent<AIActionWanderComponent>         (id); //53
    eraseComponent<AIActionEscapeComponent>         (id); //54
    eraseComponent<AIActionLungeComponent>          (id); //55
    eraseComponent<AIActionKeepDistanceComponent>   (id); //56
    eraseComponent<AIActionDistanceComponent>       (id); //57
    eraseComponent<AIActionRegroupComponent>        (id); //58
    eraseComponent<LevelComponent>                  (id); //59
    eraseComponent<IPPartComponent>                 (id); //60
    eraseComponent<SecureZoneComponent>             (id); //61
    eraseComponent<DetectionSignComponent>          (id); //62
    eraseComponent<LevitationComponent>             (id); //63
    eraseComponent<LightComponent>                  (id); //64
    eraseComponent<IluminationComponent>            (id); //65

    entityMap           .erase(id);
}

std::unordered_map<uint32_t, Entity> &EntityManager::getEntities() {
    return entityMap;
}

std::vector<uint32_t> &EntityManager::getEntitiesToUpdate() {
    return entitiesToUpdate;
}

void EntityManager::addEntityToUpdate(const uint32_t id) {
    entitiesToUpdate.emplace_back(id);
}

void EntityManager::clearEntitiesToUpdate() {
    entitiesToUpdate.clear();
}

