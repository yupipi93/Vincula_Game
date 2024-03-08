#pragma once

#include "Component.hpp"
#include <enum/WeaponType.hpp>
#include <enum/ChestType.hpp>
#include <string>
#include <cstdint>

class ChestComponent : public Component {
public:
    explicit ChestComponent(const uint32_t id);
    ~ChestComponent() override;

    ChestType   chestType 		      	{COINCHEST};
    bool 		opened        	      	{false};

    uint32_t	interrogation			{0};

    uint16_t	coins		      		{0};
    WeaponType	weaponType 		      	{UNKNOWN};

    std::string	modelOpenedChest    	{""};
};