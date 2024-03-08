#include "MapSystem.hpp"

#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

constexpr bool DISCOVER_WALLS = true;

//TEMP
//console.log


//#####################################################
//                == MAP SYSTEM ==
//  - Refresh the entities discovered on map
//
//   Using Componets:
//
//#####################################################


MapSystem::MapSystem() {}

MapSystem::~MapSystem() {}

void MapSystem::update(GameEngine &gameContext) const {
    MapComponent    &map            {gameContext.entityMan.getComponent<MapComponent>   (gameContext.mapId)};

    updateMap(gameContext, map);
    discoverPosition(gameContext, map);
}

void MapSystem::updateMap(GameEngine &gameContext, MapComponent &map) const {
    float limitLeft;
    float limitRigth;
    float limitTop;
    float limitBottom;

    Utils::calculateVisibleZoneLimits(limitLeft, limitRigth, limitTop, limitBottom, gameContext);

    // INTEREST PPOINTS
    auto &IPDatas = gameContext.entityMan.getComponents<IPDataComponent>();
    for (auto ipData : IPDatas) {
        SituationComponent situation = gameContext.entityMan.getComponent<SituationComponent>(ipData.id);

        bool outOfLimits = Utils::checkSituationIsOutsideLimits(situation, limitLeft, limitRigth, limitTop, limitBottom);

        if(!outOfLimits) {
            map.discoverEntity(ipData.id);
        }
    }

    // EGGS
    auto &EggDatas = gameContext.entityMan.getComponents<EggDataComponent>();
    for (auto eggData : EggDatas) {
        SituationComponent situation = gameContext.entityMan.getComponent<SituationComponent>(eggData.id);

        bool outOfLimits = Utils::checkSituationIsOutsideLimits(situation, limitLeft, limitRigth, limitTop, limitBottom);

        if(!outOfLimits) {
            map.discoverEntity(eggData.id);
        }
    }

    // WALLS
    if (DISCOVER_WALLS) {
        auto &WallDatas = gameContext.entityMan.getComponents<WallComponent>();
        for (auto wallDatas : WallDatas) {
            SituationComponent situation = gameContext.entityMan.getComponent<SituationComponent>(wallDatas.id);

            bool outOfLimits = Utils::checkSituationIsOutsideLimits(situation, limitLeft, limitRigth, limitTop, limitBottom);

            if(!outOfLimits) {
                map.discoverEntity(wallDatas.id);
            }
        }
    }

    // If llave completa
    //if (Utils::isKeyComplete(gameContext)) {
    auto &doorSit = gameContext.entityMan.getComponent<SituationComponent>(gameContext.doorId);
    if(!Utils::checkSituationIsOutsideLimits(doorSit, limitLeft, limitRigth, limitTop, limitBottom)) {
        map.discoverEntity(gameContext.doorId);
    }
    //}


}

void MapSystem::discoverPosition(GameEngine &gameContext, MapComponent &map) const {
    SituationComponent    &playerSit    {gameContext.entityMan.getComponent<SituationComponent>   (gameContext.playerId)};
    TerrainDataComponent  &terrainData  {gameContext.entityMan.getComponent<TerrainDataComponent>   (gameContext.terrainId)};

    float radius = terrainData.lastBiome * terrainData.biomeSize + terrainData.baseSize;

    int posX = playerSit.x + radius;
    int posZ = radius - playerSit.z;

    posX *= map.MAX_MAP_DIVISION;
    posX /= (radius * 2);
    posZ *= map.MAX_MAP_DIVISION;
    posZ /= (radius * 2);


    int pos = posX + (posZ * map.MAX_MAP_DIVISION);
    map.discoverArea(pos);


}