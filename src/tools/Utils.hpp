#pragma once

#include <eng/GameEngine.hpp>


class Utils {
public:
    Utils();

    static void     activeInfoNotification(GameEngine &gameContext, size_t i);
    static void     resetInfoNotifications(GameEngine &gameContext);

    // Check if the player has the key complete
    static bool     isKeyComplete(GameEngine &gameContext);
    static bool     checkSituationIsOutsideLimits(SituationComponent &situation, float limitLeft, float limitRigth, float limitTop, float limitBottom);

    static uint8_t  calculateBiome(GameEngine &gameContext, const float posX, const float posZ) ;
    static int      getRoundedPositionX(const float posX, GameEngine &gameContext) ;
    static int      getRoundedPositionZ(const float posZ, GameEngine &gameContext) ;
    static bool     checkIfIsInTheChunk(SituationComponent &entSituation, GameEngine &gameContext);
    static void     calculateTerrainLimits(float &terrainPosW, float &terrainPosE, float &terrainPosN, float &terrainPosS, GameEngine &gameContext);
    static void     calculateVisibleZoneLimits(float &terrainPosW, float &terrainPosE, float &terrainPosN, float &terrainPosS, GameEngine &gameContext);

    static float    calculateDistance(const float posX1, const float posX2, const float posZ1, const float posZ2);
    static bool     isCollisionAB(SituationComponent &situationA, BoundingComponent &boundingA, SituationComponent &situationB, BoundingComponent &boundingB);
    static bool     isCollisionAB(float x1, float y1, float z1, float sizeX1, float sizeY1, float sizeZ1, SituationComponent &situation, BoundingComponent &bounding);

    template <typename T>
    static void     clearAllEntitiesColliding(Storage<T> &boundings) {
        for (auto &boundingA : boundings) {
            boundingA.entitiesColliding.clear();
        }
    }
    template <typename T>
    static void insertBoundingsIdInsideChunk(GameEngine &gameContext, Storage<T> &boundings, std::vector<uint32_t> &boundingsIdInsideChunk) {
        uint32_t idBoundEnt = 0;

        // Calculate limits of the terrain
        float terrainPosW = 0.f, terrainPosE = 0.f, terrainPosN = 0.f, terrainPosS = 0.f;
        Utils::calculateTerrainLimits(terrainPosW, terrainPosE, terrainPosN, terrainPosS, gameContext);

        for (auto &bounding : boundings) {
            idBoundEnt = bounding.id;
            SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(idBoundEnt);

            if(situation.x > terrainPosW && situation.x < terrainPosE
                    && situation.z < terrainPosN && situation.z > terrainPosS) {
                boundingsIdInsideChunk.emplace_back(idBoundEnt);
            }
        }
    }

    template <typename T>
    static uint16_t dropCoins(GameEngine &gameContext, uint32_t entityId, uint8_t spawnRange) {
        uint16_t dropedCoins = 0;
        bool isEntityHaveWallet = gameContext.entityMan.existsComponent<T>(entityId);
        if(isEntityHaveWallet) {

            T     &coinContainer = gameContext.entityMan.getComponent<T>(entityId);
            SituationComponent  &sitComp    = gameContext.entityMan.getComponent<SituationComponent>(entityId);

            //get entity coins
            uint16_t coins = coinContainer.coins;
            dropedCoins = coins;

            //for each coin
            while (coins > 0) {
                GameObjectType coinType = Utils::smartCoinChanger(coins);
                Utils::spawnCoinAtRandomPosition(gameContext, coinType, sitComp.x, sitComp.y, sitComp.z, spawnRange);
                coins -= (int)coinType;
            }
            coinContainer.coins = 0;
        }
        return dropedCoins;
    }


    static GameObjectType   smartCoinChanger(uint16_t coinValue);
    static uint32_t         spawnCoinAtRandomPosition(GameEngine &gameContext, GameObjectType coinType, float posX, float posY, float posZ, uint8_t spawnRange);
    static void             dropCoinWithTarget(GameEngine &gameContext, uint32_t entityId, uint32_t targetId);

    static float    angleToTarget(SituationComponent &entitySituation, SituationComponent &targetSituation);
    static float    degToRad(float angle);
    static float    radToDeg(float angle);

    //static bool     passedMaxTime(GameEngine &gameContext, std::chrono::time_point<std::chrono::system_clock> &clock, float &maxClockTime);

    static float    calculateAttackDamage(WeaponComponent &weapon, StatsComponent &playerStats, bool &isCritic);
    static uint32_t meleeAttack(GameEngine &gameContext, MeleeWeaponComponent &meleeWeapon, SituationComponent &playerSituation, StatsComponent &playerStats, uint32_t attackerId, float lifeTime = 0);
    static uint32_t distanceAttack(GameEngine &gameContext, DistanceWeaponComponent &distanceWeapon, SituationComponent &playerSituation, StatsComponent &playerStats, uint32_t attackerId);
    static void dropWeapon(GameEngine &gameContext, WeaponType weaponType, float x, float y, float z);

    static void dropKey(GameEngine &gameContext, uint8_t biome, float x, float y, float z);

    static bool isSmartCamera(InputComponent &playerInput);


    static std::string getBiomeTexturePath(uint8_t biome, uint8_t pn);
private:
    static bool     checkIsCoordFree(float x1, float size1, float x2, float size2);
    static float    adjustAngleToAxes(float angle, float x, float z);
};