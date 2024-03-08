#include "Utils.hpp"

Utils::Utils() {}

void Utils::resetInfoNotifications(GameEngine &gameContext) {
    for (int i = 0; i < gameContext.activeInfoNotifications.size(); i++) {
        gameContext.activeInfoNotifications[i] = false;
    }

    gameContext.notificationTime = 3.f;
}

void Utils::activeInfoNotification(GameEngine &gameContext, size_t i) {
    resetInfoNotifications(gameContext);
    gameContext.activeInfoNotifications[i] = true;
}

// Check if the player has the key complete
bool Utils::isKeyComplete(GameEngine &gameContext) {
    auto &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(gameContext.playerId);
    if (playerInventory.keysCollected.size() == playerInventory.MAX_KEYS)
        return true;
    return false;
}

bool Utils::checkSituationIsOutsideLimits(SituationComponent &situation, float limitLeft, float limitRigth, float limitTop, float limitBottom) {
    return (situation.x <= limitLeft ||
            situation.x >= limitRigth ||
            situation.z >= limitTop ||
            situation.z <= limitBottom);
}

uint8_t Utils::calculateBiome(GameEngine &gameContext, const float posX, const float posZ) {
    uint32_t terrainGeneratorId = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainGeneratorId);
    float distanceToCenter = sqrt(posX * posX + posZ * posZ);

    if (distanceToCenter <= terrainData.baseSize) {
        return 0;
    } else {
        distanceToCenter -= terrainData.baseSize;
        uint16_t biomeSize = terrainData.biomeSize;
        distanceToCenter += biomeSize;
        return distanceToCenter / biomeSize;
    }
}

int Utils::getRoundedPositionX(const float posX, GameEngine &gameContext) {
    uint32_t             terrainId    = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    int roundedPosition = 0;

    roundedPosition = posX / terrainData.cubeSize ;

    return roundedPosition;
}


int Utils::getRoundedPositionZ(const float posZ, GameEngine &gameContext) {
    uint32_t             terrainId    = gameContext.terrainId;
    TerrainDataComponent &terrainData = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    int roundedPosition = 0;

    roundedPosition = posZ / terrainData.cubeSize;

    return roundedPosition;
}

bool Utils::checkIfIsInTheChunk(SituationComponent &entSituation, GameEngine &gameContext) {
    float terrainPosW = 0.f, terrainPosE = 0.f, terrainPosN = 0.f, terrainPosS = 0.f;
    calculateTerrainLimits(terrainPosW, terrainPosE, terrainPosN, terrainPosS, gameContext);

    return (entSituation.x > terrainPosW && entSituation.x < terrainPosE
            && entSituation.z < terrainPosN && entSituation.z > terrainPosS);
}

void Utils::calculateTerrainLimits(float &terrainPosW, float &terrainPosE, float &terrainPosN, float &terrainPosS, GameEngine &gameContext) {
    uint32_t             terrainId         = gameContext.terrainId;
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);

    terrainPosW = terrainSituation.x - (terrainData.chunkWidthIncrement * terrainData.cubeSize);
    terrainPosE = terrainSituation.x + (terrainData.terrainWidth - 1 + terrainData.chunkWidthIncrement) * terrainData.cubeSize;
    terrainPosN = terrainSituation.z + (terrainData.chunkHeightIncrement * terrainData.cubeSize);
    terrainPosS = terrainSituation.z - (terrainData.terrainHeight + terrainData.chunkHeightIncrement) * terrainData.cubeSize;
}

void Utils::calculateVisibleZoneLimits(float &terrainPosW, float &terrainPosE, float &terrainPosN, float &terrainPosS, GameEngine &gameContext) {
    uint32_t             terrainId         = gameContext.terrainId;
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);

    terrainPosW = terrainSituation.x;
    terrainPosE = terrainSituation.x + (terrainData.terrainWidth - 1) * terrainData.cubeSize;
    terrainPosN = terrainSituation.z;
    terrainPosS = terrainSituation.z - terrainData.terrainHeight * terrainData.cubeSize;
}


float Utils::calculateDistance(const float posX1, const float posX2, const float posZ1, const float posZ2) {
    float distance = sqrt((posX1 - posX2) * (posX1 - posX2) + (posZ1 - posZ2) * (posZ1 - posZ2));
    return distance;
}

bool Utils::isCollisionAB(SituationComponent &situationA, BoundingComponent &boundingA, SituationComponent &situationB, BoundingComponent &boundingB) {
    float x1      = situationA.x;
    float y1      = situationA.y;
    float z1      = situationA.z;
    float sizeX1   = boundingA.sizeX;
    float sizeY1   = boundingA.sizeY;
    float sizeZ1   = boundingA.sizeZ;

    float x2      = situationB.x;
    float y2      = situationB.y;
    float z2      = situationB.z;
    float sizeX2   = boundingB.sizeX;
    float sizeY2   = boundingB.sizeY;
    float sizeZ2   = boundingB.sizeZ;

    bool isXFree = checkIsCoordFree(x1, sizeX1, x2, sizeX2);
    bool isYFree = checkIsCoordFree(y1, sizeY1, y2, sizeY2);
    bool isZFree = checkIsCoordFree(z1, sizeZ1, z2, sizeZ2);

    bool colliding = !(isXFree || isYFree || isZFree);

    return colliding;
}

bool Utils::isCollisionAB(float x1, float y1, float z1, float sizeX1, float sizeY1, float sizeZ1, SituationComponent &situation, BoundingComponent &bounding) {
    float x2      = situation.x;
    float y2      = situation.y;
    float z2      = situation.z;
    float sizeX2   = bounding.sizeX;
    float sizeY2   = bounding.sizeY;
    float sizeZ2   = bounding.sizeZ;

    bool isXFree = checkIsCoordFree(x1, sizeX1, x2, sizeX2);
    bool isYFree = checkIsCoordFree(y1, sizeY1, y2, sizeY2);
    bool isZFree = checkIsCoordFree(z1, sizeZ1, z2, sizeZ2);

    bool colliding = !(isXFree || isYFree || isZFree);

    return colliding;
}

bool Utils::checkIsCoordFree(float x1, float size1, float x2, float size2) {
    float measure1 = size1 * 0.5;
    float measure2 = size2 * 0.5;

    //IF X1-W1 is greather that X2+W2 is OK
    bool checkIs_Left   = (x1 - measure1 >= x2 + measure2);

    //IF X1+W1 is losser that X2-W2 is OK
    bool checkIs_Right = (x1 + measure1 <= x2 - measure2);

    //Only return true if both are true
    return (checkIs_Left || checkIs_Right);
}


GameObjectType Utils::smartCoinChanger(uint16_t coinValue) {
    if(coinValue >= 10000)
        return COIN_500;

    if(coinValue >= 500)
        return COIN_200;

    if(coinValue >= 300)
        return COIN_100;

    if(coinValue >= 150)
        return COIN_50;

    if(coinValue >= 50)
        return COIN_20;

    if(coinValue >= 15)
        return COIN_5;

    return COIN_1;
}


uint32_t Utils::spawnCoinAtRandomPosition(GameEngine &gameContext, GameObjectType coinType, float posX, float posY, float posZ, uint8_t spawnRange) {
    //set spawn getPosition
    posX = posX + (rand() % spawnRange) - spawnRange / 2;
    posZ = posZ + (rand() % spawnRange) - spawnRange / 2;
    //create Coin
    return gameContext.entityMan.createCoin(gameContext, posX, posY, posZ, 0.f, coinType);
}

void Utils::dropCoinWithTarget(GameEngine &gameContext, uint32_t entityId, uint32_t targetId) {
    auto  &entitySituation  = gameContext.entityMan.getComponent<SituationComponent>(entityId);
    auto  &entityCollider   = gameContext.entityMan.getComponent<CollisionComponent>(entityId);
    auto  &entityWallet   = gameContext.entityMan.getComponent<WalletComponent>(entityId);

    GameObjectType coinType = smartCoinChanger(entityWallet.coins);

    //get entity position
    float posX = entitySituation.x;
    float posY = entityCollider.sizeY * 2;
    float posZ = entitySituation.z;



    //Spawn Coin
    uint32_t coinId     = gameContext.entityMan.createCoin(gameContext, posX, posY, posZ, 0.f, coinType);
    gameContext.entityMan.eraseComponent<AIMoveToComponent>(coinId);
    gameContext.entityMan.createComponent<AIMoveTo3DComponent>(coinId);

    auto &CoinComp      = gameContext.entityMan.getComponent<CoinComponent>(coinId);
    CoinComp.owner      = entityId;
    auto &moveTo3DComp  = gameContext.entityMan.getComponent<AIMoveTo3DComponent>(coinId);
    moveTo3DComp.targetId = targetId;

    entityWallet.coins -= (int)coinType;
}

float Utils::angleToTarget(SituationComponent &entitySituation, SituationComponent &targetSituation) {
    float angle {0};
    float   x  {entitySituation.x - targetSituation.x};
    float   z  {entitySituation.z - targetSituation.z};

    angle = radToDeg(atan(z / x));

    angle = adjustAngleToAxes(angle, x, z);

    angle = degToRad(angle);

    return angle;

}

float Utils::adjustAngleToAxes(float angle, float x, float z) {
    if(x > 0) {
        angle += 180;
    } else if(z > 0) {
        angle += 360;
    }
    return angle;
}

float Utils::degToRad(float angle) {
    float ret = 0;
    ret = angle / 180 * 3.14159;
    return ret;
}

float Utils::radToDeg(float angle) {
    float ret = 0;
    ret = angle * 180 / 3.14159;
    return ret;
}

float Utils::calculateAttackDamage(WeaponComponent &weapon, StatsComponent &playerStats, bool &isCritic) {
    uint32_t finalDamage = 1;

    float wDamage        = weapon.damage;
    uint8_t wDamageRange = weapon.damageRange;
    float wCriticPer     = weapon.criticalPercent;
    uint8_t pDamage      = playerStats.damage;

    //FinalDamage = (WeaponDamage+-RangeDamage) + PlayerDamage
    finalDamage += pDamage;
    finalDamage *= wDamage;
    finalDamage += (rand() % wDamageRange) / 100 * finalDamage;

    //Calculate critic probability
    uint8_t cp = rand() % 100;
    if(cp <= wCriticPer) {
        //If critic: damage*2
        isCritic = true;
        finalDamage *= 2;
    }

    return finalDamage;
}

uint32_t Utils::distanceAttack(GameEngine &gameContext, DistanceWeaponComponent &distanceWeapon, SituationComponent &playerSituation, StatsComponent &playerStats, uint32_t attackerId) {

    uint32_t idAttackRet {0};

    EffectType effect         = distanceWeapon.effect;
    CollisionType colType     = distanceWeapon.collisionType;
    bool isCritic             = false;
    float finalDamage         = Utils::calculateAttackDamage(distanceWeapon, playerStats, isCritic);
    float rot                 = playerSituation.rotation;
    float auxRot              = degToRad(rot);

    float attackSize          = distanceWeapon.attackSize;
    float sizeSepAttac        = distanceWeapon.sizeSeparationAttacks;
    uint8_t posOrigin         = distanceWeapon.posOrigin;
    uint8_t numAttacks        = distanceWeapon.numAttacks;

    uint8_t biome             = distanceWeapon.biome;
    bool rotative             = distanceWeapon.rotative;

    float bulletSpeed         = distanceWeapon.bulletSpeed;
    float bulletLifeTime      = distanceWeapon.bulletLifeTime;
    std::string bulletModel   = distanceWeapon.bulletModel;
    std::string bulletEfeModel = distanceWeapon.bulletEffectModel;

    float vx                  = bulletSpeed * cos(auxRot);
    float vz                  = bulletSpeed * sin(auxRot);



    for(uint8_t i = 0; i < numAttacks; i++) {
        float posX         = playerSituation.x + (posOrigin + (i * (attackSize + sizeSepAttac))) * cos(auxRot);
        float posY         = playerSituation.y;
        float posZ         = playerSituation.z + (posOrigin + (i * (attackSize + sizeSepAttac))) * sin(auxRot);

        uint32_t idAttack = gameContext.entityMan.createAttack(gameContext, posX, posY, posZ, rot, ATTACK_DISTANCE);

        idAttackRet = idAttack;

        DrawableComponent   &drawableComp   = gameContext.entityMan.getComponent<DrawableComponent>(idAttack);
        drawableComp.model      = bulletModel;

        CollisionComponent  &collidableComp = gameContext.entityMan.getComponent<CollisionComponent>(idAttack);
        collidableComp.sizeX = attackSize;
        collidableComp.sizeY = attackSize;
        collidableComp.sizeZ = attackSize;
        collidableComp.type  = colType;

        VelocityComponent   &velocityComp   = gameContext.entityMan.getComponent<VelocityComponent>(idAttack);
        velocityComp.speed     = bulletSpeed;
        velocityComp.velocityX = vx;
        velocityComp.velocityY = 0;
        velocityComp.velocityZ = vz;

        AttackComponent     &attackComp     = gameContext.entityMan.getComponent<AttackComponent>(idAttack);
        attackComp.attackerId       = attackerId;
        attackComp.damage           = finalDamage;
        attackComp.lifeTime         = bulletLifeTime;
        attackComp.biome            = biome;
        attackComp.rotative         = rotative;
        attackComp.isCritic         = isCritic;

        uint8_t ep = rand() % 100 + 1;
        if(ep <= distanceWeapon.effectPercent) {
            attackComp.effectType   = effect;
            drawableComp.model      = bulletEfeModel;

            if(distanceWeapon.effect == MAGNIFYBULLET) {
                gameContext.entityMan.createComponent<EffectMagnifyBulletComponent>(idAttack);
            } else if(distanceWeapon.effect == SMARTBULLET) {
                //Need to be smart (sensor, aiMoveTo, proximity)
                //gameContext.createSensor(attackId);
                gameContext.entityMan.createComponent<BoundingComponent>(idAttack);
                auto &sensorComp = gameContext.entityMan.getComponent<BoundingComponent>(idAttack);
                sensorComp.sizeX = 100;
                sensorComp.sizeY = 0;
                sensorComp.sizeZ = 100;
                auto &proximity = gameContext.entityMan.createComponent<ProximityComponent>(idAttack);
                proximity.attractiveTo = gameContext.entityMan.getComponent<AIComponent>(attackerId).hostileTo;

                gameContext.entityMan.createComponent<AIMoveToComponent>(idAttack);
            }
        }
        gameContext.getWindowFacadeRef().eraseEntity(idAttack);
        gameContext.getWindowFacadeRef().createEntity(gameContext, idAttack);
        gameContext.getWindowFacadeRef().updateEntities(gameContext, {idAttack});
    }

    //######### SOUND ########//
    if(distanceWeapon.eventName != "0") {
        SoundComponent  &soundComp  {gameContext.entityMan.createComponent<SoundComponent>(idAttackRet)};
        soundComp.eventName = distanceWeapon.eventName;
        gameContext.getSoundFacadeRef().playEvent(gameContext, idAttackRet);
    }


    return idAttackRet;
}

uint32_t Utils::meleeAttack(GameEngine &gameContext, MeleeWeaponComponent &meleeWeapon, SituationComponent &playerSituation, StatsComponent &playerStats, uint32_t attackerId, float lifeTime) {
    uint32_t idAttackRet {0};

    EffectType effect  = meleeWeapon.effect;
    bool isCritic             = false;
    float finalDamage  = Utils::calculateAttackDamage(meleeWeapon, playerStats, isCritic);
    float rot          = playerSituation.rotation;
    float rotRad       = degToRad(rot);


    float attackSize   = meleeWeapon.attackSize;
    float sizeSepAttac = meleeWeapon.sizeSeparationAttacks;
    uint8_t posOrigin  = meleeWeapon.posOrigin;
    uint8_t numAttacks = meleeWeapon.numAttacks;
    uint8_t biome      = meleeWeapon.biome;

    uint8_t ep = rand() % 100 + 1;

    for(uint8_t i = 0; i < numAttacks; i++) {
        float posX         = playerSituation.x + (posOrigin + (i * (attackSize + sizeSepAttac))) * cos(rotRad);
        float posY         = playerSituation.y;
        float posZ         = playerSituation.z + (posOrigin + (i * (attackSize + sizeSepAttac))) * sin(rotRad);

        uint32_t idAttack = gameContext.entityMan.createAttack(gameContext, posX, posY, posZ, rot, ATTACK_MELEE);

        idAttackRet = idAttack;

        DrawableComponent   &drawableComp   = gameContext.entityMan.getComponent<DrawableComponent>(idAttack);
        drawableComp.model      = "";
        drawableComp.texture    = "./media/weapons/sword2.png";
        drawableComp.scaleX     = attackSize / 10;
        drawableComp.scaleY     = attackSize / 10;
        drawableComp.scaleZ     = attackSize / 10;

        CollisionComponent  &collidableComp = gameContext.entityMan.getComponent<CollisionComponent>(idAttack);
        collidableComp.sizeX = attackSize;
        collidableComp.sizeY = attackSize;
        collidableComp.sizeZ = attackSize;
        collidableComp.type  = KINEMATICMELEE;

        VelocityComponent   &velocityComp   = gameContext.entityMan.getComponent<VelocityComponent>(idAttack);
        velocityComp.speed = 0;

        AttackComponent     &attackComp     = gameContext.entityMan.getComponent<AttackComponent>(idAttack);
        attackComp.attackerId     = attackerId;
        attackComp.damage           = finalDamage;
        attackComp.lifeTime         = lifeTime;
        attackComp.biome            = biome;
        attackComp.isCritic         = isCritic;


        if(ep <= meleeWeapon.effectPercent) {
            attackComp.effectType = effect;
        }

        gameContext.getWindowFacadeRef().updateTextures(gameContext, {idAttack});
        gameContext.getWindowFacadeRef().updateEntities(gameContext, {idAttack});
    }



    //######### SOUND ########//
    if(meleeWeapon.eventName != "0") {
        SoundComponent  &soundComp  {gameContext.entityMan.createComponent<SoundComponent>(idAttackRet)};
        soundComp.eventName = meleeWeapon.eventName;
        gameContext.getSoundFacadeRef().playEvent(gameContext, idAttackRet);
    }

    return idAttackRet;
}

void Utils::dropWeapon(GameEngine &gameContext, WeaponType weaponType, float x, float y, float z) {

    switch(weaponType) {
    //##MELEE
    case WeaponType::SWORD:
        gameContext.entityMan.createMeleeWeapon(gameContext, x, y, z, 0.f, MELEE_SWORD);
        break;

    case WeaponType::MACE:
        gameContext.entityMan.createMeleeWeapon(gameContext, x, y, z, 0.f, MELEE_MACE);
        break;

    case WeaponType::STICK:
        gameContext.entityMan.createMeleeWeapon(gameContext, x, y, z, 0.f, MELEE_STICK);
        break;

    //##DISTANCE
    case WeaponType::BLOWGUN:
        gameContext.entityMan.createDistanceWeapon(gameContext, x, y, z, 0.f, DISTANCE_BLOWGUN);
        break;

    case WeaponType::SLINGSHOT:
        gameContext.entityMan.createDistanceWeapon(gameContext, x, y, z, 0.f, DISTANCE_SLINGSHOT);
        break;

    case WeaponType::BOW:
        gameContext.entityMan.createDistanceWeapon(gameContext, x, y, z, 0.f, DISTANCE_BOW);
        break;
    }
}

void Utils::dropKey(GameEngine &gameContext, uint8_t biome, float x, float y, float z) {

    if(biome == 1) {
        gameContext.entityMan.createKey(gameContext, x, y, z, 0.f, KEY_BIOME1);
    } else if(biome == 2) {
        gameContext.entityMan.createKey(gameContext, x, y, z, 0.f, KEY_BIOME2);
    } else if(biome == 3) {
        gameContext.entityMan.createKey(gameContext, x, y, z, 0.f, KEY_BIOME3);
    } else if(biome == 4) {
        gameContext.entityMan.createKey(gameContext, x, y, z, 0.f, KEY_BIOME4);
    } else if(biome == 5) {
        gameContext.entityMan.createKey(gameContext, x, y, z, 0.f, KEY_BIOME5);
    }
}



bool Utils::isSmartCamera(InputComponent &playerInput) {
    if(playerInput.smartCamera || playerInput.zenithCamera || playerInput.zenithUpCamera || playerInput.airCamera) {
        return true;
    }
    return false;
}


std::string Utils::getBiomeTexturePath(uint8_t biome, uint8_t pn) {
    return "./media/biomes/terrain" + std::to_string(biome) + "p" + std::to_string(pn) + ".jpg";

}