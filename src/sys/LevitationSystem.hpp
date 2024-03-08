#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class LevitationSystem : public System {
public:
    LevitationSystem();
    ~LevitationSystem();

    void update(GameEngine &gameContext) const override;
    
private:

	void updateLevitation(GameEngine &gameContext, LevitationComponent &levitation, SituationComponent &situation) const;
};