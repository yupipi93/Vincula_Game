#include "TerrainSystem.hpp"

#include <tools/Utils.hpp>
#include <algorithm>

#include <eng/GameEngine.hpp>


//console.log



//#####################################################
//                == TERRAIN SYSTEM ==
//  - Move the terrain and the terrain blocks depending
// on the player position, if he has moved x blocks, this
// moves the terrain x blocks
//
//   Using Componets:
//      - TerrainDataComponent  (Read - Write)
//      - SituationComponent    (Read - Write)
//      - DrawableComponent     (Write)
//
//#####################################################

TerrainSystem::TerrainSystem() {}

TerrainSystem::~TerrainSystem() {}


void TerrainSystem::update(GameEngine &gameContext) const {
    // Get the terrain data
    uint32_t             terrainId         = gameContext.terrainId;
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    // Get the player data
    uint32_t             playerId          = gameContext.playerId;
    SituationComponent   &playerSituation  = gameContext.entityMan.getComponent<SituationComponent>(playerId);

    // Reset movements from last iteration
    terrainData.resetMovements();

    // check if the player has moved of terrain block
    if(isPlayerMoved(terrainData, terrainSituation, playerSituation, gameContext)) {
        calculateCubesToMove(terrainId, terrainData, gameContext);
    }

}


// Of the update
/*bool TerrainSystem::isPlayerMoved(TerrainDataComponent &terrainData, SituationComponent &terrainSituation
                                  , SituationComponent &playerSituation, GameEngine &gameContext) const {
    bool  playerMoved {false};
    // The position of the player is the top left corner
    float playerPosX = playerSituation.x;
    float playerPosZ = playerSituation.z;

    // Calculate the position of the center of the middle block
    float middleTerrainPosX = terrainSituation.x + (terrainData.terrainWidth * terrainData.cubeSize / 2)  + terrainData.cubeSize / 2;
    float middleTerrainPosZ = terrainSituation.z - (terrainData.terrainHeight * terrainData.cubeSize / 2) - terrainData.cubeSize / 2;

    float differenceX = playerPosX - middleTerrainPosX;
    float differenceZ = playerPosZ - middleTerrainPosZ;

    if( abs(differenceX) > (terrainData.cubeSize / 2) ) {
        // The player has moved some blocks in X
        playerMoved = true;
        //to see which way the player has moved
        if(differenceX < 0)
            terrainData.movedLeft  = true;
        else
            terrainData.movedRight = true;

    }
    if( abs(differenceZ) > (terrainData.cubeSize / 2) ) {
        playerMoved = true;
        //to see which way the player has moved
        if(differenceZ < 0)
            terrainData.movedDown = true;
        else
            terrainData.movedUp   = true;

    }


    return playerMoved;
}*/

bool TerrainSystem::isPlayerMoved(TerrainDataComponent &terrainData, SituationComponent &terrainSituation
                                  , SituationComponent &playerSituation, GameEngine &gameContext) const {
    bool  playerMoved {false};

    int playerRoundedPosX  = Utils::getRoundedPositionX(playerSituation.x, gameContext);
    int terrainRoundedPosX = Utils::getRoundedPositionX(terrainSituation.x, gameContext);
    int playerRoundedPosZ  = Utils::getRoundedPositionZ(playerSituation.z, gameContext);
    int terrainRoundedPosZ = Utils::getRoundedPositionZ(terrainSituation.z, gameContext);


    int differenceX = playerRoundedPosX - (terrainRoundedPosX + terrainData.terrainWidth / 2 );
    int differenceZ = playerRoundedPosZ - (terrainRoundedPosZ - terrainData.terrainHeight / 2 - 1 );


    if( abs(differenceX) != 0 ) {
        // The player has moved some blocks in X
        playerMoved = true;
        terrainData.cubesMoved = abs(differenceX);
        //to see which way the player has moved
        if(differenceX < 0)
            terrainData.movedLeft  = true;
        else
            terrainData.movedRight = true;

    }
    if( abs(differenceZ) != 0 ) {
        playerMoved = true;
        terrainData.cubesMoved = abs(differenceZ);
        //to see which way the player has moved
        if(differenceZ < 0)
            terrainData.movedDown = true;
        else
            terrainData.movedUp   = true;

    }


    return playerMoved;
}

void TerrainSystem::calculateCubesToMove(uint32_t terrainId, TerrainDataComponent &terrainData, GameEngine &gameContext) const {
    if(terrainData.movedLeft || terrainData.movedRight) {
        insertCubesToMoveInX(terrainId, gameContext);
        if(terrainData.cubeIdsToMove.size() != 0)
            moveCubesInX(terrainId, gameContext);
        
    }
    if(terrainData.movedDown || terrainData.movedUp) {
        insertCubesToMoveInZ(terrainId, gameContext);
        if(terrainData.cubeIdsToMove.size() != 0)
            moveCubesInZ(terrainId, gameContext);
        
    }

    //checkRandomForDrawable(terrainId, gameContext);
}

void TerrainSystem::insertCubesToMoveInX(uint32_t terrainId, GameEngine &gameContext) const {
    auto                    &terrainCubes   = gameContext.entityMan.getComponents<TerrainCubeComponent>();
    //auto                    &allSituations  = gameContext.entityMan.getComponents<SituationComponent>();
    TerrainDataComponent    &terrainData    = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);


    float posObjectiveX = calculatePosObjectiveX(terrainId, gameContext);


    //Move the terrain and calculate the cubes moved
    moveTerrainX(terrainId, gameContext);

    for(uint8_t nColums = 0; nColums < terrainData.cubesMoved && nColums < terrainData.terrainWidth; ++nColums) {
        // Iterate over the cubes to see which are in the objective position
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if( cubeSituation.x == posObjectiveX ) {
                //inset cube to move it
                terrainData.cubeIdsToMove.emplace_back(cube.id);
            }
        }

        // If the player has moved more than one cube
        if(terrainData.movedLeft) {
            // moving left
            posObjectiveX -= terrainData.cubeSize;
        } else {
            // moving right
            posObjectiveX += terrainData.cubeSize;
        }

    }
}

void TerrainSystem::insertCubesToMoveInZ(uint32_t terrainId, GameEngine &gameContext) const {
    auto                    &terrainCubes   = gameContext.entityMan.getComponents<TerrainCubeComponent>();
    //auto                    &allSituations  = gameContext.entityMan.getComponents<SituationComponent>();
    TerrainDataComponent    &terrainData    = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);


    float posObjectiveZ = calculatePosObjectiveZ(terrainId, gameContext);

    //Move the terrain and calculate the cubes moved
    moveTerrainZ(terrainId, gameContext);

    for(uint8_t nRows = 0; nRows < terrainData.cubesMoved && nRows < terrainData.terrainHeight; ++nRows) {
        // Iterate over the cubes to see which are in the objective position
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);

            if( cubeSituation.z == posObjectiveZ ) {
                //inset cube to move it
                terrainData.cubeIdsToMove.emplace_back(cube.id);
            }
        }

        // If the player has moved more than one cube
        if(terrainData.movedUp) {
            // moving up
            posObjectiveZ += terrainData.cubeSize;
        } else {
            // moving down
            posObjectiveZ -= terrainData.cubeSize;
        }

    }
}


float TerrainSystem::calculatePosObjectiveX(uint32_t terrainId, GameEngine &gameContext) const {
    float result = 0.f;
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);

    if(terrainData.movedLeft) {
        // moving left
        result = terrainSituation.x + (terrainData.terrainWidth * terrainData.cubeSize) - terrainData.cubeSize;

    } else if(terrainData.movedRight) {
        // moving right
        result = terrainSituation.x;
    }
    return result;
}



float TerrainSystem::calculatePosObjectiveZ(uint32_t terrainId, GameEngine &gameContext) const {
    float result = 0.f;
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);

    if(terrainData.movedUp) {
        // moving up
        result = terrainSituation.z - (terrainData.terrainHeight * terrainData.cubeSize);

    } else if(terrainData.movedDown) {
        // moving down
        result = terrainSituation.z - terrainData.cubeSize;

    }

    return result;
}

void TerrainSystem::moveTerrainX(uint32_t terrainId, GameEngine &gameContext) const {
    SituationComponent   &terrainSituation  = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    TerrainDataComponent &terrainData       = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);

    if(terrainData.movedLeft) {
        terrainSituation.x -= (terrainData.cubeSize * terrainData.cubesMoved);
    } else {
        terrainSituation.x += (terrainData.cubeSize * terrainData.cubesMoved);
    }
}

void TerrainSystem::moveTerrainZ(uint32_t terrainId, GameEngine &gameContext) const {
    SituationComponent   &terrainSituation  = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    TerrainDataComponent &terrainData       = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);

    if(terrainData.movedUp) {
        terrainSituation.z += (terrainData.cubeSize * terrainData.cubesMoved);
    } else {
        terrainSituation.z -= (terrainData.cubeSize * terrainData.cubesMoved);
    }
}


void TerrainSystem::moveCubesInX(uint32_t terrainId, GameEngine &gameContext) const {
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    //auto                 &allSituations    = gameContext.entityMan.getComponents<SituationComponent>();
    std::vector<uint32_t> cubesMoved {};
    std::vector<uint32_t> idsToDelete {};
    //uint8_t randomToMove = 0;
    float newPositionX = 0.f;

    cubesMoved.reserve(terrainData.terrainHeight);
    idsToDelete.reserve(terrainData.terrainHeight);


    for(uint8_t nColums = 0; nColums < terrainData.cubesMoved && nColums < terrainData.terrainWidth; ++nColums) {
        // Calculate new position in X
        if(terrainData.movedLeft) {
            newPositionX = terrainSituation.x + (terrainData.cubeSize * nColums );
        } else if(terrainData.movedRight) {
            newPositionX = terrainSituation.x + ((terrainData.terrainWidth - 1) * terrainData.cubeSize) - (terrainData.cubeSize * nColums);
        }

        // Now we have to move the line of cubes to the new positions
        for(std::size_t z = 0; z < terrainData.terrainHeight && z < terrainData.cubeIdsToMove.size(); ++z) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainData.cubeIdsToMove.at(z));
            //randomToMove = rand() % 100 + (0); //entre 0 y 100

            //if(randomToMove <= terrainData.randomizeFactor){
            // Move the cube
            cubeSituation.x = newPositionX;
            cubeSituation.z = terrainSituation.z - (terrainData.cubeSize * (z + 1));

            cubesMoved.emplace_back( terrainData.cubeIdsToMove.at(z) );

            idsToDelete.emplace_back(z); // Position of the id to delete from moving
        }

        // Update in Irrlicht
        updateCubesDrawable(cubesMoved, gameContext);

        // Once are moved, delete ID from the vector
        for(std::size_t aux = 0; aux < idsToDelete.size(); ++aux) {
            terrainData.cubeIdsToMove.erase( terrainData.cubeIdsToMove.begin() + idsToDelete[aux] - aux );
        }

        idsToDelete.clear();
        cubesMoved.clear();
    }
}


void TerrainSystem::moveCubesInZ(uint32_t terrainId, GameEngine &gameContext) const {
    TerrainDataComponent &terrainData      = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent   &terrainSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    //auto                 &allSituations    = gameContext.entityMan.getComponents<SituationComponent>();
    std::vector<uint32_t> cubesMoved {};
    std::vector<uint32_t> idsToDelete {};
    float newPositionZ = 0.f;


    for(uint8_t nRows = 0; nRows < terrainData.cubesMoved && nRows < terrainData.terrainHeight; ++nRows) {
        // Calculate new position in Z
        if(terrainData.movedUp) {
            newPositionZ = terrainSituation.z - terrainData.cubeSize - (terrainData.cubeSize * nRows);
        } else if(terrainData.movedDown) {
            newPositionZ = terrainSituation.z - (terrainData.terrainHeight * terrainData.cubeSize) + (terrainData.cubeSize * nRows);
        }

        cubesMoved.reserve(terrainData.terrainWidth);
        idsToDelete.reserve(terrainData.terrainHeight);
        // Now we have to move the line of cubes to the new positions
        for(std::size_t x = 0; x < terrainData.terrainWidth && x < terrainData.cubeIdsToMove.size(); ++x) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(terrainData.cubeIdsToMove.at(x));

            // Move the cube
            cubeSituation.x = terrainSituation.x + (terrainData.cubeSize * (x));
            cubeSituation.z = newPositionZ;

            cubesMoved.emplace_back( terrainData.cubeIdsToMove.at(x) );

            idsToDelete.emplace_back(x); // Position of the id to delete
        }

        // Update in Irrlicht
        updateCubesDrawable(cubesMoved, gameContext);

        // Once are moved, delete ID from the vector
        for(std::size_t aux = 0; aux < idsToDelete.size(); ++aux) {
            terrainData.cubeIdsToMove.erase( terrainData.cubeIdsToMove.begin() + idsToDelete[aux] - aux );
        }

        idsToDelete.clear();
        cubesMoved.clear();
    }
}

void TerrainSystem::updateCubesDrawable(std::vector<uint32_t> &cubesMoved, GameEngine &gameContext) const {
    std::vector<uint32_t> cubesIdToUpdate {};

    cubesIdToUpdate.reserve(cubesMoved.size());
    for(std::size_t i = 0; i < cubesMoved.size(); ++i) {
        if( checkIfHasDrawable(cubesMoved[i], gameContext) ) {
            //set the texture
            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(cubesMoved[i]);
            setCubeTexture(gameContext, cubesMoved[i], drawable);

            //inset in vector to update in Irrlicht
            cubesIdToUpdate.emplace_back(cubesMoved[i]);
        }
    }

    // Update in Irrlicht the cubes that have drawable
    gameContext.getWindowFacadeRef().updateEntities(gameContext, cubesIdToUpdate);
    gameContext.getWindowFacadeRef().updateTerrainCubeTextures(gameContext, cubesIdToUpdate);
}

bool TerrainSystem::checkIfHasDrawable(uint32_t cubeId, GameEngine &gameContext) const {
    auto &allDrawables = gameContext.entityMan.getComponents<DrawableComponent>();
    bool hasDrawable = false;

    //auto it = allDrawables.find(cubeId);
    if (allDrawables.count(cubeId)) {
        // id found in drawables vector
        hasDrawable = true;
    }


    return hasDrawable;
}

void TerrainSystem::setCubeTexture(GameEngine &gameContext, uint32_t cubeId, DrawableComponent  &cubeDrawable) const {
    SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cubeId);
    //DrawableComponent  &cubeDrawable  = gameContext.entityMan.getComponent<DrawableComponent>(cubeId);
    uint8_t biome = 0;

    biome = Utils::calculateBiome(gameContext, cubeSituation.x, cubeSituation.z);
    if (biome > 10)
        biome = 10;

    uint8_t pn = rand() % 100 + 1;
    if(pn < 25)
        pn = 1;
    else if(pn >= 25 && pn < 50)
        pn = 2;
    else if(pn >= 50 && pn < 75)
        pn = 3;
    else if(pn >= 75)
        pn = 4;

    //cubeDrawable.texture = "./media/biomes/terrain" + std::to_string(biome) + "p" + std::to_string(pn) + ".jpg";
    cubeDrawable.texture = Utils::getBiomeTexturePath(biome, pn);
}


void TerrainSystem::checkRandomForDrawable(uint32_t terrainId, GameEngine &gameContext) const {
    TerrainDataComponent    &terrainData            = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    auto                    &terrainCubes           = gameContext.entityMan.getComponents<TerrainCubeComponent>();
    std::vector<uint32_t>   cubesIdToUpdateTexture  {};
    std::vector<float>      percentagesVector       {};

    cubesIdToUpdateTexture.reserve(terrainCubes.size());
    calculateRandomPercentages(terrainData, percentagesVector);

    if(terrainData.movedLeft || terrainData.movedRight) {
        // set or delete drawable to the left and right
        randomizeDrawableX(terrainId, percentagesVector, cubesIdToUpdateTexture, gameContext);
    }

    if(terrainData.movedUp || terrainData.movedDown) {
        // set or delete drawable to the up and down
        randomizeDrawableZ(terrainId, percentagesVector, cubesIdToUpdateTexture, gameContext);
    }

    // UPDATE TEXTURES IN IRRLICHT
    gameContext.getWindowFacadeRef().updateTerrainCubeTextures(gameContext, cubesIdToUpdateTexture);
}


void TerrainSystem::calculateRandomPercentages(TerrainDataComponent &terrainData, std::vector<float> &percentagesVector) const {
    percentagesVector.reserve(terrainData.randomLength);

    for(std::size_t i = 0; i <= terrainData.randomLength; ++i) {
        percentagesVector.emplace_back(i * 100 / terrainData.randomLength);
    }

    for(int i = 0; i < terrainData.cubesMoved; ++i) {
        percentagesVector.emplace_back(100);
    }
}


void TerrainSystem::randomizeDrawableX(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    // Create or delete drawables in the X axe depending of the movemet of the player
    TerrainDataComponent    &terrainData        = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent      &terrainSituation   = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    auto                    &terrainCubes       = gameContext.entityMan.getComponents<TerrainCubeComponent>();

    // Get the player velocity in order to avoid blinking when moving in both axes
    uint32_t playerId            = gameContext.playerId;
    VelocityComponent &playerVel = gameContext.entityMan.getComponent<VelocityComponent>(playerId);

    float posObjetiveLeftX = terrainSituation.x - terrainData.cubeSize;

    //from left to right, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.x == posObjetiveLeftX) {
                if(terrainData.movedLeft) {
                    // creates on left
                    createDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                } else {
                    // deletes on left
                    eraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                }
            }
        }
        posObjetiveLeftX += terrainData.cubeSize;
    }


    float posObjetiveRightX = 0.f;

    posObjetiveRightX = terrainSituation.x + (terrainData.cubeSize * terrainData.terrainWidth);
    //from right to left, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.x == posObjetiveRightX) {
                if(terrainData.movedLeft) {
                    // deletes on right
                    eraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                } else {
                    // creates on right
                    createDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                }
            }
        }
        posObjetiveRightX -= terrainData.cubeSize;
    }

    if(playerVel.velocityZ == 0)
        totalRandomizeDrawableZ(terrainId, percentagesVector, cubesIdToUpdateTexture, gameContext);
}

void TerrainSystem::randomizeDrawableZ(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    // Create or delete drawables in the X axe depending of the movemet of the player
    TerrainDataComponent    &terrainData        = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent      &terrainSituation   = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    auto                    &terrainCubes       = gameContext.entityMan.getComponents<TerrainCubeComponent>();
    float posObjetiveUpZ = 0.f;

    // Get the player velocity in order to avoid blinking when moving in both axes
    uint32_t playerId            = gameContext.playerId;
    VelocityComponent &playerVel = gameContext.entityMan.getComponent<VelocityComponent>(playerId);



    posObjetiveUpZ = terrainSituation.z - terrainData.cubeSize;

    //from up to down, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.z == posObjetiveUpZ) {
                if(terrainData.movedUp) {
                    // creates up
                    createDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                } else {
                    // deletes down
                    eraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                }
            }
        }
        posObjetiveUpZ -= terrainData.cubeSize;
    }



    float posObjetiveDownZ = terrainSituation.z - (terrainData.cubeSize * (terrainData.terrainHeight + 2));
    //from right to left, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.z == posObjetiveDownZ) {
                if(terrainData.movedUp) {
                    // deletes when up
                    eraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                } else {
                    // creates when down
                    createDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);

                }
            }
        }
        posObjetiveDownZ += terrainData.cubeSize;
    }

    if(playerVel.velocityX == 0)
        totalRandomizeDrawableX(terrainId, percentagesVector, cubesIdToUpdateTexture, gameContext);
}

void TerrainSystem::totalRandomizeDrawableZ(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    // Randomizes the Z sides when moving only in X
    TerrainDataComponent    &terrainData        = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent      &terrainSituation   = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    auto                    &terrainCubes       = gameContext.entityMan.getComponents<TerrainCubeComponent>();

    float posObjetiveUpZ = terrainSituation.z - terrainData.cubeSize;
    // Calculate the sides right and left in order to avoid conflicts
    float posLeftX   = terrainSituation.x + (terrainData.cubeSize * terrainData.randomLength);
    float posRightX  = terrainSituation.x + ( (terrainData.terrainWidth - terrainData.randomLength) * terrainData.cubeSize);

    //from up to down, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.z == posObjetiveUpZ && cubeSituation.x > posLeftX && cubeSituation.x < posRightX) {
                createOrEraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);
            }
        }
        posObjetiveUpZ -= terrainData.cubeSize;
    }



    float posObjetiveDownZ = terrainSituation.z - (terrainData.cubeSize * (terrainData.terrainHeight + 2));
    //from right to left, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.z == posObjetiveDownZ && cubeSituation.x > posLeftX && cubeSituation.x < posRightX) {
                createOrEraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);
            }
        }
        posObjetiveDownZ += terrainData.cubeSize;
    }
}


void TerrainSystem::totalRandomizeDrawableX(uint32_t terrainId, std::vector<float> &percentagesVector, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    // Randomizes the Z sides when moving only in X
    TerrainDataComponent    &terrainData        = gameContext.entityMan.getComponent<TerrainDataComponent>(terrainId);
    SituationComponent      &terrainSituation   = gameContext.entityMan.getComponent<SituationComponent>(terrainId);
    auto                    &terrainCubes       = gameContext.entityMan.getComponents<TerrainCubeComponent>();

    float posObjetiveLeftX = terrainSituation.x - terrainData.cubeSize;
    // Calculate the sides up and down in order to avoid conflicts
    float posUpZ   = terrainSituation.z - (terrainData.cubeSize * (terrainData.randomLength + 1));
    float posDownZ = terrainSituation.z - ( (terrainData.terrainHeight - terrainData.randomLength) * terrainData.cubeSize);

    //from up to down, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.x == posObjetiveLeftX && cubeSituation.z > posDownZ && cubeSituation.z < posUpZ) {
                createOrEraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);
            }
        }
        posObjetiveLeftX += terrainData.cubeSize;
    }



    float posObjetiveRightX = terrainSituation.x + (terrainData.cubeSize * terrainData.terrainWidth);
    //from right to left, creates or erases
    for(uint8_t i = 0; i <= terrainData.randomLength; ++i) {
        for(auto &cube : terrainCubes) {
            SituationComponent &cubeSituation = gameContext.entityMan.getComponent<SituationComponent>(cube.id);
            if(cubeSituation.x == posObjetiveRightX && cubeSituation.z > posDownZ && cubeSituation.z < posUpZ) {
                createOrEraseDrawableRandomly(cube.id, percentagesVector[i], cubesIdToUpdateTexture, gameContext);
            }
        }
        posObjetiveRightX -= terrainData.cubeSize;
    }
}



void TerrainSystem::eraseDrawableRandomly(uint32_t cubeId, float percentageErase, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    float randomToErase = rand() % 100 + (0); //entre 0 y 100

    if(randomToErase > percentageErase) {
        if( checkIfHasDrawable(cubeId, gameContext) ) {
            gameContext.entityMan.eraseComponent<DrawableComponent>(cubeId);
            cubesIdToUpdateTexture.erase(std::remove(cubesIdToUpdateTexture.begin(), cubesIdToUpdateTexture.end(), cubeId), cubesIdToUpdateTexture.end());
        }
    }
}

void TerrainSystem::createDrawableRandomly(uint32_t cubeId, float percentageDraw, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    float randomToDraw = rand() % 100 + (0); //entre 0 y 100

    if(randomToDraw <= percentageDraw) {
        // if dont have drawable, set one
        if( !checkIfHasDrawable(cubeId, gameContext) ) {
            //gameContext.addDrawableToEntity(cubeId);
            DrawableComponent &drawable = gameContext.entityMan.createComponent<DrawableComponent>(cubeId);
            setCubeTexture(gameContext, cubeId, drawable);
            cubesIdToUpdateTexture.emplace_back(cubeId);
        }
    }
}

// WTF
void TerrainSystem::createOrEraseDrawableRandomly(uint32_t cubeId, float percentageDraw, std::vector<uint32_t> &cubesIdToUpdateTexture, GameEngine &gameContext) const {
    float randomToDraw = rand() % 100 + (0); //entre 0 y 100

    if(randomToDraw <= percentageDraw) {
        // if dont have drawable, set one
        if( !checkIfHasDrawable(cubeId, gameContext) ) {
            //gameContext.addDrawableToEntity(cubeId);
            DrawableComponent &drawable = gameContext.entityMan.createComponent<DrawableComponent>(cubeId);
            setCubeTexture(gameContext, cubeId, drawable);
            cubesIdToUpdateTexture.emplace_back(cubeId);
        }

    } else {
        //Delete drawable
        if( checkIfHasDrawable(cubeId, gameContext) ) {
            gameContext.entityMan.eraseComponent<DrawableComponent>(cubeId);
            cubesIdToUpdateTexture.erase(std::remove(cubesIdToUpdateTexture.begin(), cubesIdToUpdateTexture.end(), cubeId), cubesIdToUpdateTexture.end());
        }
    }
}