#include "Entity.hpp"

static uint32_t currentId = 1;

Entity::Entity(EntityType t, GameObjectType goT):
    id {currentId++}, type {t}, goType {goT} {}

Entity::~Entity() {}

const uint32_t Entity::getId() const {
    return id;
}


EntityType Entity::getType() const {
    return type;
}

GameObjectType Entity::getGameObjectType() const {
    return goType;
}

uint32_t Entity::getCurrentId() {
    return currentId;
}