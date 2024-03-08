#pragma once

#include <sys/System.hpp>

class AIDefensiveStateSystem : public System {
public:
    AIDefensiveStateSystem();
    ~AIDefensiveStateSystem();

    void 	update(GameEngine &gameContext) const override;

private:

};