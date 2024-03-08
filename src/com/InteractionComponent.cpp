#include "InteractionComponent.hpp"

InteractionComponent::InteractionComponent(const uint32_t id)
    : Component(id) {}

InteractionComponent::~InteractionComponent() {}

void InteractionComponent::resetActions() {
    openingDoor				= false;
    weaponIdInteracting 	= 0;
    coinChestIdInteracting 	= 0;
    keyInteracting      	= 0;
    dialogueInteracting   	= 0;
    palantirInteracting   	= 0;
    
    entityInteractibleId    = 0;
}
