#pragma once

#include <sys/System.hpp>

class PauseSystem : public System {
public:
    PauseSystem();
    ~PauseSystem();

    void update(GameEngine &gameContext) const override;

};
