#include "DashComponent.hpp"

DashComponent::DashComponent(const uint32_t id)
    : Component(id) {}

DashComponent::~DashComponent() {}

void DashComponent::activateDash() {
    dashing = true;
    timerDash = dashMaxTime;
}

void DashComponent::deactivateDash() {
    dashing = false;
    timerCoolDown = maxCoolDown;
}
