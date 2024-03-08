#pragma once

#include "Component.hpp"
#include <vector>

class TerrainDataComponent : public Component {
public:
    explicit TerrainDataComponent(const uint32_t id);
    ~TerrainDataComponent() override;

    void resetMovements();

    // This has to be moved to a world entity component
    uint8_t                 lastBiome               {5};
    uint16_t                biomeSize               {1200}; //1200
    uint16_t                baseSize                {500};  //500
    uint8_t                 wallHeight              {140};
    uint8_t                 wallSize                {120};
    uint8_t                 collidablesPerWall      {4};
    uint8_t                 wallCollidableSize      {(uint8_t)(wallSize / collidablesPerWall)};

    // number of terrain cubes in x and z, max 256
    uint8_t                 terrainWidth            {19};
    uint8_t                 terrainHeight           {19};
    uint8_t                 chunkWidthIncrement     {(uint8_t)(terrainWidth / 2)};
    uint8_t                 chunkHeightIncrement    {(uint8_t)(terrainHeight / 2)};

    uint8_t                 randomLength            {4};

    float                   cubeSize                {60.f};

    std::vector<uint32_t>   cubeIdsToMove           {};

    // Bool to know where the player has moved
    bool                    movedUp                 {false};
    bool                    movedDown               {false};
    bool                    movedLeft               {false};
    bool                    movedRight              {false};

    uint8_t cubesMoved = 0;
};
