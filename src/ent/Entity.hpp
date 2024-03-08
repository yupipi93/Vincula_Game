#pragma once

#include <enum/EntityType.hpp>
#include <enum/GameObjectType.hpp>
#include <stdint.h>

class Entity {
public:
    Entity(EntityType t, GameObjectType goT = NONE);
   // Entity(Entity& e) = delete;
    ~Entity();

    const uint32_t getId() const;
    EntityType getType() const;
    GameObjectType getGameObjectType() const;
    static uint32_t getCurrentId();

private:
    uint32_t id;
    EntityType type;
    GameObjectType goType;
};