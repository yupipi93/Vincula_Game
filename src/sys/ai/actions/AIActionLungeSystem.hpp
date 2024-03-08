#pragma once

#include <sys/System.hpp>

class AIActionLungeSystem : public System {
public:
    AIActionLungeSystem();
    ~AIActionLungeSystem();

    void 	update(GameEngine &gameContext) const override;
private:
	void 	setTarget(GameEngine &gameContext, SituationComponent& sit, AIMoveToComponent &moveToComp, uint32_t targetId) const;
};