#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class AttackSystem : public System {
public:
    AttackSystem();
    ~AttackSystem();

    void update(GameEngine &gameContext) const override;

private:
    void checkChangingWeapon(GameEngine &gameContext, InputComponent &playerInput, InventoryComponent &playerInventory) const;
    void changeWeapon(GameEngine &gameContext, InventoryComponent &playerInventory) const;

    void checkAttacking(GameEngine &gameContext, InputComponent &playerInput, InventoryComponent &playerInventory, DashComponent &playerDash, uint32_t playerId) const;
    void checkWeaponType(GameEngine &gameContext, InventoryComponent &playerInventory,  InputComponent &playerInput, uint32_t playerID) const;

    void checkAttackAnimation(GameEngine &gameContext, uint32_t primaryWeaponId, uint32_t playerId) const;
    void updateAttackAnimation(GameEngine &gameContext, WeaponType &weaponType, uint32_t playerId) const;

    void removeAttacksByLifeTime(GameEngine &gameContext) const;
    void removeAttacksColliding(GameEngine &gameContext) const;
    bool entityAlreadyDamagedByAttack(AttackComponent &attack, uint32_t entColl) const;
    void attackHitSomeEntity(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const;
    void attackHitEntityEgg(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const;
    uint16_t addDamageRecived(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const;
    void attackHitEntityLived(GameEngine &gameContext, AttackComponent &attack, uint32_t entColl) const;
    void addEffectToEntity(GameEngine &gameContext, EffectType effectType, uint32_t idHitten) const;
    bool attackHasToDrill(GameEngine &gameContext, uint32_t idEntColl, CollisionComponent &collisionAttack) const;

    void addAttacksToUpdatePool(GameEngine &gameContext) const;
};