#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InteractionSystem : public System {
public:
    InteractionSystem();
    ~InteractionSystem();

    void update(GameEngine &gameContext) const override;
private:
    void checkInteractions(GameEngine &gameContext, uint32_t &playerId, InteractionComponent &playerInteraction) const;

};
