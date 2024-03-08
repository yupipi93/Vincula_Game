#include "DeathSystem.hpp"

#include <tools/Utils.hpp>
//console.log

#include <eng/GameEngine.hpp>

//#####################################################
//                == DEATH SYSTEM ==
//  - Get all death entities
//  - Execute action post mortem (Drop Coins, ...)
//  - Delete entities from the game (except player)
//
//   Using Componets:
//      - Stats     (Read)
//      - Wallets   (Read)
//      - Situation (Read)
//#####################################################

constexpr uint8_t enemySpawnRange   = 100;
constexpr uint8_t playerSpawnRange  = 150;
constexpr uint8_t eggSpawnRange     = 150;

DeathSystem::DeathSystem() {}

DeathSystem::~DeathSystem() {
}


void DeathSystem::update(GameEngine &gameContext) const {
    uint32_t    playerId    = gameContext.playerId;
    std::vector<uint32_t>   deathEntities;
    deathEntities = getDeathEntities(gameContext);
    auto &enemiesSpawned = gameContext.entityMan.getComponent<DirectorDataComponent>(gameContext.aIDirectorId).enemiesSpawned;

    for (uint32_t entityId : deathEntities) {
        actionPostMortem(gameContext, entityId, enemiesSpawned);
        EntityType entityTypeToDeath = gameContext.entityMan.getEntity(entityId).getType();
        if(entityTypeToDeath != PLAYER && entityTypeToDeath != KEYEGG) {
            deleteEntity(gameContext, entityId);
        }
    }
}

std::vector<uint32_t> DeathSystem::getDeathEntities(GameEngine &gameContext) const {
    auto        &allStats   = gameContext.entityMan.getComponents<StatsComponent>();

    std::vector<uint32_t> deathEntities;
    deathEntities.reserve(allStats.size());

    for (auto &statsPair : allStats) {
        auto entityId       {statsPair.id};
        auto &statsComp     {statsPair};

        if (statsComp.currentHealth == 0) {
            deathEntities.emplace_back(entityId);
        }
    }
    return deathEntities;
}


void DeathSystem::actionPostMortem(GameEngine &gameContext, uint32_t entityId, std::vector<uint8_t> &enemiesSpawned) const {

    //NEED RESTRUCTURE TO REMOVE SWITCH
    Entity &entity {gameContext.getEntity(entityId)};
    auto entityType = entity.getType();
    switch(entityType) {
    case ENEMY:
        Utils::dropCoins<WalletComponent>(gameContext, entityId, enemySpawnRange);
        gameContext.getSoundFacadeRef().setParameterEventByID(entityId, DEAD_SOUND);

        switch (entity.getGameObjectType()) {
        case ENEMY_PIG:
        case ENEMY_RODENT:
            enemiesSpawned[1]--;
            break;
        case ENEMY_PINEAPPLE:
            enemiesSpawned[2]--;
            break;
        case ENEMY_SPIDER:
            enemiesSpawned[3]--;
            break;
        case ENEMY_MONKEY:
            enemiesSpawned[4]--;
            break;
        case ENEMY_GHOST:
            enemiesSpawned[5]--;
            break;
        }

        break;
    case PLAYER:
        playerActionPostMortem(gameContext, entityId);
        break;
    case KEYEGG: {
        auto &playerMoveTo = gameContext.entityMan.getComponent<AIMoveToComponent>(gameContext.playerId);
        if (playerMoveTo.targetId == entityId)
            playerMoveTo.targetId = 0;
        eggActionPostMortem(gameContext, entityId);
        break;
    }
    }

    if (entityType == ENEMY)
        ++gameContext.enemiesDeaths;
}

void DeathSystem::playerActionPostMortem(GameEngine &gameContext, uint32_t playerId) const {
    DrawableComponent &playerDrawable = gameContext.entityMan.getComponent<DrawableComponent>(playerId);
    playerDrawable.model = "./media/player/Animations/death/death";
    gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
    gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    gameContext.dropedCoins = Utils::dropCoins<WalletComponent>(gameContext, playerId, playerSpawnRange);
}

void DeathSystem::eggActionPostMortem(GameEngine &gameContext, uint32_t eggId) const {
    EggDataComponent    &eggData      = gameContext.entityMan.getComponent<EggDataComponent>(eggId);
    if(!eggData.broken) {
        SituationComponent  &eggSituation = gameContext.entityMan.getComponent<SituationComponent>(eggId);
        DrawableComponent   &eggDrawable  = gameContext.entityMan.getComponent<DrawableComponent>(eggId);
        StatsComponent      &eggStats     = gameContext.entityMan.getComponent<StatsComponent>(eggId);

        eggDrawable.model = eggData.modelEggBroken;
        gameContext.getWindowFacadeRef().updateModel(gameContext, eggId);

        eggStats.resetHealth();

        Utils::dropKey(gameContext, eggData.biome, eggSituation.x, eggSituation.y + 50, eggSituation.z);
        Utils::dropCoins<WalletComponent>(gameContext, eggId, eggSpawnRange);

        eggData.broken = true;
    }
}

void DeathSystem::deleteEntity(GameEngine &gameContext, uint32_t entityId) const {
    gameContext.eraseEntityByID(entityId);
}







