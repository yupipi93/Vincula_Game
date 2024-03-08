#pragma once
class GameEngine;
#include <com/ComponentsForwardDeclaration>
#include <enum/EnumsIncluder>
#include <stdint.h>

//#include <tools/Storage.hpp>
//#include <stdint.h>

struct AIfunctions {
    AIfunctions();

    static void turnToTarget(GameEngine &gameContext, float angle, SituationComponent &situation);
    static void setAction(GameEngine &gameContext, AIComponent &aIComponent, AiAction action);
    static void removeAiAction(GameEngine &gameContext, uint32_t entityId);
    static void removeTarget(GameEngine &gameContext, uint32_t entityId);
    static uint32_t createTarget(GameEngine &gameContext, uint32_t entityId, float posX, float posZ);
    static void moveTarget(GameEngine &gameContext, uint32_t wayPointId, float posX, float posZ);

};
