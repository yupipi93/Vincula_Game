#include "InputComponent.hpp"

InputComponent::InputComponent(const uint32_t id)
    : Component(id) {}

InputComponent::~InputComponent() {}

void InputComponent::resetActions() {
    attacking           = false;
    setingTarget        = false;
    dancing             = false;
    interacting         = false;
    dashing             = false;
    changingWeapon      = false;
    callingCompass      = false;
    storageCoin         = false;
    obtainCoin          = false;
    pressingW           = false;
    pressingS           = false;
    pressingA           = false;
    pressingD           = false;
    controlClickLeft    = false;
    controlClickMiddle  = false;
    controlEspKeyPress  = false;
    dropingPalantir     = false;
}

void InputComponent::resetMenuActions() {
    controlUpKeyPress     = false;
    controlDownKeyPress   = false;
    controlReturnKeyPress = false;
    controlEscKeyPress    = false;
}

void InputComponent::subDeltaTime(const float deltaTime) {
    interactingTimer -= deltaTime;
    if (interactingTimer < 0)
        interactingTimer = 0;
    
    changingWeaponTimer -= deltaTime;
    if (changingWeaponTimer < 0)
        changingWeaponTimer = 0;

    callingCompassTimer -= deltaTime;
    if (callingCompassTimer < 0)
        callingCompassTimer = 0;
}