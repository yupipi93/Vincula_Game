#include "CameraSystem.hpp"

#include <eng/GameEngine.hpp>

#include <tools/Utils.hpp>
#include <math.h>


//#####################################################
//                == CAMERA SYSTEM ==
//  - Updates the camera position based on player
//      position
//
//   Using Componets:
//      - Situation      (Read and Write)
//#####################################################
constexpr float CAMERA_POSITION_Y = 100.f;//150.f
constexpr float CAMERA_POSITION_Z = -194.f;//-120.f



CameraSystem::CameraSystem() {}

CameraSystem::~CameraSystem() {}

void CameraSystem::update(GameEngine &gameContext) const {

    uint32_t playerId = gameContext.playerId;
    uint32_t cameraId = gameContext.cameraId;
    SituationComponent      &playerSituation    = gameContext.entityMan.getComponent<SituationComponent>(playerId);
    InputComponent          &playerImput        = gameContext.entityMan.getComponent<InputComponent>(playerId);
    SituationComponent      &cameraSituation    = gameContext.entityMan.getComponent<SituationComponent>(cameraId);
    TerrainDataComponent    &terrainData        = gameContext.entityMan.getComponent<TerrainDataComponent>(gameContext.terrainId);


    //##### SELECT CAMERA METOD WITH KEYS (F1, F2, F3, F4, F5, F6) #####//
    if(playerImput.choppedCamera)
        chopped(cameraSituation, playerSituation);
    else if(playerImput.choppedZoomCamera)
        choppedZoom(cameraSituation, playerSituation);
    else if(playerImput.smartCamera)
        smart(cameraSituation, playerSituation);
    else if(playerImput.zenithCamera)
        zenith(cameraSituation, playerSituation);
    else if(playerImput.zenithUpCamera)
        zenithUp(cameraSituation, playerSituation);
    else if (playerImput.airCamera)
        air(cameraSituation, playerSituation);



    smartWallLimit(cameraSituation, terrainData); 
    gameContext.getWindowFacadeRef().updateCamera(gameContext, cameraSituation, playerSituation);
}


void CameraSystem::smart(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    float angle =  atan2(playerSituation.x, playerSituation.z);
    cameraSituation.x   = playerSituation.x + (CAMERA_POSITION_Z * sin(angle));
    cameraSituation.y   = playerSituation.y + CAMERA_POSITION_Y;
    cameraSituation.z   = playerSituation.z + (CAMERA_POSITION_Z * cos(angle));


}

void CameraSystem::chopped(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    cameraSituation.x = playerSituation.x;
    cameraSituation.y = playerSituation.y + CAMERA_POSITION_Y;
    cameraSituation.z = playerSituation.z + CAMERA_POSITION_Z;
}

void CameraSystem::choppedZoom(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    cameraSituation.x = playerSituation.x;
    cameraSituation.y = playerSituation.y + CAMERA_POSITION_Y/2;
    cameraSituation.z = playerSituation.z + CAMERA_POSITION_Z/2;
}

void CameraSystem::zenith(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    float angle =  atan2(playerSituation.x, playerSituation.z);

    cameraSituation.x = playerSituation.x + (-1 * sin(angle));
    cameraSituation.y = playerSituation.y + CAMERA_POSITION_Y + 50.f;
    cameraSituation.z = playerSituation.z + (-1 * cos(angle));
    cameraSituation.rotation = playerSituation.rotation;
}

void CameraSystem::zenithUp(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    float angle =  atan2(playerSituation.x, playerSituation.z);

    cameraSituation.x = playerSituation.x + (-1 * sin(angle));
    cameraSituation.y = playerSituation.y + CAMERA_POSITION_Y + 400.f;
    cameraSituation.z = playerSituation.z + (-1 * cos(angle));
    cameraSituation.rotation = playerSituation.rotation;
}

void CameraSystem::air(SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    float angle =  atan2(playerSituation.x, playerSituation.z);

    cameraSituation.x = playerSituation.x + (-10 * sin(angle));
    cameraSituation.y = playerSituation.y + CAMERA_POSITION_Y + 750.f;
    cameraSituation.z = playerSituation.z + (-10 * cos(angle));
    cameraSituation.rotation = playerSituation.rotation;
}


void CameraSystem::smartWallLimit(SituationComponent &cameraSituation, TerrainDataComponent &terrainData) const {
    uint32_t maxCameraRadius = (terrainData.lastBiome) * terrainData.biomeSize + terrainData.baseSize - 6;

    float distance = Utils::calculateDistance(0, cameraSituation.x, 0, cameraSituation.z);

    if (distance >= maxCameraRadius) {
        cameraSituation.z = -sqrt(maxCameraRadius * maxCameraRadius - cameraSituation.x * cameraSituation.x);
        cameraSituation.y -= distance - maxCameraRadius;
    }
}







//Camara seguimiento zenith con animacion inicial
/*
if(cameraSituation.getPositionY() < playerSituation.getPositionY()+CAMERA_POSITION_Y){
    cameraSituation.setPositionY(cameraSituation.getPositionY()+0.5);
}else{
    cameraSituation.setPositionY(playerSituation.getPositionY() + CAMERA_POSITION_Y);
    cameraSituation.setPositionZ(playerSituation.getPositionZ() - CAMERA_POSITION_Z);
}
if(cameraSituation.getPositionZ() > playerSituation.getPositionZ()-CAMERA_POSITION_Z){
    cameraSituation.setPositionZ(cameraSituation.getPositionZ()-0.5);
}else{
    cameraSituation.setPositionZ(playerSituation.getPositionZ() - CAMERA_POSITION_Z);
}
*/