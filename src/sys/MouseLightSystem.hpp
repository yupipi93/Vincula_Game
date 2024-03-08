#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class MouseLightSystem : public System {
public:
    MouseLightSystem();
    ~MouseLightSystem();

    void update(GameEngine &gameContext) const override;
private:




};