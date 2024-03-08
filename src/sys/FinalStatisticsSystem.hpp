#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class FinalStatisticsSystem : public System {
public:
    FinalStatisticsSystem();
    ~FinalStatisticsSystem();

    void update(GameEngine &gameContext) const override;
};