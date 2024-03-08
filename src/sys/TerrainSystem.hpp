#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class TerrainSystem : public System {
public:
    TerrainSystem();
    ~TerrainSystem();

    void update(GameEngine &gameContext) const override;

private:
    bool  isPlayerMoved(TerrainDataComponent &terrainData, SituationComponent &terrainSituation
                        , SituationComponent &playerSituation, GameEngine &gameContext) const;
    void  calculateCubesToMove(uint32_t terrainId, TerrainDataComponent &terrainData, GameEngine &gameContext) const;

    void  insertCubesToMoveInX(uint32_t terrainId, GameEngine &gameContext) const;
    void  insertCubesToMoveInZ(uint32_t terrainId, GameEngine &gameContext) const;

    float calculatePosObjectiveX(uint32_t terrainId, GameEngine &gameContext) const;
    float calculatePosObjectiveZ(uint32_t terrainId, GameEngine &gameContext) const;

    void  moveTerrainX(uint32_t terrainId, GameEngine &gameContext) const;
    void  moveTerrainZ(uint32_t terrainId, GameEngine &gameContext) const;

    void  moveCubesInX(uint32_t terrainId, GameEngine &gameContext) const;
    void  moveCubesInZ(uint32_t terrainId, GameEngine &gameContext) const;

    void  updateCubesDrawable(std::vector<uint32_t> &cubesMoved, GameEngine &gameContext) const;
    bool  checkIfHasDrawable(uint32_t cubeId, GameEngine &gameContext) const;
    void  setCubeTexture(GameEngine &gameContext, uint32_t cubeId,  DrawableComponent  &cubeDrawable) const;

    void  checkRandomForDrawable(uint32_t terrainId, GameEngine &gameContext) const;
    void  calculateRandomPercentages(TerrainDataComponent &terrainData, std::vector<float> &percentagesVector) const;
    void  randomizeDrawableX(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  randomizeDrawableZ(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  totalRandomizeDrawableZ(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  totalRandomizeDrawableX(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  eraseDrawableRandomly(uint32_t cubeId, float percentageDraw, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  createDrawableRandomly(uint32_t cubeId, float percentageDraw, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
    void  createOrEraseDrawableRandomly(uint32_t cubeId, float percentageDraw, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const;
};