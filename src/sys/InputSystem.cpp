#include "InputSystem.hpp"

#include <eng/GameEngine.hpp>
#include <fac/WindowFacade.hpp>

// ---------------- KEY CODES ------------------
// Go to <https://www.glfw.org/docs/3.3/group__keys.html> to see key codes

InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::update(GameEngine &gameContext) const {
    float deltaTime = gameContext.getDeltaTime();
    uint32_t playerId = gameContext.playerId;
    InputComponent &playerInput = gameContext.entityMan.getComponent<InputComponent>(playerId);
    LevelComponent &playerLevelComp = gameContext.entityMan.getComponent<LevelComponent>(playerId);
    AtributesComponent &playerAtrib = gameContext.entityMan.getComponent<AtributesComponent>(playerId);
    WindowFacade &wf = gameContext.getWindowFacadeRef();

    //Reset all the values before the new checks
    playerInput.resetActions();

    playerInput.subDeltaTime(deltaTime);

    bool paralyzed = false;
    if (gameContext.entityMan.existsComponent<EffectParalysisComponent>(gameContext.playerId))
        if (gameContext.entityMan.getComponent<EffectParalysisComponent>(gameContext.playerId).paralysisTime)
            paralyzed = true;

    bool hasWeapon = gameContext.playerHasWeapon;
    if (!paralyzed && hasWeapon)
        checkPlayerMovement(wf, playerInput, playerLevelComp, playerAtrib.isSecure);

    checkPlayerInteraction(wf, playerInput, playerLevelComp, playerAtrib.isSecure);

    checkPausing(wf, gameContext, playerInput);

    checkShowingMapAndStats(wf, gameContext, playerLevelComp);

    if(gameContext.debuging) {
        checkChangeCamera(wf, gameContext, playerInput);
        checkOverPower(wf, gameContext);
        setTimeManually(wf, gameContext);
    }



    // PALANTIR: MOVE TO NEW METHOD
    //DROP PALANTIR
    if(wf.checkInput(GLFW_KEY_Q) && !gameContext.palantirId) {
        playerInput.dropingPalantir = true;
    }

    //Reset Mouse Scrolls
    gameContext.getWindowFacadeRef().resetScroll();
}

void InputSystem::checkShowingMapAndStats(WindowFacade &wf, GameEngine &gameContext, LevelComponent &playerLevelComp) const {
    InputComponent &input {gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId)};
    if (wf.checkInput(GLFW_KEY_LEFT_SHIFT) || wf.checkInput(GLFW_KEY_TAB)) {
        input.lookingStats = true;
        if (playerLevelComp.mapUnlocked)
            input.lookingMap = true;
    } else {
        input.lookingMap = false;
        input.lookingStats = false;
    }
}

void InputSystem::checkPlayerMovement(WindowFacade &wf, InputComponent &playerInput, LevelComponent &playerLevelComp, bool playerIsSecure) const {
    //It checks the movement keys
    if(wf.checkInput(GLFW_KEY_W)) {
        playerInput.pressingW = true;
    }

    if(wf.checkInput(GLFW_KEY_A)) {
        playerInput.pressingA = true;
    }

    if(wf.checkInput(GLFW_KEY_S)) {
        playerInput.pressingS = true;
    }

    if(wf.checkInput(GLFW_KEY_D)) {
        playerInput.pressingD = true;
    }

    //End the movement check keys

    //Check the dash, the player must be moving in order to dash (Only with key down, not press)
    if(wf.checkInput(GLFW_KEY_SPACE) && !playerIsSecure && playerLevelComp.dashUnlocked) {
        if(!playerInput.espKeyPress && !playerInput.controlEspKeyPress) {
            playerInput.espKeyPress = true;
            playerInput.controlEspKeyPress = true;
        }
    } else {
        playerInput.espKeyPress = false;
    }
    if(playerInput.espKeyPress && playerInput.controlEspKeyPress) {
        playerInput.dashing = true;
    }
}

void InputSystem::checkPlayerInteraction(WindowFacade &wf, InputComponent &playerInput, LevelComponent &playerLevelComp, bool playerIsSecure) const {
    //INTERACT
    if(wf.checkInput(GLFW_KEY_F)) {
        if(playerInput.interactingTimer <= 0) {
            playerInput.interacting = true;
            playerInput.interactingTimer = playerInput.interactingMaxTimer;
        }
    }

    //CALL COMPASS
    if(wf.checkInput(GLFW_KEY_E) && playerLevelComp.compassUnlocked) {
        if(playerInput.callingCompassTimer <= 0) {
            playerInput.callingCompass = true;
            playerInput.callingCompassTimer = playerInput.callingCompassMaxTimer;
        }
    }

    //ONE CLICK DOWN (NOT PRESSED) CONTROL
    if(wf.getLeftClick()) {
        if(!playerInput.leftMouseClick && !playerInput.controlClickLeft) {
            playerInput.leftMouseClick = true;
            playerInput.controlClickLeft = true;
        }
    } else {
        playerInput.leftMouseClick = false;
    }
    if(playerInput.leftMouseClick && playerInput.controlClickLeft && !playerIsSecure) {
        playerInput.attacking = true;
    }

    // SETTING TARGET
    if(wf.getMiddleClick()) {
        if(!playerInput.middleMouseClick && !playerInput.controlClickMiddle) {
            playerInput.middleMouseClick = true;
            playerInput.controlClickMiddle = true;
        }
    } else {
        playerInput.middleMouseClick = false;
    }
    if(playerInput.middleMouseClick && playerInput.controlClickMiddle) {
        playerInput.setingTarget = true;
    }


    if(!playerInput.doingAttack && (wf.getUpScroll() || wf.getDownScroll())) {
        if(playerInput.changingWeaponTimer <= 0) {
            playerInput.changingWeapon = true;
            playerInput.changingWeaponTimer = playerInput.changingWeaponMaxTimer;
        }
    }


    playerInput.mousePosition = wf.getMousePosition();
}

void InputSystem::checkPausing(WindowFacade &wf, GameEngine &gameContext, InputComponent &playerInput) const {
    if(wf.checkInput(GLFW_KEY_ESCAPE)) {
        if(!playerInput.escKeyPress && !playerInput.controlEscKeyPress) {
            playerInput.escKeyPress = true;
            playerInput.controlEscKeyPress = true;
        }
    } else {
        playerInput.escKeyPress = false;
    }

    if(playerInput.escKeyPress && playerInput.controlEscKeyPress) {
        gameContext.setGameState(PAUSE);
    }



    //OTHERS
    /*if(wf.checkInput(GLFW_KEY_B)) {
        playerInput.dancing = true;
    }*/


}

void InputSystem::checkChangeCamera(WindowFacade &wf, GameEngine &gameContext, InputComponent &playerInput) const {
    if(wf.checkInput(GLFW_KEY_F1)) {
        playerInput.choppedZoomCamera   = true;
        playerInput.choppedCamera       = false;
        playerInput.smartCamera         = false;
        playerInput.zenithCamera        = false;
        playerInput.zenithUpCamera      = false;
        playerInput.airCamera           = false;
    }
    if(wf.checkInput(GLFW_KEY_F2)) {
        playerInput.choppedZoomCamera   = false;
        playerInput.choppedCamera       = true;
        playerInput.smartCamera         = false;
        playerInput.zenithCamera        = false;
        playerInput.zenithUpCamera      = false;
        playerInput.airCamera           = false;

    }
    if(wf.checkInput(GLFW_KEY_F3)) {
        playerInput.choppedZoomCamera   = false;
        playerInput.choppedCamera       = false;
        playerInput.smartCamera         = true;
        playerInput.zenithCamera        = false;
        playerInput.zenithUpCamera      = false;
        playerInput.airCamera           = false;

    }
    if(wf.checkInput(GLFW_KEY_F4)) {
        playerInput.choppedZoomCamera   = false;
        playerInput.choppedCamera       = false;
        playerInput.smartCamera         = false;
        playerInput.zenithCamera        = true;
        playerInput.zenithUpCamera      = false;
        playerInput.airCamera           = false;

    }
    if(wf.checkInput(GLFW_KEY_F5)) {
        playerInput.choppedZoomCamera   = false;
        playerInput.choppedCamera       = false;
        playerInput.smartCamera         = false;
        playerInput.zenithCamera        = false;
        playerInput.zenithUpCamera      = true;
        playerInput.airCamera           = false;

    }

    if(wf.checkInput(GLFW_KEY_F6)) {
        playerInput.choppedZoomCamera   = false;
        playerInput.choppedCamera       = false;
        playerInput.smartCamera         = false;
        playerInput.zenithCamera        = false;
        playerInput.zenithUpCamera      = false;
        playerInput.airCamera           = true;

    }

}

void InputSystem::checkOverPower(WindowFacade &wf, GameEngine &gameContext) const {
    if(wf.checkInput(GLFW_KEY_O)) {
        VelocityComponent       &velocityComp       = gameContext.entityMan.getComponent<VelocityComponent>(gameContext.playerId);
        StatsComponent          &statsComp          = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
        WalletComponent         &walletComp         = gameContext.entityMan.getComponent<WalletComponent>(gameContext.playerId);
        LevelComponent          &playerLevel           = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);

        //OVERPOWERED
        /*
                if (playerLevel.level == 99) {
                    velocityComp.speed      = velocityComp.speedNormal;
                    statsComp.maxHealth     = 10;
                    statsComp.currentHealth = statsComp.maxHealth;
                    playerLevel.level          = 1;
                    statsComp.damage        = 1;
                    walletComp.coins        = 0;
                } else {
                    velocityComp.speed      = 2000;
                    statsComp.maxHealth     = 30000;
                    statsComp.currentHealth = statsComp.maxHealth;
                    playerLevel.level          = 99;
                    statsComp.damage        = 255;
                    walletComp.coins        = 65000;
                }
        */

        if (velocityComp.speed == velocityComp.speedNormal)
            velocityComp.speed = 1000;
        else
            velocityComp.speed = velocityComp.speedNormal;

        gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, LEVEL_UP_SOUND);
        playerLevel.levelingUp = true;

        playerLevel.level         += 1;
        uint8_t newDamage         = pow(1.66, (0.5 * ((float)playerLevel.level + 1)));
        uint16_t newHealth        = pow(10.f, (0.13 * ((float)playerLevel.level - 1) + 1));
        statsComp.maxHealth     = newHealth;
        statsComp.currentHealth = statsComp.maxHealth;
        statsComp.damage        = newDamage;
    }

}

void InputSystem::setTimeManually(WindowFacade &wf, GameEngine &gameContext) const {
    //SET DAY
    if(wf.checkInput(GLFW_KEY_1)) {
        gameContext.currentHour = 7.f;
    }
    //SET SUNSET
    if(wf.checkInput(GLFW_KEY_2)) {
        gameContext.currentHour = 15.f;
    }
    //SET NOGHT
    if(wf.checkInput(GLFW_KEY_3)) {
        gameContext.currentHour = 23.f;
    }


}


