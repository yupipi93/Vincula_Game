#include "AIActionLungeSystem.hpp"
#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>
#include <tools/AIfunctions.hpp>

//#####################################################
//     == AI ACTION LUNGE SYSTEM [OFFENSIVE] ==
//  - Get All entities whith Lunge Component
//  - Get holstile entity id from Offensive Componet
//  - Set Target in MoveTo Component
//
//   Using Componets:
//      - AiLunge           (Read)
//      - MoveTo            (Read - Write)
//      - AiOffensive       (Read)
//      - Situation         (Read)
//      - Waypoint          (Create)
//#####################################################


AIActionLungeSystem::AIActionLungeSystem() {
}

AIActionLungeSystem::~AIActionLungeSystem() {

}

const uint8_t LUNGE_SIZE = 90;


void AIActionLungeSystem::update(GameEngine &gameContext) const {
    auto &allLunges {gameContext.entityMan.getComponents<AIActionLungeComponent>()};

    for(auto &lungeComp : allLunges) {
        uint32_t entityId       {lungeComp.id};
        auto &aiOffensive       {gameContext.entityMan.getComponent<AIOffensiveComponent>(entityId)};
        auto &sitComp           {gameContext.entityMan.getComponent<SituationComponent>(entityId)};
        auto &moveToComp        {gameContext.entityMan.getComponent<AIMoveToComponent>(entityId)};
        auto &entityStats       {gameContext.entityMan.getComponent<StatsComponent>(entityId)};
        auto &meleeWeapon       {gameContext.entityMan.getComponent<MeleeWeaponComponent>(entityId)};
        auto &velComp           {gameContext.entityMan.getComponent<VelocityComponent>(entityId)};

        if(moveToComp.targetId) {
            aiOffensive.inAction = true;
            velComp.speed = velComp.speedUpped;

            if (!lungeComp.attackId) {
                lungeComp.attackId = Utils::meleeAttack(gameContext, meleeWeapon, sitComp, entityStats, entityId, 100);
            } else {
                if (gameContext.entityMan.existsComponent<SituationComponent>(lungeComp.attackId)) {
                    auto &attackSit = gameContext.entityMan.getComponent<SituationComponent>(lungeComp.attackId);
                    float rot          = sitComp.rotation;
                    float auxRot       = Utils::degToRad(rot);
                    float posX         = sitComp.x + 1 * cos(auxRot);
                    float posZ         = sitComp.z + 1 * sin(auxRot);


                    attackSit.x = posX;
                    attackSit.z = posZ;
                } else {
                    //aiOffensive.inAction = false;
                    //velComp.speed = velComp.speedNormal;
                    moveToComp.targetId = 0;
                }
            }


        } else {
            lungeComp.tempToAtack -= gameContext.getDeltaTime();

            aiOffensive.inAction = false;

            if (gameContext.entityMan.existsComponent<SituationComponent>(aiOffensive.targetId)) {
                auto &targetSituation = gameContext.entityMan.getComponent<SituationComponent>(aiOffensive.targetId);
                float angle = Utils::angleToTarget(targetSituation, sitComp);
                angle = Utils::radToDeg(angle);
                AIfunctions::turnToTarget(gameContext, angle, sitComp);
            }

            if (lungeComp.attackId) {
                if (gameContext.entityMan.existsComponent<AttackComponent>(lungeComp.attackId)) {
                    auto &attack = gameContext.entityMan.getComponent<AttackComponent>(lungeComp.attackId);
                    attack.lifeTime = 0;
                }
                lungeComp.attackId = 0;
            }

            if(lungeComp.tempToAtack <= 0) {
                velComp.speed = velComp.speedNormal;
                if (aiOffensive.targetId)
                    setTarget(gameContext, sitComp, moveToComp, aiOffensive.targetId);
                lungeComp.tempToAtack = lungeComp.maxTempToAtack;

            }
        }
    }
}


void AIActionLungeSystem::setTarget(GameEngine &gameContext, SituationComponent &sit, AIMoveToComponent &moveToComp, uint32_t targetId) const {
    uint32_t entityId {moveToComp.id};
    auto &targetSitComp     {gameContext.entityMan.getComponent<SituationComponent>(targetId)};
    float angle = Utils::angleToTarget(sit, targetSitComp);
    float x = targetSitComp.x + cos(angle) * LUNGE_SIZE;
    float z = targetSitComp.z + sin(angle) * LUNGE_SIZE;

    moveToComp.targetId = AIfunctions::createTarget(gameContext,  entityId,  x,  z);
}






