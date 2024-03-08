#pragma once

#include <sys/System.hpp>

class AIActionDistanceSystem : public System {
public:
    AIActionDistanceSystem();
    ~AIActionDistanceSystem();

    void update(GameEngine &gameContext) const override;

};