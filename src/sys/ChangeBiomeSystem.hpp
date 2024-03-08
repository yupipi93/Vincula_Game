#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class ChangeBiomeSystem : public System {
public:
    ChangeBiomeSystem();
    ~ChangeBiomeSystem();

    void update(GameEngine &gameContext) const override;

private:
	void callFunctionOfNewBiome(GameEngine &gameContext) const;

};