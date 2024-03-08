#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class InfoActionSystem : public System {
public:
    InfoActionSystem();
    ~InfoActionSystem();

    void update(GameEngine &gameContext) const override;
    
};