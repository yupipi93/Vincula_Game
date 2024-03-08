#pragma once

#include <sys/System.hpp>

class LoadingSystem : public System {
public:
    LoadingSystem();
    ~LoadingSystem();

    void update(GameEngine &gameContext) const override;

};
