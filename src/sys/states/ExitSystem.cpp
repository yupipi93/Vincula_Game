#include "ExitSystem.hpp"
#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

// ---------------- KEY CODES ------------------
// Go to <https://www.glfw.org/docs/3.3/group__keys.html> to see key codes

ExitSystem::ExitSystem() {}

ExitSystem::~ExitSystem() {}

void ExitSystem::update(GameEngine &gameContext) const {
    
    uint32_t playerId = gameContext.playerId;
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);

    playerInput.resetMenuActions();

    
    int8_t option = gameContext.stateOptionSelected;

    auto &wf = gameContext.getWindowFacadeRef();

    if(wf.checkInput(GLFW_KEY_W) || wf.checkInput(GLFW_KEY_UP)) {
        if(!playerInput.upKeyPress && !playerInput.controlUpKeyPress) {
            playerInput.upKeyPress = true;
            playerInput.controlUpKeyPress = true;
        }
    } else {
        playerInput.upKeyPress = false;
    }
    if(playerInput.upKeyPress && playerInput.controlUpKeyPress) {
        --option;
    }

    if(wf.checkInput(GLFW_KEY_S) || wf.checkInput(GLFW_KEY_DOWN)) {
        if(!playerInput.downKeyPress && !playerInput.controlDownKeyPress) {
            playerInput.downKeyPress = true;
            playerInput.controlDownKeyPress = true;
        }
    } else {
        playerInput.downKeyPress = false;
    }
    if(playerInput.downKeyPress && playerInput.controlDownKeyPress) {
        ++option;
    }

    if (option < 0)
        option = 1;
    else if (option > 1)
        option = 0;


    if(wf.checkInput(GLFW_KEY_ESCAPE)) {
        if(!playerInput.escKeyPress && !playerInput.controlEscKeyPress) {
            playerInput.escKeyPress = true;
            playerInput.controlEscKeyPress = true;
        }
    } else {
        playerInput.escKeyPress = false;
    }

    if(playerInput.escKeyPress && playerInput.controlEscKeyPress) {
        option = 2;
        gameContext.setGameState(PAUSE);
        playerInput.controlEscKeyPress = false;
    }




    if(wf.checkInput(GLFW_KEY_ENTER) || wf.checkInput(GLFW_KEY_SPACE)) {
        if(!playerInput.returnKeyPress && !playerInput.controlReturnKeyPress) {
            playerInput.returnKeyPress = true;
            playerInput.controlReturnKeyPress = true;
        }
    } else {
        playerInput.returnKeyPress = false;
    }

    if(playerInput.returnKeyPress && playerInput.controlReturnKeyPress) {
        if(option == 0) {
            option = 2;
            gameContext.setGameState(PAUSE);
        } else if(option == 1) {
            gameContext.updatePlaying(false);
        }
    }

    gameContext.stateOptionSelected = option;

}



