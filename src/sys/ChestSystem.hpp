#pragma once
#include "System.hpp"

class ChestSystem : public System {
public:
    ChestSystem();
    ~ChestSystem();

    void update(GameEngine &gameContext) const override;

private:

	void searchChest(GameEngine &gameContext, InteractionComponent &playerInteraction) const;
	
};