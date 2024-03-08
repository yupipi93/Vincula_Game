#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>
class WindowFacade;

class InputSystem : public System {
public:
    InputSystem();
    ~InputSystem();

    void update(GameEngine &gameContext) const override;

private:
    void checkShowingMapAndStats(WindowFacade &wf, GameEngine &gameContext, LevelComponent &playerLevelComp) const;
    void checkPlayerMovement(WindowFacade &wf, InputComponent &playerInput, LevelComponent &playerLevelComp, bool isPlayerSecure) const;
    void checkPlayerInteraction(WindowFacade &wf, InputComponent &playerInput, LevelComponent &playerLevelComp, bool playerIsSecure) const;
    void checkPausing(WindowFacade &wf, GameEngine &gameContext, InputComponent &playerInput) const;
    void checkChangeCamera(WindowFacade &wf, GameEngine &gameContext, InputComponent &playerInput) const;
    void checkOverPower(WindowFacade &wf, GameEngine &gameContext) const;
    void setTimeManually(WindowFacade &wf, GameEngine &gameContext) const;
};
