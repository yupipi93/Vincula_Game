#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class StaticEntitiesSystem : public System {
public:
    StaticEntitiesSystem();
    ~StaticEntitiesSystem();

    void    init(GameEngine &gameContext) const;
    void    postInit(GameEngine &gameContext) const;
    void    update(GameEngine &gameContext) const override;
    void    createTerrain(GameEngine &gameContext) const;
    void    resetTerrain(GameEngine &gameContext) const;

private:

    void    createMusicEvents(GameEngine &gameContext) const;
    void    createVillage(GameEngine &gameContext) const;
    void    createMap(GameEngine &gameContext) const;
    void    createCamera(GameEngine &gameContext) const;
    void    createBank(GameEngine &gameContext) const;
    void    createNPCImproverAndEnvironment(GameEngine &gameContext) const;
    void    createHud(GameEngine &gameContext) const;
    void    createWallControls(GameEngine &gameContext) const;
    void    createPlayer(GameEngine &gameContext) const;
    void    createBaseNPCs(GameEngine &gameContext) const;
    void    createSun(GameEngine &gameContext) const;
    void    createMouseLight(GameEngine &gameContext) const;


    void    createSource(GameEngine &gameContext) const;
    void    generateWall(GameEngine &gameContext) const;
    void    generateBiomeStructures(GameEngine &gameContext) const;
    void    setEntitiesToCreate(GameEngine &gameContext, std::vector< std::vector<bool> > &usedCuadrants, std::vector< std::vector<GameObjectType> > &entToGenerate) const;
    void    setEntitiesInPosition(GameEngine &gameContext, std::vector< std::vector<bool> > &usedCuadrants, std::vector< std::vector<GameObjectType> > &entToGenerate) const;
    void    setQuadrant(std::vector< std::vector<uint8_t> > &usedQuadrants, int vectorPos, int entity) const;
    bool    checkQuadrant(std::vector< std::vector<uint8_t> > &usedQuadrants, int vectorPos, int entity) const;
    void    setPositionInQuadrant(float &posX, float &posZ, uint8_t quadrant) const;
    void    createEntitiesInPosition(GameEngine &gameContext, float posX, float posZ, int currentBiome, GameObjectType entToGenerate) const;
    void    createCoinChest(GameEngine &gameContext, float posX, float posY, float posZ, float rot, size_t currentBiome, GameObjectType chestType, uint32_t interrogationId) const;
    void    createWeaponChest(GameEngine &gameContext, float posX, float posY, float posZ, GameObjectType chestType, uint32_t interrogationId) const;
    void    createCubes(GameEngine &gameContext) const;
    void    createWeapons(GameEngine &gameContext) const;
    void    createAIDirector(GameEngine &gameContext) const;
    void    createObstacleData(GameEngine &gameContext) const;
    std::string getCubeTexture(GameEngine &gameContext, const float posX, const float posZ) const;

};