#pragma once

#include <unordered_map>
class GameEngine;

#include <tools/Storage.hpp>
#include <com/ComponentsIncluder>
#include <ent/Entity.hpp>
#include <enum/GameObjectType.hpp>


class EntityManager {
public:

    //////////////////////////////////////////////////
    //              THE RULE OF THE 3               //
    //////////////////////////////////////////////////

    explicit EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager &em) = delete;

    //////////////////////////////////////////////////
    //                  GET COMPONENT               //
    //////////////////////////////////////////////////

    template <typename T>
    T  &getComponent(const uint32_t id) {
        auto &storage = getComponents<T>();
        return storage[storage[id]];
        //return storage[storage[id]];
    }

    template <typename T>
    Storage<T>  &getComponents() {
        static Storage<T> storage{MAX_ENTITIES};
        return storage;
    }

    template <typename T>
    bool existsComponent(const uint32_t id) {
        auto &storage = getComponents<T>();
        return storage.exists(id);
    }


    //////////////////////////////////////////////////
    //                CREATE COMPONENT              //
    //////////////////////////////////////////////////

    template <typename T>
    T &createComponent(const uint32_t id) {
        auto &storage = getComponents<T>();
        storage.emplace(id);
        return storage[storage[id]];
        //return storage[storage[id]];
    }


    //////////////////////////////////////////////////
    //                ERASE COMPONENT               //
    //////////////////////////////////////////////////

    template <typename T>
    void eraseComponent(const uint32_t id) {
        auto &storage = getComponents<T>();
        storage.erase(id);
    }

    //////////////////////////////////////////////////
    //                ENTITY CREATION               //
    //////////////////////////////////////////////////

    //uint32_t createEntity           (GameEngine &gameContext, EntityType eType, float x, float y, float z, GameObjectType goType = NONE);

    uint32_t createVillageObstacle  (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createMap              (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createPlayer           (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createEnemy            (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createCamera           (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createSource           (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createBank             (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createBankDepositor    (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createBankExtractor    (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createWall             (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createWallCollidable   (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createDoor             (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createAIDirector       (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createCoin             (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createChest            (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createObstacle         (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createObstacleData     (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createTerrain          (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createTerrainCube      (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createInterestPoint    (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createInterestPointPart(GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createSecureZone       (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createKey              (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createKeyEgg           (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createWaypoint         (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createInfoBox          (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createMessageBox       (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createNotification     (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createDetectionSign    (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createControlsFoor     (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createCompass          (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createAttack           (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createMeleeWeapon      (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createDistanceWeapon   (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createHUD              (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createNPC              (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createSoundEvent       (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createLight            (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t mouseShape             (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);
    uint32_t createPalantir         (GameEngine &gameContext, float x, float y, float z, float r = 0, GameObjectType goType = NONE);


    //////////////////////////////////////////////////
    //                GET ENTITY                    //
    //////////////////////////////////////////////////
    Entity &getEntity(uint32_t id);
    /* Tonto estabas buscando esto? que no hay que usarlo, pringao
    std::vector<uint32_t> getEntitiesIdByType(EntityType type);
    */
    std::unordered_map<uint32_t, Entity> &getEntities();


    //////////////////////////////////////////////////
    //                ERASE ENTITY                  //
    //////////////////////////////////////////////////
    void eraseEntityByID                (uint32_t id);


    //////////////////////////////////////////////////
    //                UPDATE ENTITY                 //
    //////////////////////////////////////////////////
    std::vector<uint32_t>   &getEntitiesToUpdate();
    void addEntityToUpdate(const uint32_t id);
    void clearEntitiesToUpdate();

private:

    std::unordered_map<uint32_t, Entity> entityMap;
    std::vector<uint32_t> entitiesToUpdate;
    const uint32_t MAX_ENTITIES = 9092;

};
