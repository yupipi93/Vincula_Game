#include "GameOverSystem.hpp"
#include <tools/Utils.hpp>

#include <eng/GameEngine.hpp>


// ---------------- KEY CODES ------------------
// Go to <https://www.glfw.org/docs/3.3/group__keys.html> to see key codes

GameOverSystem::GameOverSystem() {}

GameOverSystem::~GameOverSystem() {}

void GameOverSystem::update(GameEngine &gameContext) const {

    uint32_t playerId = gameContext.playerId;
    auto &playerStats = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
    if (playerStats.deadTime >= 2) {
        InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);

        playerInput.resetMenuActions();

        auto &wf = gameContext.getWindowFacadeRef();


        if(wf.checkInput(GLFW_KEY_ENTER) || wf.checkInput(GLFW_KEY_SPACE)) {
        	playerStats.deadTime = 0;
            gameContext.reset();
        }
    } else playerStats.deadTime += gameContext.getDeltaTime();
}