#include "WeaponComponent.hpp"

WeaponComponent::WeaponComponent(const uint32_t id)
    : Component(id) {
    	defaultPosition.reserve(3);
    }

WeaponComponent::~WeaponComponent() {}
