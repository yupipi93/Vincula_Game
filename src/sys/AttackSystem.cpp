#include "AttackSystem.hpp"

#include <math.h>
#include <tools/Utils.hpp>
//console.log

#include <eng/GameEngine.hpp>

//#####################################################
//                == ATTACK SYSTEM ==
//  - Delete all Attacks_Entities
//  - Create an Attack_Entity if 'isAttacking' is true
//
//   Using Componets:
//      - Input (Read)
//#####################################################

AttackSystem::AttackSystem() {}

AttackSystem::~AttackSystem() {}

void AttackSystem::update(GameEngine &gameContext) const {
    uint32_t playerID = gameContext.playerId;
    InputComponent     &playerInput     = gameContext.entityMan.getComponent<InputComponent>(playerID);
    InventoryComponent &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(playerID);
    DashComponent      &playerDash      = gameContext.entityMan.getComponent<DashComponent>(playerID);

    removeAttacksColliding(gameContext);
    removeAttacksByLifeTime(gameContext);

    checkChangingWeapon(gameContext, playerInput, playerInventory);
    checkAttacking(gameContext, playerInput, playerInventory, playerDash, playerID);

    addAttacksToUpdatePool(gameContext);
}


void AttackSystem::checkChangingWeapon(GameEngine &gameContext, InputComponent &playerInput, InventoryComponent &playerInventory) const {
    if(playerInput.changingWeapon && playerInventory.secondaryWeaponId != 0) {
        changeWeapon(gameContext, playerInventory);
    }
}

void AttackSystem::changeWeapon(GameEngine &gameContext, InventoryComponent &playerInventory) const {
    uint32_t idPrimary   = playerInventory.primaryWeaponId;
    uint32_t idSecondary = playerInventory.secondaryWeaponId;
    playerInventory.primaryWeaponId   = idSecondary;
    playerInventory.secondaryWeaponId = idPrimary;

    gameContext.getWindowFacadeRef().updateWeapons2DTexture(gameContext);
}


void AttackSystem::checkAttacking(GameEngine &gameContext, InputComponent &playerInput, InventoryComponent &playerInventory, DashComponent &playerDash, uint32_t playerId) const {
    bool paralyzed = gameContext.entityMan.existsComponent<EffectParalysisComponent>(playerId);
    
    if(!paralyzed && !playerInput.doingAttack && !playerDash.dashing) {
        if (playerInput.attacking && playerInventory.primaryWeaponId != 0) {
            playerInput.doingAttack = true;
            playerInput.doingDance  = false;
            playerInput.boring      = false;
            playerInput.boredTime   = 0;
            checkAttackAnimation(gameContext, playerInventory.primaryWeaponId, playerId);
        }
    }

    if(playerInput.doingAttack) {
        playerInput.attackingTimer += gameContext.getDeltaTime();
        checkWeaponType(gameContext, playerInventory, playerInput, playerId);
    }
}

void AttackSystem::checkAttackAnimation(GameEngine &gameContext, uint32_t primaryWeaponId, uint32_t playerId) const {
    auto typeWeapon = gameContext.getEntity(primaryWeaponId).getType();
    if(typeWeapon == MELEEWEAPON) {
        MeleeWeaponComponent &meleeWeapon = gameContext.entityMan.getComponent<MeleeWeaponComponent>(primaryWeaponId);
        updateAttackAnimation(gameContext, meleeWeapon.weapon, playerId);
    } else if(typeWeapon == DISTANCEWEAPON) {
        DistanceWeaponComponent &distanceWeapon = gameContext.entityMan.getComponent<DistanceWeaponComponent>(primaryWeaponId);
        updateAttackAnimation(gameContext, distanceWeapon.weapon, playerId);
    }
}

void AttackSystem::updateAttackAnimation(GameEngine &gameContext, WeaponType &weaponType, uint32_t playerId) const {
    DrawableComponent &playerDrawable = gameContext.entityMan.getComponent<DrawableComponent>(playerId);

    if(weaponType == STICK) {
        playerDrawable.model = "./media/player/Animations/attackStick/attackStick";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(weaponType == SWORD) {
        playerDrawable.model = "./media/player/Animations/attackSword/attackSword";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(weaponType == MACE) {
        playerDrawable.model = "./media/player/Animations/attackMace/attackMace";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(weaponType == SLINGSHOT) {
        playerDrawable.model = "./media/player/Animations/shotSlingshot/shotSlingshot";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(weaponType == BLOWGUN) {
        playerDrawable.model = "./media/player/Animations/shotBlowgun/shotBlowgun";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    } else if(weaponType == BOW) {
        playerDrawable.model = "./media/player/Animations/shotBow/shotBow";
        gameContext.getWindowFacadeRef().updateModel(gameContext, playerId);
        gameContext.getWindowFacadeRef().updateTimeFrame(gameContext, playerId, 1.f / 30.f);
    }
}


void AttackSystem::checkWeaponType(GameEngine &gameContext, InventoryComponent &playerInventory, InputComponent &playerInput, uint32_t playerID) const {
    StatsComponent &playerStats = gameContext.entityMan.getComponent<StatsComponent>(playerID);
    SituationComponent &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(playerID);
    auto typeWeapon = gameContext.getEntity(playerInventory.primaryWeaponId).getType();

    if(typeWeapon == MELEEWEAPON) {
        MeleeWeaponComponent &meleeWeapon = gameContext.entityMan.getComponent<MeleeWeaponComponent>(playerInventory.primaryWeaponId);

        meleeWeapon.coolDown -= gameContext.getDeltaTime();

        if(playerInput.attackingTimer >= meleeWeapon.attackApparitionTime && !playerInput.attacked) {
            Utils::meleeAttack(gameContext, meleeWeapon, playerSituation, playerStats, playerID);
            playerInput.attacked = true;
        }
        if(playerInput.attackingTimer >= meleeWeapon.maxCoolDown) {
            playerInput.attacked       = false;
            playerInput.doingAttack    = false;
            playerInput.attackingTimer = 0.f;
        }

        if (meleeWeapon.coolDown <= 0)
            meleeWeapon.coolDown = meleeWeapon.maxCoolDown;
    } else if(typeWeapon == DISTANCEWEAPON) {
        DistanceWeaponComponent &distanceWeapon = gameContext.entityMan.getComponent<DistanceWeaponComponent>(playerInventory.primaryWeaponId);

        distanceWeapon.coolDown -= gameContext.getDeltaTime();

        if(playerInput.attackingTimer >= distanceWeapon.attackApparitionTime && !playerInput.attacked) {
            Utils::distanceAttack(gameContext, distanceWeapon, playerSituation, playerStats, playerID);
            playerInput.attacked = true;
        }
        if(playerInput.attackingTimer >= distanceWeapon.maxCoolDown) {
            playerInput.attacked       = false;
            playerInput.doingAttack = false;
            playerInput.attackingTimer = 0.f;
        }

        if (distanceWeapon.coolDown <= 0)
            distanceWeapon.coolDown = distanceWeapon.maxCoolDown;
    }
}

void AttackSystem::addAttacksToUpdatePool(GameEngine &gameContext) const {
    auto &allAttacks = gameContext.entityMan.getComponents<AttackComponent>();
    for(auto &attack : allAttacks) {
        gameContext.addEntityToUpdate(attack.id);
    }
}

void AttackSystem::removeAttacksByLifeTime(GameEngine &gameContext) const {
    auto &allAttacks = gameContext.entityMan.getComponents<AttackComponent>();
    std::vector<uint32_t> attacksIdToDelete;

    for(auto &attack : allAttacks) {
        attack.lifeTime -= gameContext.getDeltaTime();
        if(attack.lifeTime <= 0) {
            attacksIdToDelete.emplace_back(attack.id);
        }
    }

    for (size_t i = 0; i < attacksIdToDelete.size(); ++i) {
        gameContext.eraseEntityByID(attacksIdToDelete[i]);
    }
}

void AttackSystem::removeAttacksColliding(GameEngine &gameContext) const {

    auto &allAttacks = gameContext.entityMan.getComponents<AttackComponent>();
    std::vector<uint32_t> attacksIdToDelete;

    for(auto &attack : allAttacks) { //Recorremos todos los ataques
        CollisionComponent &attackCol = gameContext.entityMan.getComponent<CollisionComponent>(attack.id);
        auto entitiesColliding = attackCol.entitiesColliding;

        for(uint32_t entCollId : entitiesColliding) { //Recorremos todas las entidades que chocan con el attack

            if(attack.attackerId != entCollId) { //Si el ataque NO choca con quien lo ha creado:

                attackHitSomeEntity(gameContext, attack, entCollId);

                if(!attackHasToDrill(gameContext, entCollId, attackCol)) { //Si el Attack NO tiene que perforar, lo eliminamos
                    attacksIdToDelete.emplace_back(attack.id);
                }
            }
        }
    }

    for (size_t i = 0; i < attacksIdToDelete.size(); ++i) {
        gameContext.eraseEntityByID(attacksIdToDelete[i]);
    }
}

void AttackSystem::attackHitSomeEntity(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const {
    EntityType hittenType = gameContext.getEntity(entColl).getType();
    if(!entityAlreadyDamagedByAttack(attack, entColl)) { //Si la entidad golpeada NO ha sido golpeada ya por este attack:
        if(hittenType == ENEMY || hittenType == PLAYER) {
            attackHitEntityLived(gameContext, attack, entColl);
        } else if(hittenType == KEYEGG) {
            attackHitEntityEgg(gameContext, attack, entColl);
        }
    }
}

void AttackSystem::attackHitEntityLived(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const {
    auto damageReceived = addDamageRecived(gameContext, attack, entColl);

    if (attack.effectType != WINHEART)  {
        if (gameContext.entityMan.existsComponent<EffectWinHeartComponent>(entColl))
            gameContext.entityMan.getComponent<EffectWinHeartComponent>(entColl).active = false;
    }

    if (attack.effectType == HEALTHSTEAL) {
        auto &stats = gameContext.entityMan.getComponent<StatsComponent>(attack.attackerId);
        stats.healing = true;
        stats.damageHealing = damageReceived * 0.5; // Percent of life stealth
    } else if(attack.effectType != NOEFFECT) {
        addEffectToEntity(gameContext, attack.effectType, entColl);
    }
}

void AttackSystem::attackHitEntityEgg(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const {
    EggDataComponent &eggdata = gameContext.entityMan.getComponent<EggDataComponent>(entColl);
    if(!eggdata.broken && attack.attackerId == gameContext.playerId) {
        addDamageRecived(gameContext, attack, entColl);
    }

    //PREVIOUS IMPLEMENTATION: Egg only gets broken with the weapon of same biome
    /*
    EggDataComponent &eggdata = gameContext.entityMan.getComponent<EggDataComponent>(entColl);
    if(attack.biome == eggdata.biome) {
        if(!eggdata.broken) {
            addDamageRecived(gameContext, attack, entColl);
        }
    } else {
        SituationComponent &eggSituation = gameContext.entityMan.getComponent<SituationComponent>(entColl);
        gameContext.entityMan.createNotification(gameContext, eggSituation.x, eggSituation.y + 40, eggSituation.z, 0, NOTIFICATION_FAILEGG);
    }*/
}

uint16_t AttackSystem::addDamageRecived(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const {
    StatsComponent &stats = gameContext.entityMan.getComponent<StatsComponent>(entColl);
    stats.damageReceived += attack.damage;
    stats.isCritic = attack.isCritic;
    stats.damaged = true;
    stats.damagedTime = 0;

    //MIN DAMAGE 1
    if(stats.damageReceived == 0) {
        stats.damageReceived = 1;
    }

    bool isEnemy = gameContext.entityMan.getComponents<AIComponent>().exists(entColl);
    if (isEnemy) {
        auto &aiComp = gameContext.entityMan.getComponent<AIComponent>(entColl);
        aiComp.currentAlertState = aiComp.defaultAlertState;
        if(attack.attackerId != 0) {
            aiComp.aggredBy = attack.attackerId;
        }


        // Make enemies attacks you no matter how far you are
        aiComp.forceCombat = true;
    }

    attack.entitiesAlreadyDamaged.push_back(entColl);


    return stats.damageReceived;

}

bool AttackSystem::entityAlreadyDamagedByAttack(AttackComponent &attack, uint32_t entColl) const {
    bool alreadyDamaged = false;
    for(uint32_t idEnt : attack.entitiesAlreadyDamaged) {
        if(entColl == idEnt) //Si ya ha chocado
            alreadyDamaged = true;
    }

    return alreadyDamaged;
}

void AttackSystem::addEffectToEntity(GameEngine &gameContext, EffectType effectType, uint32_t idHitten) const {
    if(effectType == POISON) {
        gameContext.entityMan.eraseComponent<EffectPoisonComponent>(idHitten);
        gameContext.entityMan.createComponent<EffectPoisonComponent>(idHitten);

    } else if(effectType == PARALYSIS) {
        gameContext.entityMan.eraseComponent<EffectParalysisComponent>(idHitten);
        gameContext.entityMan.createComponent<EffectParalysisComponent>(idHitten);

    } else if(effectType == SLOWDOWN) {
        gameContext.entityMan.eraseComponent<EffectSlowdownComponent>(idHitten);
        gameContext.entityMan.createComponent<EffectSlowdownComponent>(idHitten);

    } else if(effectType == WINHEART) {
        if(gameContext.getEntity(idHitten).getType() == ENEMY && gameContext.getEntity(idHitten).getGameObjectType() != ENEMY_GHOST) {
            gameContext.entityMan.eraseComponent<EffectWinHeartComponent>(idHitten);
            gameContext.entityMan.createComponent<EffectWinHeartComponent>(idHitten);
        }
    }
}

bool AttackSystem::attackHasToDrill(GameEngine &gameContext, uint32_t idEntColl, CollisionComponent &collisionAttack) const {
    //Drill = Perforar
    bool hasToDrill = true;
    CollisionComponent &collisionHitten = gameContext.entityMan.getComponent<CollisionComponent>(idEntColl);

    if(collisionAttack.type == KINEMATICMELEE && collisionHitten.type == STATIC) {  //TODO: Sound Effect: Broken WeaponMelee
        hasToDrill = false;
    } else if(collisionAttack.type == KINEMATIC && (collisionHitten.type == STATIC || collisionHitten.type == DYNAMIC || collisionHitten.type == KINEMATICMELEE)) { //Attack kinematico solo atraviesa dinamicos
        hasToDrill = false;
    }

    return hasToDrill;
}





