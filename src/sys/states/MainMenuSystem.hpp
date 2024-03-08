#pragma once

#include <sys/System.hpp>

class MainMenuSystem : public System {
public:
    MainMenuSystem();
    ~MainMenuSystem();

    void update(GameEngine &gameContext) const override;

};
