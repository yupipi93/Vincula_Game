#pragma once

#include "WeaponComponent.hpp"

class MeleeWeaponComponent : public WeaponComponent {
public:
    explicit MeleeWeaponComponent(const uint32_t id);
    ~MeleeWeaponComponent() override;
};
