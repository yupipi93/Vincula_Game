#pragma once

#include "WeaponComponent.hpp"
#include <enum/CollisionType.hpp>
#include <string>

class DistanceWeaponComponent : public WeaponComponent {
public:
    explicit DistanceWeaponComponent(const uint32_t id);
    ~DistanceWeaponComponent() override;

    std::string		bulletModel			{""};
    std::string		bulletEffectModel	{""};
    float			bulletSpeed			{300};
    float			bulletLifeTime		{5};
    CollisionType	collisionType		{STATIC};
    bool 			rotative 			{false};

};