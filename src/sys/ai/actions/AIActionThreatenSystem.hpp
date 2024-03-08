#pragma once

#include <sys/System.hpp>

class AIActionThreatenSystem : public System {
public:
    AIActionThreatenSystem();
    ~AIActionThreatenSystem();

    void 	update(GameEngine &gameContext) const override;

private:


};