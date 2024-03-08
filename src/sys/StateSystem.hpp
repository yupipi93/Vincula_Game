#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class StateSystem : public System {
public:
    StateSystem();
    ~StateSystem();

    void update(GameEngine &gameContext) const override;
private:
	void checkGameOver (uint32_t &playerId, GameEngine &gameContext) const;
	void checkWinGame (uint32_t &playerId, GameEngine &gameContext) const;
};