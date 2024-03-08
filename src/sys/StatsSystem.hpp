#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class StatsSystem : public System {
public:
    StatsSystem();
    ~StatsSystem();

    void update(GameEngine &gameContext) const override;
private:
    void refreshPlayerHeart(GameEngine &gameContext) const;

    bool isPlayerRecivedDamage(GameEngine &gameContext) const;
    void subtractDamageReceivedToCurrentLife(GameEngine &gameContext) const;
};