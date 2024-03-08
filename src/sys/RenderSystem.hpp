#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem();

    void update(GameEngine &gameContext) const override;

private:

};