#pragma once

#include <sys/System.hpp>

class AIStandByStateSystem : public System {
public:
    AIStandByStateSystem();
    ~AIStandByStateSystem();

    void 	update(GameEngine &gameContext) const override;

private:

};