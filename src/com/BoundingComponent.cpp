#include "BoundingComponent.hpp"
#include <algorithm>


BoundingComponent::BoundingComponent(const uint32_t id)
    : Component(id) {}

BoundingComponent::~BoundingComponent() {}

void BoundingComponent::removeEntityColliding(const uint32_t id) {
    entitiesColliding.erase(std::remove(entitiesColliding.begin(), entitiesColliding.end(), id), entitiesColliding.end());
}
