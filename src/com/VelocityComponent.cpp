#include "VelocityComponent.hpp"

VelocityComponent::VelocityComponent(const uint32_t id)
    : Component(id) {}

VelocityComponent::~VelocityComponent() {}

void VelocityComponent::resetAllVelocities() {
    velocityX = 0.f;
    velocityY = 0.f;
    velocityZ = 0.f;
}
