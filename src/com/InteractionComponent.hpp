#pragma once

#include "Component.hpp"

class InteractionComponent : public Component {
public:
    explicit InteractionComponent(const uint32_t id);
    ~InteractionComponent() override;

    void resetActions();

    bool		openingDoor				{false};

    uint32_t	weaponIdInteracting		{0};
    uint32_t	coinChestIdInteracting	{0};
    uint32_t	keyInteracting			{0};
    uint32_t    dialogueInteracting	    {0};
    uint32_t 	palantirInteracting		{0};

    uint32_t	entityInteractibleId    {0}; 
};