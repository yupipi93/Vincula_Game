#pragma once

#include "man/EntityManager.hpp"

#include <sys/RenderSystem.hpp>
#include <sys/StaticEntitiesSystem.hpp>
#include <sys/SystemsIncluder>

#include <ent/Entity.hpp>

#include <com/ComponentsIncluder>

#include <fac/WindowFacade.hpp>
#include <fac/SoundFacade.hpp>

#include <enum/EnumsIncluder>

#include <eng/pap/PapayaEngine.hpp>
#include <eng/fmod/VibrateEngine.hpp>

#include <memory>


class GameEngine {
public:
    GameEngine(bool debugMode, int width, int height, bool windowsMode);
    ~GameEngine();

    void reset();
    void init();
    void run() ;
    void saveGame();
    void loadGame();
    WindowFacade &getWindowFacadeRef();
    SoundFacade &getSoundFacadeRef();
    void updatePlaying(const bool p);
    const float getDeltaTime() const;
    void calculateDeltaTime(std::chrono::time_point<std::chrono::system_clock> &then);



    Entity                              &getEntity(uint32_t id)                ;
    std::vector<uint32_t>               getEntitiesIdByType(EntityType type)   ;


    std::unordered_map<uint32_t, Entity>                         &getEntities();

    void eraseEntityByID                 (uint32_t id);

    std::vector<uint32_t> &getEntitiesToUpdate();
    void addEntityToUpdate(const uint32_t id);
    void clearEntitiesToUpdate();

    GameState getGameState() const;
    void setGameState(const GameState gs);
    void setLastState(const GameState gs);

    //DEBUG
    bool debuging = false;
    int screenWidth = 1920;
    int screenHeight = 1080;


    //Static Ids
    //TODO, put all this fuking ids in a one component "staticIdsComponent"
    uint32_t                playerId            {0};
    uint32_t                terrainId           {0};
    uint32_t                HUDId               {0};
    uint32_t                cameraId            {0};
    uint32_t                sourceId            {0};
    uint32_t                bankId              {0};
    uint32_t                bankDepositorId     {0};
    uint32_t                bankExtractorId     {0};
    uint32_t                aIDirectorId        {0};
    uint32_t                obstacleDataId      {0};
    uint32_t                npcShamanImprover   {0};
    uint32_t                npcOldMan           {0};
    uint32_t                doorId              {0};
    uint32_t                controlsFloorId     {0};
    uint32_t                compassId           {0};
    uint32_t                compassPointsTo     {0};
    uint32_t                mapId               {0};
    uint32_t                dirLightId          {0};
    uint32_t                mouseLightId        {0};
    uint32_t                mouseShapeId        {0};
    uint32_t                palantirId          {0};
    uint32_t                cowboyId            {0};
    std::vector<uint32_t>   villageIds          {};
    std::vector<uint32_t>   lightCristalIds     {};

    //WEAPONS CREATED
    std::unordered_map<GameObjectType,bool> weaponsCreated {
        {DISTANCE_BOW, false},
        {DISTANCE_BLOWGUN, false},
        {DISTANCE_SLINGSHOT, false},
        {MELEE_STICK, false},
        {MELEE_MACE, false},
        {MELEE_SWORD, false}
    };

    //SOUNDS
    uint32_t                ambietMusicId        {0};
    uint32_t                ambientSFXId         {0};
    uint32_t                heartSoundId         {0};
    uint32_t                playerActionsEventId {0};
    uint32_t                winEventId           {0};
    uint32_t                gameOverEventId      {0};



    uint8_t                 playerBiome         {0};
    uint8_t                 lastPlayerBiome     {0};
    float                   distanceToCenter    {0.f};
    bool                    playerHasWeapon     {false};


    //BIOME NAMES
    std::vector<float>      timeInBiomes        {};
    uint8_t                 biomeInAnimation    {};

    //INFO ACTIONS
    std::vector<bool>       desactiveInfoActions            {false};

    //INFO NOTIFICATIONS:
    // To add a new notifications just adjust the
    // activeInfoNotifications.resize() in Constructor,
    // add the resource to load in PapayaEngine.cpp
    // and stablish the gameContext.activeInfoNotifications[X]
    // where it must
    float                   notificationTime                {0.f};
    std::vector<bool>       activeInfoNotifications         {false};

    //TUTORIAL
    uint8_t                 tutorialPhases      {0};
    uint32_t                lastDialogueID      {0};


    // DAY TIME
    float currentHour {6.f};
    float timeFactor  {24};//24  // 60sec (REAL) = 1h (GAME)

    // TODO: Will deprecate
    /*int numEntities     {0};
    int prevNumEntities {0};*/


    EntityManager   entityMan;

    uint8_t         stateOptionSelected {0};
    uint8_t         finalDialogue       {0};
    uint16_t        dropedCoins         {0};
    uint8_t         loadingProcess      {0};
    std::string     loadingText         {"Borrando recuerdos ("};

    //Final Statistics
    float           playedTime          {0};
    uint16_t        deaths              {0};
    uint32_t        totalDropedCoins    {0};
    uint32_t        enemiesDeaths       {0};


private:
    void update();
    void updateWithTimers();
    void updateSound();
    void render();
    void calculatePausedTime();

    std::vector<std::unique_ptr<System>> systems;
    //std::unique_ptr<Vec3> v1 = std::make_unique<Vec3>();
    RenderSystem sysRender;
    StaticEntitiesSystem sysStaticEntities;

    PapayaEngine windowFacade;
    //IrrlichtEngine windowFacade;
    VibrateEngine soundFacade;

    const uint8_t MAX_SYSTEMS       { 255 };


    float deltaTime {0};
    float deltaFromLastUpdate {0};

    // To check process time of the system
    //This works every 5 updates it gets the average value of the time for each system
    std::vector<float *> systemTimes;
    uint32_t timesUpdated_sys = 0;
    std::vector<float> updateTimes;
    std::vector<float> soundTimes;
    std::vector<float> renderTimes;
    std::vector<float> allTimes;
    uint32_t timesUpdated_upd = 0;
    const uint8_t MAX_TIMES = 25;

    bool        playing     {true};
    GameState   gameState   {MAINMENU};
    GameState   lastState   {NOSTATE};

};
