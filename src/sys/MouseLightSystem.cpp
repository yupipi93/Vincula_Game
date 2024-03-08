#include "MouseLightSystem.hpp"

#include <eng/GameEngine.hpp>

//#####################################################
//                == MOUSE LIGTH SYSTEM ==
//  - Update mouse ligth position
//
//
//   Using Componets:
//      - Situation        (Read - Write)
//#####################################################


MouseLightSystem::MouseLightSystem() {}

MouseLightSystem::~MouseLightSystem() {}

void MouseLightSystem::update(GameEngine &gameContext) const {

    SituationComponent &lightSit = gameContext.entityMan.getComponent<SituationComponent>(gameContext.mouseLightId);
    SituationComponent &mouseShapeSit = gameContext.entityMan.getComponent<SituationComponent>(gameContext.mouseShapeId);
    AIMoveToComponent  &playerMoveToComp = gameContext.entityMan.getComponent<AIMoveToComponent>(gameContext.playerId);


    if (playerMoveToComp.targetId) {
        SituationComponent &targetSit = gameContext.entityMan.getComponent<SituationComponent>(playerMoveToComp.targetId);

        lightSit.x = mouseShapeSit.x = targetSit.x;
        lightSit.y = mouseShapeSit.y =  50.f;
        lightSit.z = mouseShapeSit.z =  targetSit.z;
    } else {
        std::array<float, 3> mouseProjectedCoor = gameContext.getWindowFacadeRef().getProjectedCursor();
        lightSit.x = mouseShapeSit.x = mouseProjectedCoor[0];
        lightSit.y = mouseShapeSit.y =  23.f;
        lightSit.z = mouseShapeSit.z =  mouseProjectedCoor[2];
    }

    gameContext.getWindowFacadeRef().updateEntities(gameContext, {gameContext.mouseShapeId});
    gameContext.getWindowFacadeRef().updatePointLight(gameContext, gameContext.mouseLightId);
}
