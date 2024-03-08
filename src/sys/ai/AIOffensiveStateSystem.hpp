#pragma once

#include <sys/System.hpp>

class AIOffensiveStateSystem : public System {
public:
    AIOffensiveStateSystem();
    ~AIOffensiveStateSystem();

    void 	update(GameEngine &gameContext) const override;

private:
    AiAction determateOffensiveAction(GameEngine &gameContext, AIDifuseActionComponent &aIDifuse, uint32_t targetId) const;

};