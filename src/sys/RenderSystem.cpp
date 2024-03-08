#include "RenderSystem.hpp"

#include <eng/GameEngine.hpp>


//#####################################################
//                == RENDER SYSTEM ==
//  - Call the render function in the current Engine
//
//   Using Componets:
//      - None
//#####################################################

RenderSystem::RenderSystem() {}

RenderSystem::~RenderSystem() {}

void RenderSystem::update(GameEngine &gameContext) const  {
    gameContext.getWindowFacadeRef().render(gameContext);
}
