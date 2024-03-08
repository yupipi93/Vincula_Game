#pragma once

#include <sys/System.hpp>

class ExitSystem : public System {
public:
    ExitSystem();
    ~ExitSystem();

    void update(GameEngine &gameContext) const override;

};
