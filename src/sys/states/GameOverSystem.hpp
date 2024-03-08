#pragma once

#include <sys/System.hpp>

class GameOverSystem : public System {
public:
    GameOverSystem();
    ~GameOverSystem();

    void update(GameEngine &gameContext) const override;

};
