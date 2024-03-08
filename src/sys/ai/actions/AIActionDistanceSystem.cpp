#include "AIActionDistanceSystem.hpp"

//#include <math.h>
#include <tools/Utils.hpp>
#include <tools/AIfunctions.hpp>


#include <eng/GameEngine.hpp>

//console.log

//#####################################################
//     == AI ACTION DISTANCE SYSTEM [OFFENSIVE] ==
//  - Get All entities whith DISTANCE Component
//  - Create an attack in its position
//
//   Using Componets:
//  - Situation Component       (read)
//  - Attack Component          (read - write)
//  - DistanceWeapon Component  (read)
//#####################################################

AIActionDistanceSystem::AIActionDistanceSystem() {}

AIActionDistanceSystem::~AIActionDistanceSystem() {}

void AIActionDistanceSystem::update(GameEngine &gameContext) const {


    auto &allDistanceActions {gameContext.entityMan.getComponents<AIActionDistanceComponent>()};

    for(auto &distanceComp : allDistanceActions) {
        uint32_t entityId           {distanceComp.id};
        auto    &entitySituation    {gameContext.entityMan.getComponent<SituationComponent>         (entityId)};
        auto    &entityStats        {gameContext.entityMan.getComponent<StatsComponent>             (entityId)};
        auto    &distanceWeapon     {gameContext.entityMan.getComponent<DistanceWeaponComponent>    (entityId)};
        auto    &aiOffensive        {gameContext.entityMan.getComponent<AIOffensiveComponent>       (entityId)};
        
        // TODO: Check this
        if (aiOffensive.targetId) {
            auto    &targetSituation    {gameContext.entityMan.getComponent<SituationComponent>(aiOffensive.targetId)};




            float angle = Utils::angleToTarget(targetSituation, entitySituation);
            angle = Utils::radToDeg(angle);

            distanceWeapon.coolDown -= gameContext.getDeltaTime();
            if (distanceWeapon.coolDown < 0)
                distanceWeapon.coolDown = 0;

            AIfunctions::turnToTarget(gameContext, angle, entitySituation);
            if(distanceWeapon.coolDown <= 0) {
                Utils::distanceAttack(gameContext, distanceWeapon, entitySituation, entityStats, entityId);
                distanceWeapon.coolDown = distanceWeapon.maxCoolDown;
            }
        }



    }

}