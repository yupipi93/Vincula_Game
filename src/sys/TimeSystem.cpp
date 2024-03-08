#include "TimeSystem.hpp"
#include <eng/GameEngine.hpp>

//console.log

//#####################################################
//                == TIME SYSTEM ==
//  - Updates the current hour from the GameEngine
//
//   Using Componets:
//
//#####################################################


TimeSystem::TimeSystem() {}

TimeSystem::~TimeSystem() {}

void TimeSystem::update(GameEngine &gameContext) const {
    gameContext.currentHour += gameContext.getDeltaTime() / gameContext.timeFactor;
    // Check for new day
    if(gameContext.currentHour > 24.f) {
    	gameContext.currentHour -= 24.f;
    }

}