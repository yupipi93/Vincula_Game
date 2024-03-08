#pragma once

#include <sys/System.hpp>

class WinSystem : public System {
public:
    WinSystem();
    ~WinSystem();

    void update(GameEngine &gameContext) const override;
};
