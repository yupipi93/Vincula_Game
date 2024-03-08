#include "LightSystem.hpp"
#include <tools/Utils.hpp>
//console.log


//#####################################################
//               == LIGHT SYSTEM ==
//  - Create or destroy point light if they are in the visible zone
//  - Manages the directional light (Sun) creating day and night cycle
//
//   Using Componets:
//      - LightComponent        (Read - Write)
//      - SituationComponent    (Read - Write)
//
//#####################################################

/*const float dayPosX   { 100.f};
const float nightPosX {-100.f};

const float maxSunHeight {150.f};
const float minSunHeight {20.f};*/

const float horizontalRadius {100.f};
const float verticalRadius   {60.f};
const float maxElipseAng {195.f};
const float minElipseAng {-15.f};

const float noShadowHours {1.f}; // hours without shadow

const float dayStarts   {8.f};
const float nightStarts {20.f};

// For the color
const float nightColorStarts  {20.f};
const float dayColorStarts   {5.f};
const float afterColorStarts {13.f};


const std::array<float, 3> dayColor   {1.f, 1.f, 1.f};
const std::array<float, 3> nightColor {0.12f, 0.17f, 0.55f};
const std::array<float, 3> afterColor {1.f, 0.72f, 0.27f};

LightSystem::LightSystem() {}

LightSystem::~LightSystem() {}

void LightSystem::update(GameEngine &gameContext) const {
    updateCristalLights(gameContext);

    updateDirectionalLight(gameContext);
}

void LightSystem::updateCristalLights(GameEngine &gameContext) const {
    float limitLeft, limitRigth, limitTop, limitBottom;
    //get all secureZoneComp
    auto &allSecureZones {gameContext.entityMan.getComponents<SecureZoneComponent>()};
    Utils::calculateVisibleZoneLimits(limitLeft, limitRigth, limitTop, limitBottom, gameContext);

    for(auto &secureZoneComp : allSecureZones) {
        uint32_t lightId = secureZoneComp.cristalLightId;
        SituationComponent &lightSit = gameContext.entityMan.getComponent<SituationComponent>(lightId);
        bool outOfLimits = Utils::checkSituationIsOutsideLimits(lightSit, limitLeft, limitRigth, limitTop, limitBottom);

        if(outOfLimits) {
            gameContext.getWindowFacadeRef().eraseEntity(lightId);

        } else if(!outOfLimits) {
            if(!gameContext.getWindowFacadeRef().existsNode(lightId)) {
                gameContext.getWindowFacadeRef().createPointLight(gameContext, lightId);
            } else {
                updateCristalLightColor(gameContext, secureZoneComp);
            }
        }
    }

}

void LightSystem::updateCristalLightColor(GameEngine &gameContext, SecureZoneComponent &secureZoneComp) const {
    // Set color
    LightComponent &lightComp = gameContext.entityMan.getComponent<LightComponent>(secureZoneComp.cristalLightId);

    if(secureZoneComp.isActive) {
        lightComp.color[0]   = secureZoneComp.activeLightColor[0];
        lightComp.color[1]   = secureZoneComp.activeLightColor[1];
        lightComp.color[2]   = secureZoneComp.activeLightColor[2];

        //Smaller light --- https://learnopengl.com/Lighting/Light-casters
        lightComp.constant   = 1.f;
        lightComp.linear     = 0.014f;
        lightComp.quadratic  = 0.0007f;

    } else {
        lightComp.color[0]   = 0.15f;
        lightComp.color[1]   = 0.5f;
        lightComp.color[2]   = 1.0f;

        //Bigger light
        lightComp.constant   = 1.f;
        lightComp.linear     = 0.007f;
        lightComp.quadratic  = 0.0002f;

    }
    gameContext.getWindowFacadeRef().updatePointLight(gameContext, secureZoneComp.cristalLightId);
}






void LightSystem::updateDirectionalLight(GameEngine &gameContext) const {
    uint32_t dirLightID = gameContext.dirLightId;
    updatePosition(gameContext, dirLightID);
    updateDirection(gameContext, dirLightID);
    updateColor(gameContext, dirLightID);

    // Now update on the Engine
    gameContext.getWindowFacadeRef().updateDirectionalLight(gameContext, dirLightID);
}

void LightSystem::updatePosition(GameEngine &gameContext, uint32_t dirLightID) const {
    SituationComponent &playerSit   = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
    SituationComponent &dirLightSit = gameContext.entityMan.getComponent<SituationComponent>(dirLightID);
    float currentHour = gameContext.currentHour;
    float angle = 0.f;

    // Variables with the time of the day
    //60.f, 67.f, -45.f
    if(currentHour > nightStarts + noShadowHours || currentHour <= nightStarts) { // during shadow time
        //Calculate the angle in witch we are
        float totalAngle  = abs(minElipseAng) + abs(maxElipseAng);
        float angleHours  = 24.f - noShadowHours;
        float auxCurrentH = currentHour;
        if(auxCurrentH > nightStarts + noShadowHours) {
            auxCurrentH -= nightStarts + noShadowHours;
        } else {
            auxCurrentH += 24.f - (nightStarts + noShadowHours);
        }

        float percAngle = auxCurrentH / angleHours;

        angle = (percAngle * totalAngle) + minElipseAng;

    } else { //No shadow hours
        //angle = 270.f;
        //Calculate the angle in witch we are
        float totalAngle  = 360 - (abs(minElipseAng) + abs(maxElipseAng));
        float angleHours  = noShadowHours;
        float auxCurrentH = currentHour - nightStarts;

        float percAngle = auxCurrentH / angleHours;

        angle = (percAngle * totalAngle) + maxElipseAng;
    }

    dirLightSit.x = playerSit.x + horizontalRadius * cos(Utils::degToRad(angle));
    dirLightSit.y = playerSit.y + verticalRadius * sin(Utils::degToRad(angle));

    // Constant
    dirLightSit.z = playerSit.z - 45.f;
}

void LightSystem::updateDirection(GameEngine &gameContext, uint32_t dirLightID) const {
    SituationComponent &playerSit   = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
    SituationComponent &dirLightSit = gameContext.entityMan.getComponent<SituationComponent>(dirLightID);
    LightComponent     &dirLightCom = gameContext.entityMan.getComponent<LightComponent>(dirLightID);

    // The objective of the light is the player
    dirLightCom.objective[0] = playerSit.x;
    dirLightCom.objective[1] = playerSit.y;
    dirLightCom.objective[2] = playerSit.z;

    // Calculate direction
    dirLightCom.direction[0] = dirLightCom.objective[0] - dirLightSit.x;
    dirLightCom.direction[1] = dirLightCom.objective[1] - dirLightSit.y;
    dirLightCom.direction[2] = dirLightCom.objective[2] - dirLightSit.z;
}

void LightSystem::updateColor(GameEngine &gameContext, uint32_t dirLightID) const {
    LightComponent &dirLightCom = gameContext.entityMan.getComponent<LightComponent>(dirLightID);
    float currentHour = gameContext.currentHour;

    if(currentHour > afterColorStarts && currentHour <= nightColorStarts) { // from afternoon to night
        float totalChangeR = afterColor[0] - nightColor[0];
        float totalChangeG = afterColor[1] - nightColor[1];
        float totalChangeB = afterColor[2] - nightColor[2];
        float afterHours = nightColorStarts - afterColorStarts;
        float percAfter = (currentHour - afterColorStarts) / afterHours;

        float changeR = afterColor[0] - totalChangeR * percAfter;
        float changeG = afterColor[1] - totalChangeG * percAfter;
        float changeB = afterColor[2] - totalChangeB * percAfter;

        dirLightCom.color = std::array<float, 3> {changeR, changeG, changeB};

    } else if(currentHour > dayColorStarts && currentHour <= afterColorStarts) { // during day time
        float totalChangeR = dayColor[0] - afterColor[0];
        float totalChangeG = dayColor[1] - afterColor[1];
        float totalChangeB = dayColor[2] - afterColor[2];
        float dayHours = afterColorStarts - dayColorStarts;
        float percDay = (currentHour - dayColorStarts) / dayHours;

        float changeR = dayColor[0] - totalChangeR * percDay;
        float changeG = dayColor[1] - totalChangeG * percDay;
        float changeB = dayColor[2] - totalChangeB * percDay;

        dirLightCom.color = std::array<float, 3> {changeR, changeG, changeB};

    } else { // From night no day
        float totalChangeR = dayColor[0] - nightColor[0];
        float totalChangeG = dayColor[1] - nightColor[1];
        float totalChangeB = dayColor[2] - nightColor[2];

        float nightHours = 24 - (nightColorStarts - dayColorStarts);
        float auxCurrentH = currentHour;
        if(auxCurrentH > nightColorStarts) {
            auxCurrentH -= nightColorStarts;
        } else {
            auxCurrentH += 24.f - nightColorStarts;
        }

        float percNight = auxCurrentH / nightHours;

        float changeR = nightColor[0] + totalChangeR * percNight;
        float changeG = nightColor[1] + totalChangeG * percNight;
        float changeB = nightColor[2] + totalChangeB * percNight;

        dirLightCom.color = std::array<float, 3> {changeR, changeG, changeB};
    }
}