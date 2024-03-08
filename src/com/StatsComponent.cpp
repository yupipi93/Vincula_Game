#include "StatsComponent.hpp"

StatsComponent::StatsComponent(const uint32_t id)
    : Component(id) {}

StatsComponent::~StatsComponent() {}

void StatsComponent::resetHealth() {
    currentHealth = maxHealth;
}
