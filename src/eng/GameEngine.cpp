#include "GameEngine.hpp"

#include <typeinfo>
#include <random>
#include <time.h>
#include <cstdint>


#include <sys/SystemsIncluder>

const float DELTA_TO_UPDATE = 1. / 60.;

GameEngine::GameEngine(bool debugMode, int width, int height, bool windowsMode)
    : sysRender(), windowFacade(width, height, windowsMode), soundFacade() {
    debuging = debugMode;
    screenWidth = width;
    screenHeight = height;


    srand(time(NULL)); // initialize the random seed
    //init();
    systems.reserve(MAX_SYSTEMS);
    timeInBiomes.resize(6);
    desactiveInfoActions.resize(7);
    activeInfoNotifications.resize(4);
}

GameEngine::~GameEngine() {}

void GameEngine::reset() {
    auto &allEnemies    {entityMan.getComponents<AIComponent>()};
    auto &allObstacles  {entityMan.getComponents<ObstacleComponent>()};
    auto &allCoins      {entityMan.getComponents<CoinComponent>()};
    auto &allAttacks    {entityMan.getComponents<AttackComponent>()};
    auto &dashComp      {entityMan.getComponent<DashComponent>(playerId)};
    auto &playerVelo    {entityMan.getComponent<VelocityComponent>(playerId)};
    auto &playerSitu    {entityMan.getComponent<SituationComponent>(playerId)};
    auto &playerWallet  {entityMan.getComponent<WalletComponent>(playerId)};
    auto &playerStats   {entityMan.getComponent<StatsComponent>(playerId)};
    auto &playerinput   {entityMan.getComponent<InputComponent>(playerId)};
    auto &aiDirector    {entityMan.getComponent<DirectorDataComponent>(aIDirectorId)};

    // reset num enemies spawned
    aiDirector.resetEnemies();


    //Delete enemies
    for(auto &e : allEnemies) {
        eraseEntityByID(e.id);
    }
    //Delete obstacles
    for(auto &e : allObstacles) {
        eraseEntityByID(e.id);
    }
    //Delete coins
    for(auto &c : allCoins) {
        eraseEntityByID(c.id);
    }
    //Delete attacks
    for(auto &e : allAttacks) {
        eraseEntityByID(e.id);
    }

    // Clear Player effects
    entityMan.eraseComponent<EffectPoisonComponent>(playerId);
    if (entityMan.existsComponent<EffectParalysisComponent>(playerId)) {
        auto &paralyEffe    {entityMan.getComponent<EffectParalysisComponent>(playerId)};
        paralyEffe.paralysisTime = 0;
    }

    //Clear Player wallet
    playerWallet.coins = 0;
    //Set Player at 0,0
    playerSitu.x = 0; //240
    playerSitu.y = 0;
    playerSitu.z = 250; //250
    getWindowFacadeRef().updateEntities(*this, {playerId});
    //Stop Player movement
    playerVelo.velocityX = 0;
    playerVelo.velocityY = 0;
    playerVelo.velocityZ = 0;
    dashComp.deactivateDash();

    //Reset full health
    playerStats.resetHealth();

    //Reset inputs
    playerinput.resetActions();


    //Reset terrain
    sysStaticEntities.resetTerrain(*this);

    //ResetGame
    systems.clear();
    init();

}

void GameEngine::init() {
    setGameState(PLAYING);

    //Attack must be the first because on it collisioned attacks are deleted
    systems.emplace_back(std::make_unique<TimeSystem>());                   //#00
    systems.emplace_back(std::make_unique<AttackSystem>());                 //#01
    systems.emplace_back(std::make_unique<InputSystem>());                  //#02
    systems.emplace_back(std::make_unique<InteractionSystem>());            //#03
    systems.emplace_back(std::make_unique<InventorySystem>());              //#04
    systems.emplace_back(std::make_unique<AIMoveToSystem>());               //#05
    systems.emplace_back(std::make_unique<AIStateMachineSystem>());         //#06
    systems.emplace_back(std::make_unique<AICombatStateSystem>());          //#07
    systems.emplace_back(std::make_unique<AIOffensiveStateSystem>());       //#08
    systems.emplace_back(std::make_unique<AIDefensiveStateSystem>());       //#09
    systems.emplace_back(std::make_unique<AIStandByStateSystem>());         //#10
    systems.emplace_back(std::make_unique<AIActionThreatenSystem>());       //#11
    systems.emplace_back(std::make_unique<AIActionMeleSystem>());           //#12
    systems.emplace_back(std::make_unique<AIActionDistanceSystem>());       //#13
    systems.emplace_back(std::make_unique<AIActionWanderSystem>());         //#14
    systems.emplace_back(std::make_unique<AIActionEscapeSystem>());         //#15
    systems.emplace_back(std::make_unique<AIActionLungeSystem>());          //#16
    systems.emplace_back(std::make_unique<AIActionKeepDistanceSystem>());   //#17
    systems.emplace_back(std::make_unique<AIActionRegroupSystem>());        //#18
    systems.emplace_back(std::make_unique<EffectsSystem>());                //#19
    systems.emplace_back(std::make_unique<PhysicsSystem>());                //#20
    systems.emplace_back(std::make_unique<SoundSystem>());                  //#21
    systems.emplace_back(std::make_unique<CollisionSystem>());              //#22 High consume*****
    systems.emplace_back(std::make_unique<SensorSystem>());                 //#23 High consume***
    systems.emplace_back(std::make_unique<CoinDetectorSystem>());           //#24
    systems.emplace_back(std::make_unique<AnimationSystem>());              //#25
    systems.emplace_back(std::make_unique<SolidSystem>());                  //#26
    systems.emplace_back(std::make_unique<CompassSystem>());                //#27
    systems.emplace_back(std::make_unique<WalletSystem>());                 //#28
    systems.emplace_back(std::make_unique<ChestSystem>());                  //#29
    systems.emplace_back(std::make_unique<BoxesSystem>());                  //#30
    systems.emplace_back(std::make_unique<DetectionSignSystem>());          //#31
    systems.emplace_back(std::make_unique<LevitationSystem>());             //#32
    systems.emplace_back(std::make_unique<DialogueSystem>());               //#33
    systems.emplace_back(std::make_unique<ProximitySystem>());              //#34
    systems.emplace_back(std::make_unique<StatsSystem>());                  //#35
    systems.emplace_back(std::make_unique<BaseSystem>());                   //#36
    systems.emplace_back(std::make_unique<CameraSystem>());                 //#37
    systems.emplace_back(std::make_unique<TerrainSystem>());                //#38
    systems.emplace_back(std::make_unique<ObstacleSystem>());               //#39
    systems.emplace_back(std::make_unique<SecureSystem>())  ;               //#40
    systems.emplace_back(std::make_unique<AIDirectorSystem>(*this));        //#41
    systems.emplace_back(std::make_unique<ChangeBiomeSystem>());            //#42
    systems.emplace_back(std::make_unique<InfoActionSystem>());             //#43
    systems.emplace_back(std::make_unique<InfoNotificationSystem>());       //#44
    systems.emplace_back(std::make_unique<TutorialSystem>());               //#45
    systems.emplace_back(std::make_unique<DeathSystem>());                  //#46
    systems.emplace_back(std::make_unique<GenerateZoneSystem>());           //#47
    systems.emplace_back(std::make_unique<VisibleSystem>());                //#48 High consume*
    systems.emplace_back(std::make_unique<WaypointSystem>());               //#49
    systems.emplace_back(std::make_unique<MapSystem>());                    //#50
    systems.emplace_back(std::make_unique<LightSystem>());                  //#51
    systems.emplace_back(std::make_unique<MouseLightSystem>());             //#52
    systems.emplace_back(std::make_unique<EffectsColorSystem>());           //#53
    systems.emplace_back(std::make_unique<FinalStatisticsSystem>());        //#54

    // StateSystem allways must be the last one!
    systems.emplace_back(std::make_unique<StateSystem>());                  //#55
}

void GameEngine::run() {

    //Call to initializate the static entities (wall, door...)
    sysStaticEntities.init(*this);

    // For the interpolation
    std::chrono::time_point<std::chrono::system_clock> then;
    then = std::chrono::system_clock::now();

    while (playing) {

        if (lastState != gameState) {
            // State reciently changed. Systems vector must be updated
            systems.clear();
            switch (gameState) {
            case WIN:
                systems.emplace_back(std::make_unique<WinSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());
                break;

            case GAMEOVER:
                systems.emplace_back(std::make_unique<GameOverSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());

                break;

            case PLAYING:
                init();
                break;

            case MAINMENU:
                systems.emplace_back(std::make_unique<MainMenuSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());

                //init();
                break;

            case LOADING:
                sysStaticEntities.postInit(*this);
                systems.emplace_back(std::make_unique<LoadingSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());

                break;

            case EXIT:
                systems.emplace_back(std::make_unique<ExitSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());

                break;

            case PAUSE:
                systems.emplace_back(std::make_unique<PauseSystem>());
                systems.emplace_back(std::make_unique<SoundSystem>());

                break;

            case RESET:
                reset();
            }

            lastState = gameState;
        }

        calculateDeltaTime(then);



        deltaFromLastUpdate += deltaTime;
        if (deltaFromLastUpdate >= DELTA_TO_UPDATE) {
            update();
            updateSound();
            deltaFromLastUpdate = 0;
        }

        render();

    }//PLAYING


    //GAME CLOSED
}

void GameEngine::saveGame() {

}

void GameEngine::loadGame() {

}

void GameEngine::update() {

    entityMan.clearEntitiesToUpdate();

    for (size_t i = 0; i < systems.size(); ++i) {
        systems[i]->update(*this);
    }
}

void GameEngine::updateSound() {
    soundFacade.update(*this);
}

void GameEngine::render() {
    sysRender.update(*this);
}


void GameEngine::updatePlaying(const bool p) {
    playing = p;
}

const float GameEngine::getDeltaTime() const {
    return deltaFromLastUpdate;
}

void GameEngine::calculateDeltaTime(std::chrono::time_point<std::chrono::system_clock> &then) {
    //calculate deltaTime
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
    std::chrono::duration<float, std::ratio<1>> elapsed_time = now - then;
    deltaTime = elapsed_time.count();


    then = now;

}

WindowFacade &GameEngine::getWindowFacadeRef() {
    return windowFacade;
}

SoundFacade &GameEngine::getSoundFacadeRef() {
    return soundFacade;
}



Entity &GameEngine::getEntity(uint32_t id) {
    return entityMan.getEntity(id);
}

/* Tonto estabas buscando esto? que no hay que usarlo, pringao
std::vector<uint32_t> GameEngine::getEntitiesIdByType(EntityType type) {
    return entityMan.getEntitiesIdByType(type);
}*/


void GameEngine::eraseEntityByID(uint32_t id) {
    windowFacade.eraseEntity(id);


    // Removing the entity ID on Components of ais
    for (auto &combatComp : entityMan.getComponents<AICombatComponent>()) {
        if (combatComp.targetId == id) {
            combatComp.targetId = 0;
            entityMan.getComponent<AIComponent>(combatComp.id).forceStandBy = true;
        }
    }

    for (auto &defensiveComp : entityMan.getComponents<AIDefensiveComponent>()) {
        if (defensiveComp.targetId == id) {
            defensiveComp.targetId = 0;
        }

    }

    for (auto &moveTo3DComp : entityMan.getComponents<AIMoveTo3DComponent>()) {
        if (moveTo3DComp.targetId == id) {
            moveTo3DComp.targetId = 0;
        }
    }

    for (auto &moveToComp : entityMan.getComponents<AIMoveToComponent>()) {
        if (moveToComp.targetId == id) {
            moveToComp.targetId = 0;
        }
    }

    for (auto &offensiveComp : entityMan.getComponents<AIOffensiveComponent>()) {

        if (offensiveComp.targetId == id) {
            offensiveComp.targetId = 0;

        }
    }

    for (auto &standByComp : entityMan.getComponents<AIStandByComponent>()) {

        if (standByComp.targetId == id) {
            standByComp.targetId = 0;
        }
    }

    for (auto &aiComp : entityMan.getComponents<AIComponent>()) {

        if (aiComp.aggredBy == id) {
            aiComp.aggredBy = 0;

        }
    }

    if (entityMan.existsComponent<AIActionLungeComponent>(id)) {
        auto &aiLungeComp = entityMan.getComponent<AIActionLungeComponent>(id);
        if (aiLungeComp.attackId) {
            if (entityMan.existsComponent<AttackComponent>(aiLungeComp.attackId)) {
                AttackComponent &attack = entityMan.getComponent<AttackComponent>(aiLungeComp.attackId);
                attack.lifeTime = 0;
            }
        }
    }

    // Remove the light of the ilumination component
    if(entityMan.existsComponent<IluminationComponent>(id)) {
        IluminationComponent &iluminationComp = entityMan.getComponent<IluminationComponent>(id);

        // Delete the light
        if(iluminationComp.pointLightId != 0) {
            eraseEntityByID(iluminationComp.pointLightId);
        }
    }


    // Removing the entity ID on entities which collides and are detected with it
    if (entityMan.getComponents<CollisionComponent>().count(id)) {
        BoundingComponent &collisionComponent = entityMan.getComponent<CollisionComponent>(id);

        std::vector<uint32_t> collisions = collisionComponent.entitiesColliding;

        for (uint32_t collisionId : collisions) {

            CollisionComponent &collisionComponentToUpdate = entityMan.getComponent<CollisionComponent>(collisionId);
            collisionComponentToUpdate.removeEntityColliding(id);

        }
    }

    auto &allSensors = entityMan.getComponents<BoundingComponent>();
    for (auto &sensor : allSensors) {
        sensor.removeEntityColliding(id);
    }

    for (auto &inventory : entityMan.getComponents<InventoryComponent>()) {

        if (inventory.primaryWeaponId == id) {

            inventory.primaryWeaponId = 0;
        }

        if (inventory.secondaryWeaponId == id) {
            inventory.secondaryWeaponId = 0;
        }
    }

    for (auto &interaction : entityMan.getComponents<InteractionComponent>()) {

        if (interaction.weaponIdInteracting == id) {
            interaction.weaponIdInteracting = 0;
        }
        if (interaction.coinChestIdInteracting == id) {
            interaction.coinChestIdInteracting = 0;
        }
        if (interaction.keyInteracting == id) {
            interaction.keyInteracting = 0;
        }
        if (interaction.dialogueInteracting == id) {
            interaction.dialogueInteracting = 0;
        }
        if (interaction.entityInteractibleId == id) {
            interaction.entityInteractibleId = 0;
        }
    }

    for(auto &infoBox : entityMan.getComponents<InfoBoxComponent>()) {

        if (infoBox.parentEntityId == id) {
            infoBox.parentEntityId = 0;
        }
    }

    for(auto &messageBox : entityMan.getComponents<MessageBoxComponent>()) {
        if (messageBox.parentEntityId == id) {
            messageBox.parentEntityId = 0;
        }
    }

    for(auto &attack : entityMan.getComponents<AttackComponent>()) {

        if (attack.attackerId == id) {

            attack.attackerId = 0;
        }
    }


    // ##### MAY NOT BE HERE ##### //
    for(auto &chest : entityMan.getComponents<ChestComponent>()) {

        if (chest.interrogation == id) {

            chest.interrogation = 0;
        }
    }

    for(auto &dialogue : entityMan.getComponents<DialogueComponent>()) {

        if (dialogue.interrogation == id) {

            dialogue.interrogation = 0;
        }
    }

    for(auto &ip : entityMan.getComponents<IPDataComponent>()) {

        if (ip.secureZoneId == id) {

            ip.secureZoneId = 0;
        }
    }

    for(auto &atrib : entityMan.getComponents<AtributesComponent>()) {

        if (atrib.secureZone == id) {

            atrib.secureZone = 0;
        }
    }

    for(auto &secureZone : entityMan.getComponents<SecureZoneComponent>()) {

        if (secureZone.cristalLightId == id) {

            secureZone.cristalLightId = 0;
        }
    }

    // ##### MAY NOT BE HERE - CHECK EFICIENCE ##### //
    for(auto &coin : entityMan.getComponents<CoinComponent>()) {

        if (coin.owner == id) {

            coin.owner = 0;
        }
    }

    // ##### THIS COMPONENTS HAS A VECTOR ##### //
    //MAP         COMPONENT
    //TERRAINDATA COMPONENT


    std::vector<uint32_t> waypointsToDelete;
    auto &allWaypoints = entityMan.getComponents<WaypointComponent>();

    for (auto &waypoint : allWaypoints) {

        if (waypoint.target_of == id) {

            waypointsToDelete.emplace_back(waypoint.id);
        }
    }

    for (uint32_t waypointId : waypointsToDelete) {
        eraseEntityByID(waypointId);

    }

    getSoundFacadeRef().setParameterEventByID(id, STOP_SOUND);

    entityMan.eraseEntityByID(id);


}



std::unordered_map<uint32_t, Entity> &GameEngine::getEntities() {

    return entityMan.getEntities();
}


std::vector<uint32_t> &GameEngine::getEntitiesToUpdate() {
    return entityMan.getEntitiesToUpdate();
}

void GameEngine::addEntityToUpdate(const uint32_t id) {
    entityMan.addEntityToUpdate(id);
}

void GameEngine::clearEntitiesToUpdate() {
    entityMan.clearEntitiesToUpdate();
}

GameState GameEngine::getGameState() const {
    return gameState;
}

void GameEngine::setGameState(const GameState gs) {
    gameState = gs;
}

void GameEngine::setLastState(const GameState gs) {
    lastState = gs;
}
