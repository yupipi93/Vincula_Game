#include "AIActionMeleSystem.hpp"

//#include <math.h>
#include <tools/AIfunctions.hpp>
#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>


//#####################################################
//     == AI ACTION MELE SYSTEM [OFFENSIVE] ==
//  - Get All entities whith MELE Component
//  - Create an attack in its position
//
//   Using Componets:
//  - Situation Component      (read)
//  - Attack Component         (read - write)
//  - MeleeWeapon Component    (read)
//#####################################################

AIActionMeleSystem::AIActionMeleSystem() {}

AIActionMeleSystem::~AIActionMeleSystem() {}


void AIActionMeleSystem::update(GameEngine &gameContext) const {

    auto &meleeActions {gameContext.entityMan.getComponents<AIActionMeleeComponent>()};

    for(auto &meleeComp : meleeActions) {
        uint32_t entityId   = meleeComp.id;
        SituationComponent       &entitySituation   = gameContext.entityMan.getComponent<SituationComponent>(entityId);
        StatsComponent           &entityStats       = gameContext.entityMan.getComponent<StatsComponent>(entityId);
        MeleeWeaponComponent     &meleeWeapon       = gameContext.entityMan.getComponent<MeleeWeaponComponent>(entityId);
        auto    &aiOffensive        {gameContext.entityMan.getComponent<AIOffensiveComponent>       (entityId)};


        if (aiOffensive.targetId) {

            auto    &targetSituation    {gameContext.entityMan.getComponent<SituationComponent>(aiOffensive.targetId)};

            float angle = Utils::angleToTarget(targetSituation, entitySituation);
            angle = Utils::radToDeg(angle);

            meleeWeapon.coolDown -= gameContext.getDeltaTime();

            AIfunctions::turnToTarget(gameContext, angle, entitySituation);
            if(meleeWeapon.coolDown <= 0) {
                Utils::meleeAttack(gameContext, meleeWeapon, entitySituation, entityStats, entityId);
                meleeWeapon.coolDown = meleeWeapon.maxCoolDown;
            }
        }
    }

}
