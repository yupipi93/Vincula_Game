#pragma once

#include <sys/System.hpp>

class AIActionWanderSystem : public System {
public:
    AIActionWanderSystem();
    ~AIActionWanderSystem();

    void 	update(GameEngine &gameContext) const override;

private:
    void setRandomTarget(GameEngine &gameContext, AIMoveToComponent &moveToComp,  uint32_t entityId) const;
    void setRandomSleepTime(AIActionWanderComponent &wanderComp) const;

};