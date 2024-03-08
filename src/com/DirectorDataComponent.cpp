#include "DirectorDataComponent.hpp"

DirectorDataComponent::DirectorDataComponent(const uint32_t id)
    : Component(id) {}

DirectorDataComponent::~DirectorDataComponent() {}

void DirectorDataComponent::addSpawn(const float timer, const uint8_t max) {
    spawnTimer.emplace_back(timer);
    spawnCurrentTimer.emplace_back(0);
    enemiesSpawned.emplace_back(0);
    maxEnemiesSpawned.emplace_back(max);
}

void DirectorDataComponent::resetEnemies() {
    for (auto &enemies : enemiesSpawned)
        enemies = 0;
}
