#pragma once

#include <sys/System.hpp>

class AICombatStateSystem : public System {
public:
    AICombatStateSystem();
    ~AICombatStateSystem();

    void update(GameEngine &gameContext) const override;

private:
    void 		setState(GameEngine &gameContext, AIComponent &aIComponent, uint32_t entityTarget, AiState state) const;
    AiState 	determinateCombatState(GameEngine &gameContext, AICombatComponent &combatComp) const;

};