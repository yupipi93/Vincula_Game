#include "LoadingSystem.hpp"
#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>

//console.log

// ---------------- KEY CODES ------------------
// Go to <https://www.glfw.org/docs/3.3/group__keys.html> to see key codes

const bool LOAD_RESOURCES = true;

LoadingSystem::LoadingSystem() {}

LoadingSystem::~LoadingSystem() {}

void LoadingSystem::update(GameEngine &gameContext) const {
    uint8_t &process = gameContext.loadingProcess;
    auto &wf = gameContext.getWindowFacadeRef();
    if (process < 100) {
        if (process == 0)
            gameContext.loadingText = "Plantando pinos (";
        if (process == 10)
            gameContext.loadingText = "Escondiendo cosas secretas (";
        if (process == 20)
            gameContext.loadingText = "Afilando armas (";
        if (process == 30)
            gameContext.loadingText = "Llenando carcajs (";
        if (process == 40)
            gameContext.loadingText = "Emplumando pollos (";
        if (process == 50)
            gameContext.loadingText = "Limpiando cerdos (";
        if (process == 60)
            gameContext.loadingText = "Alimentando animales (";
        if (process == 70)
            gameContext.loadingText = "Reinventando la rueda (";
        if (process == 80)
            gameContext.loadingText = "Fingiendo hacer cosas, pero no (";
        if (process == 90)
            gameContext.loadingText = "Componiendo musica (";

        if (LOAD_RESOURCES) {
            wf.loadModels(process);
            wf.loadHUD(process);
            ++process;
        } else
            process = 100;
    } else {
        uint32_t playerId = gameContext.playerId;
        InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);

        playerInput.resetMenuActions();


        if(wf.checkInput(GLFW_KEY_ENTER) || wf.checkInput(GLFW_KEY_SPACE)) {
            gameContext.setGameState(PLAYING);
        }
    }
}