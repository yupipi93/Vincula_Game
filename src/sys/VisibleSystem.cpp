#include "VisibleSystem.hpp"
#include <tools/Utils.hpp>
//console.log
#include <string>



const float WALL_DISTANCE_ADD = 300.f;

//#####################################################
//               == VISIBLE SYSTEM ==
//  - ????????????????????
//  - ????????????????????
//  - ????????????????????
//
//   Using Componets:
//      - ??????    (Read - Write)
//      - ??????    (Read - Write)
//      - ??????    (Write)
//
//#####################################################

VisibleSystem::VisibleSystem() {}

VisibleSystem::~VisibleSystem() {}

void VisibleSystem::update(GameEngine &gameContext) const {
    float limitLeft;
    float limitRigth;
    float limitTop;
    float limitBottom;

    Utils::calculateVisibleZoneLimits(limitLeft, limitRigth, limitTop, limitBottom, gameContext);

    auto &obstacles = gameContext.entityMan.getComponents<ObstacleComponent>();
    refreshDrawables(gameContext, obstacles, limitLeft, limitRigth, limitTop, limitBottom);

    auto &enemies = gameContext.entityMan.getComponents<AIComponent>();
    refreshDrawables(gameContext, enemies, limitLeft, limitRigth, limitTop, limitBottom);

    auto &walls = gameContext.entityMan.getComponents<WallComponent>();
    refreshDrawables(gameContext, walls, limitLeft - WALL_DISTANCE_ADD, limitRigth + WALL_DISTANCE_ADD, limitTop + WALL_DISTANCE_ADD, limitBottom - WALL_DISTANCE_ADD);

    auto &meleeWeapons = gameContext.entityMan.getComponents<MeleeWeaponComponent>();
    refreshDrawables(gameContext, meleeWeapons, limitLeft, limitRigth, limitTop, limitBottom);

    auto &distanceWeapons = gameContext.entityMan.getComponents<DistanceWeaponComponent>();
    refreshDrawables(gameContext, distanceWeapons, limitLeft, limitRigth, limitTop, limitBottom);

    auto &eggDatas = gameContext.entityMan.getComponents<EggDataComponent>();
    refreshDrawables(gameContext, eggDatas, limitLeft, limitRigth, limitTop, limitBottom);

    auto &IPDatas = gameContext.entityMan.getComponents<IPDataComponent>();
    refreshDrawables(gameContext, IPDatas, limitLeft, limitRigth, limitTop, limitBottom);

    auto &coins = gameContext.entityMan.getComponents<CoinComponent>();
    refreshDrawables(gameContext, coins, limitLeft, limitRigth, limitTop, limitBottom);

    auto &keys = gameContext.entityMan.getComponents<KeyComponent>();
    refreshDrawables(gameContext, keys, limitLeft, limitRigth, limitTop, limitBottom);

    auto &chests = gameContext.entityMan.getComponents<ChestComponent>();
    refreshDrawables(gameContext, chests, limitLeft, limitRigth, limitTop, limitBottom);

    //Optimize animations with this off 
    auto &npcDialogues = gameContext.entityMan.getComponents<DialogueComponent>();
    refreshDrawables(gameContext, npcDialogues, limitLeft, limitRigth, limitTop, limitBottom);

    auto &attacks = gameContext.entityMan.getComponents<AttackComponent>();
    refreshDrawables(gameContext, attacks, limitLeft, limitRigth, limitTop, limitBottom);

    auto &messageBoxes = gameContext.entityMan.getComponents<MessageBoxComponent>();
    refreshDrawables(gameContext, messageBoxes, limitLeft, limitRigth, limitTop, limitBottom);

    auto &iPPart = gameContext.entityMan.getComponents<IPPartComponent>();
    refreshDrawables(gameContext, iPPart, limitLeft, limitRigth, limitTop, limitBottom);

    auto &secureZone = gameContext.entityMan.getComponents<SecureZoneComponent>();
    refreshDrawables(gameContext, secureZone, limitLeft, limitRigth, limitTop, limitBottom);

    auto &detectionSigns = gameContext.entityMan.getComponents<DetectionSignComponent>();
    refreshDrawables(gameContext, detectionSigns, limitLeft, limitRigth, limitTop, limitBottom);

    std::vector<uint32_t> villageIds = gameContext.villageIds;
    for (uint32_t id : villageIds)
        refreshDrawable(gameContext, id, limitLeft, limitRigth, limitTop, limitBottom);

    uint32_t sourceId = gameContext.sourceId;
    refreshDrawable(gameContext, sourceId, limitLeft, limitRigth, limitTop, limitBottom);

    uint32_t bankId = gameContext.bankId;
    refreshDrawable(gameContext, bankId, limitLeft, limitRigth, limitTop, limitBottom);

    uint32_t bankDepositorId = gameContext.bankDepositorId;
    refreshDrawable(gameContext, bankDepositorId, limitLeft, limitRigth, limitTop, limitBottom);

    uint32_t bankExtractorId = gameContext.bankExtractorId;
    refreshDrawable(gameContext, bankExtractorId, limitLeft, limitRigth, limitTop, limitBottom);

    uint32_t doorId = gameContext.doorId;
    refreshDrawable(gameContext, doorId, limitLeft - WALL_DISTANCE_ADD, limitRigth + WALL_DISTANCE_ADD, limitTop + WALL_DISTANCE_ADD, limitBottom - WALL_DISTANCE_ADD);

    uint32_t controlsFloorId = gameContext.controlsFloorId;
    refreshDrawable(gameContext, controlsFloorId, limitLeft, limitRigth, limitTop, limitBottom);

}

template <typename T>
void VisibleSystem::refreshDrawables(GameEngine &gameContext, Storage<T> &storage, float limitLeft, float limitRigth, float limitTop, float limitBottom) const {
    for (auto &component : storage) {
        uint32_t entityId = component.id;

        if(gameContext.entityMan.getEntity(entityId).getType() == ATTACK) {
            AttackComponent &attack = gameContext.entityMan.getComponent<AttackComponent>(entityId);
            if(attack.attackType == ATTACKDISTANCE) {
                refreshDrawable(gameContext, entityId, limitLeft, limitRigth, limitTop, limitBottom);
            }
        } else {
            refreshDrawable(gameContext, entityId, limitLeft, limitRigth, limitTop, limitBottom);
        }
    }
}

void VisibleSystem::refreshDrawable(GameEngine &gameContext, uint32_t entityId, float limitLeft, float limitRigth, float limitTop, float limitBottom) const {
    SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(entityId);
    DrawableComponent  &drawable  = gameContext.entityMan.getComponent<DrawableComponent>(entityId);

    bool outOfLimits = Utils::checkSituationIsOutsideLimits(situation, limitLeft, limitRigth, limitTop, limitBottom);

    if(outOfLimits || !drawable.mustBeDrawn) {
        gameContext.getWindowFacadeRef().eraseEntity(entityId);
        
    } else if(!outOfLimits && drawable.mustBeDrawn) {
        if(!gameContext.getWindowFacadeRef().existsNode(entityId)) {
            if(gameContext.entityMan.getEntity(entityId).getType() == CONTROLSWALL || gameContext.entityMan.getEntity(entityId).getType() == MESSAGEBOX) {
                gameContext.getWindowFacadeRef().createPlaneEntity(gameContext, entityId, "plane");
            } else {
                gameContext.getWindowFacadeRef().createEntity(gameContext, entityId);
            }
        }
    }
}

