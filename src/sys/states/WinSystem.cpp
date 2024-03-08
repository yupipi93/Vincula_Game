#include "WinSystem.hpp"

#include <eng/GameEngine.hpp>


// ---------------- KEY CODES ------------------
// Go to <https://www.glfw.org/docs/3.3/group__keys.html> to see key codes

WinSystem::WinSystem() {}

WinSystem::~WinSystem() {}

void WinSystem::update(GameEngine &gameContext) const {

	uint32_t playerId = gameContext.playerId;
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);

    playerInput.resetMenuActions();

    auto &wf = gameContext.getWindowFacadeRef();

    if(wf.checkInput(GLFW_KEY_ENTER) || wf.checkInput(GLFW_KEY_SPACE)) {
        if(!playerInput.returnKeyPress && !playerInput.controlReturnKeyPress) {
            playerInput.returnKeyPress = true;
            playerInput.controlReturnKeyPress = true;
        }
    } else {
        playerInput.returnKeyPress = false;
    }
    if(playerInput.returnKeyPress && playerInput.controlReturnKeyPress) {
        if (gameContext.finalDialogue == 5) {
            gameContext.updatePlaying(false);
        } else {
        	++gameContext.finalDialogue;
        }
    }
}




