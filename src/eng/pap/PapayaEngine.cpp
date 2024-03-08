#include "PapayaEngine.hpp"
#include <eng/GameEngine.hpp>
#include <tools/Utils.hpp>

#include <string>
//console.log
#include <filesystem>
namespace fs = std::filesystem;

//constexpr bool fullscreen = false;


PapayaEngine::PapayaEngine(int width, int height, bool windowsMode)
    : nodeMap(), SCREEN_WIDTH{width}, SCREEN_HEIGHT{height}, device{std::make_unique<pap::PapayaDevice>(width, height, !windowsMode)} {

    // TODO add lights
    //device.get()->addDirectionalLightSceneNode(nullptr, glm::vec3(60.f, 67.f, -45.f), glm::vec3(1.f, 1.0f, 1.f), glm::vec3(-0.5f, -1.0f, -0.3f));

    //device.get()->addPointLightSceneNode(nullptr, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.15f, 1.f, 0.3f), 1.f, 0.014f, 0.0007f);
    //device.get()->addPointLightSceneNode(nullptr, glm::vec3(0.f, 0.f, -20.f), glm::vec3(1.0f, 0.2f, 0.3f), 1.f, 0.014f, 0.0007f);

    // Set the textrenderer font
    //device.get()->getTextRenderer().loadFont("./Tipografia/Vincula Handwriting.ttf", 24);
    device.get()->getTextRenderer().loadFont("./Tipografia/IBMPlexMono-Regular.ttf", 25);

}

PapayaEngine::~PapayaEngine() {
}

const bool PapayaEngine::checkInput(const uint16_t key) const {
    return device.get()->getWindow()->receiver.isKeyPressed(key);
}

std::array<float, 2> PapayaEngine::getMousePosition() const {
    std::array<float, 2> mousePositions;
    mousePositions[0] = device.get()->getWindow()->mouseReceiver.mouseX;
    mousePositions[1] = device.get()->getWindow()->mouseReceiver.mouseY;


    return mousePositions;
}

std::array<float, 3> PapayaEngine::getProjectedCursor() const {
    std::array<float, 3> projectedCursor;

    glm::vec3 mousePos = device.get()->getProjectedCursor();

    projectedCursor[0] = mousePos.x;
    projectedCursor[1] = mousePos.y;
    projectedCursor[2] = -mousePos.z;
    return projectedCursor;
}


bool PapayaEngine::getLeftClick() const {
    return device.get()->getWindow()->mouseReceiver.LeftButtonDown;
}

bool PapayaEngine::getRightClick() const {
    return device.get()->getWindow()->mouseReceiver.RightButtonDown;
}

bool PapayaEngine::getMiddleClick() const {
    return device.get()->getWindow()->mouseReceiver.MiddleButtonDown;
}

void PapayaEngine::resetScroll() const {
    device.get()->getWindow()->mouseReceiver.ScrollUp = false;
    device.get()->getWindow()->mouseReceiver.ScrollDown = false;
}

bool PapayaEngine::getUpScroll() const {
    return device.get()->getWindow()->mouseReceiver.ScrollUp;
}

bool PapayaEngine::getDownScroll() const {
    return device.get()->getWindow()->mouseReceiver.ScrollDown;
}

void PapayaEngine::render(GameEngine &gameContext) const {
    device.get()->clearColor = glm::vec4(0.0f, 0.f, 1.0f, 1.0f);
    // Draw 3d things
    device.get()->drawAll();

    // Draw 2d HUD
    uint32_t HUDID = gameContext.HUDId;
    HUDComponent &HUD = gameContext.entityMan.getComponent<HUDComponent>(HUDID);
    auto &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(gameContext.playerId);

    if(playerInventory.primaryWeaponId == 0 && !gameContext.debuging) {
        drawTutorialPhases(gameContext);
    } else {
        drawStats(gameContext, HUD);
        drawEnemiesHealth(gameContext);
        drawEggsHealth(gameContext);
        drawHealthHighlights(gameContext, HUD);
        drawHudBackground(gameContext, HUD);
        drawLevel(gameContext, HUD);
        drawHealth(gameContext, HUD);
        drawWeapons(gameContext, HUD);
        drawWalletCoins(gameContext);
        drawDash(gameContext, HUD);
        drawEffects(gameContext, HUD);
        drawKeys(gameContext, HUD);
    }
    drawInfoInteroperable(gameContext);
    drawInfoActions(gameContext);
    drawInfoNotifications(gameContext);
    drawMap(gameContext, HUD);

    drawBiomeThings(gameContext);

    // Draw dialogues
    drawDialogues(gameContext);

    //Draw menu
    drawStateHUD(gameContext);

    device.get()->endScene();
}

void PapayaEngine::drawKeys(GameEngine &gameContext, HUDComponent &HUD) const {
    uint32_t playerID = gameContext.playerId;
    InventoryComponent   &playerInventory   = gameContext.entityMan.getComponent<InventoryComponent>(playerID);
    if (playerInventory.showKeysTime > 0) {
        device.get()->draw2DImage(HUD.keys, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        std::string totalKeysText = std::to_string( int(playerInventory.keysCollected.size()));
        totalKeysText += "/";
        totalKeysText += std::to_string( int(playerInventory.MAX_KEYS) );

        device.get()->renderText(totalKeysText, 126 * SCREEN_WIDTH / 1920, 364 * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), false);
    }
}

void PapayaEngine::drawHealthHighlights(GameEngine &gameContext, HUDComponent &HUD) const {
    // HEALTH HIGHLIGHT
    uint32_t playerID = gameContext.playerId;
    StatsComponent &playerStats = gameContext.entityMan.getComponent<StatsComponent>(playerID);
    if (playerStats.healing)
        device.get()->draw2DImage(HUD.healingHighlight, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    if (playerStats.damaged)
        device.get()->draw2DImage(HUD.healthHighlight, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void PapayaEngine::drawHudBackground(GameEngine &gameContext, HUDComponent &HUD) const {
    device.get()->draw2DImage(HUD.background, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void PapayaEngine::drawHealth(GameEngine &gameContext, HUDComponent &HUD) const {
    uint32_t playerID = gameContext.playerId;
    StatsComponent &playerStats = gameContext.entityMan.getComponent<StatsComponent>(playerID);
    uint32_t health = playerStats.currentHealth;


    float healthPerc = calculateHealth(health, playerStats.maxHealth);

    auto healthTexture = HUD.health;
    if (gameContext.entityMan.existsComponent<EffectPoisonComponent>(playerID))
        if (gameContext.entityMan.getComponent<EffectPoisonComponent>(playerID).damageTimes > 0)
            healthTexture = HUD.poison;

    device.get()->draw2DImage(healthTexture, glm::vec2(150 * SCREEN_WIDTH / 1920.f, 56 * SCREEN_HEIGHT / 1080.f),
                              glm::vec2(healthPerc * SCREEN_WIDTH / 1920.f, 38 * SCREEN_HEIGHT / 1080.f));


    device.get()->draw2DImage(HUD.healthDetails, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    //device.get()->draw2DImage(HUD.healthDetails2, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));


    // NUMBER
    std::string healthText = std::to_string( (int)health );

    // Draw the coin text

    uint8_t auxPosX = 102;
    uint8_t auxPosY = 68;

    device.get()->renderText(healthText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), true);

}

void PapayaEngine::drawEnemiesHealth(GameEngine &gameContext) const {

    auto &enemies = gameContext.entityMan.getComponents<EnemyComponent>();

    for(auto &enemy : enemies) {
        uint32_t enemyID = enemy.id;
        StatsComponent &enemyStats = gameContext.entityMan.getComponent<StatsComponent>(enemyID);
        SituationComponent &enemySituation = gameContext.entityMan.getComponent<SituationComponent>(enemyID);
        auto &aiComp = gameContext.entityMan.getComponent<AIComponent>(enemy.id);

        float rotationY = updateElement2DLookAtCamera(gameContext, enemySituation, gameContext.cameraId);

        //DRAW HIGHLIGHT
        if (enemyStats.damaged) {
            device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(3.2f, 0.45, 1.f), "./media/hud/enemies/healthHighlight.png");

            //Draw hit damage
            std::string hitDamage = std::to_string((int)enemyStats.lastDamageReceived);
            glm::vec3 color {1.f, 1.f, 1.f};
            if (enemyStats.isCritic)
                color = glm::vec3(1.f, 0.f, 0.f);

            device.get()->renderText3d(hitDamage, glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight + 5.f, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), 0.4f, color);

        }


        //DRAW BACKGROUND
        device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight, -enemySituation.z), glm::vec3(0.f, rotationY,  0.f), glm::vec3(3.f, 0.25f, 1.f), "./media/hud/enemies/newBackground.png");


        //DRAW HEALTH
        float healthPerc = (float)enemyStats.currentHealth / (float)enemyStats.maxHealth;
        //float healthDisplacement = (((float)enemyStats.maxHealth - (float)enemyStats.currentHealth)) * 5.f;

        auto healthTexture = "./media/hud/enemies/newHealth.png";
        if (gameContext.entityMan.existsComponent<EffectPoisonComponent>(enemyID))
            healthTexture = "./media/hud/enemies/newPoison.png";

        if (gameContext.entityMan.existsComponent<EffectSlowdownComponent>(enemyID))
            device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight - 5, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(0.6, 0.6f, 0.6f), "./media/hud/enemies/slow.png");

        if (gameContext.entityMan.existsComponent<EffectParalysisComponent>(enemyID))
            device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight - 5, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(0.6, 0.6f, 0.6f), "./media/hud/enemies/stunned.png");

        //WINHERE
        if (gameContext.entityMan.existsComponent<EffectWinHeartComponent>(enemyID))
                device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight - 5, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(0.6, 0.6f, 0.6f), "./media/hud/enemies/winHeart.png");


        device.get()->drawElement2Din3D(glm::vec3(enemySituation.x, enemySituation.y + enemy.healthHeight, -enemySituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(healthPerc * 3.f, 0.25f, 1.f), healthTexture);
    }
}

void PapayaEngine::drawEggsHealth(GameEngine &gameContext) const {

    auto &eggs = gameContext.entityMan.getComponents<EggDataComponent>();

    for(auto &egg : eggs) {
        uint32_t eggID = egg.id;
        StatsComponent &eggStats = gameContext.entityMan.getComponent<StatsComponent>(eggID);
        SituationComponent &eggSituation = gameContext.entityMan.getComponent<SituationComponent>(eggID);

        if(!egg.broken) {
            float rotationY = updateElement2DLookAtCamera(gameContext, eggSituation, gameContext.cameraId);

            //DRAW HIGHLIGHT
            if (eggStats.damaged) {
                device.get()->drawElement2Din3D(glm::vec3(eggSituation.x, eggSituation.y + egg.healthHeight, -eggSituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(5.2f, 0.85, 1.f), "./media/hud/enemies/healthHighlight.png");

                //Draw hit damage
                std::string hitDamage = std::to_string((int)eggStats.lastDamageReceived);
                device.get()->renderText3d(hitDamage, glm::vec3(eggSituation.x, eggSituation.y + egg.healthHeight + 5.f, -eggSituation.z), glm::vec3(0.f, rotationY, 0.f), 0.4f, glm::vec3(1.f, 1.f, 1.f));

            }

            //DRAW BACKGROUND
            device.get()->drawElement2Din3D(glm::vec3(eggSituation.x, eggSituation.y + egg.healthHeight, -eggSituation.z), glm::vec3(0.f, rotationY,  0.f), glm::vec3(5.f, 0.65f, 1.f), "./media/hud/enemies/newBackground.png");

            //DRAW HEALTH
            float healthPerc = (float)eggStats.currentHealth / (float)eggStats.maxHealth;

            auto healthTexture = "./media/hud/enemies/newHealth.png";


            device.get()->drawElement2Din3D(glm::vec3(eggSituation.x, eggSituation.y + egg.healthHeight, -eggSituation.z), glm::vec3(0.f, rotationY, 0.f), glm::vec3(healthPerc * 5.f, 0.65f, 1.f), healthTexture);
        }
    }
}



float PapayaEngine::calculateHealth(uint32_t currentHealth, uint32_t maxHealth) const {

    float healthPercent = currentHealth * 100 / maxHealth;
    float health = 563 * healthPercent / 100;

    return health;
}


void PapayaEngine::drawWeapons(GameEngine &gameContext, HUDComponent &HUD) const {
    bool isSecure = gameContext.entityMan.getComponent<AtributesComponent>(gameContext.playerId).isSecure;

    // SECONDARY WEAPON
    device.get()->draw2DImage(HUD.secondaryWeapon, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    if(weaponsSecondaryTexture != "")
        device.get()->draw2DImage(weaponsSecondaryTexture, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    if (isSecure)
        device.get()->draw2DImage(HUD.secondaryCancel, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    // PRIMARY WEAPON
    device.get()->draw2DImage(HUD.primaryWeapon, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    if(weaponsPrimaryTexture != "")
        device.get()->draw2DImage(weaponsPrimaryTexture, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    if (isSecure)
        device.get()->draw2DImage(HUD.primaryCancel, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

}

void PapayaEngine::drawWalletCoins(GameEngine &gameContext) const {

    uint32_t playerID = gameContext.playerId;
    WalletComponent &playerWallet = gameContext.entityMan.getComponent<WalletComponent>(playerID);
    uint32_t coins = playerWallet.coins;

    std::string coinsText = std::to_string( int(coins) );

    // Draw the coin text
    device.get()->renderText(coinsText, 115 * SCREEN_WIDTH / 1920, 209 * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), false);
}

void PapayaEngine::drawLevel(GameEngine &gameContext, HUDComponent &HUD) const {
    uint32_t playerID = gameContext.playerId;
    LevelComponent &levelComponent  = gameContext.entityMan.getComponent<LevelComponent>(playerID);
    StatsComponent &statsComponent = gameContext.entityMan.getComponent<StatsComponent>(playerID);

    uint16_t auxPosX;
    uint16_t auxPosY;
    auto color = glm::vec3(1.0f, 1.0f, 1.0f);

    // LEVELING
    if (levelComponent.levelingUp) {
        device.get()->draw2DImage(HUD.levelingUp, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        //Draw health Diff
        auto newHealth = statsComponent.maxHealth;
        auto oldHealth = pow(10.f, (0.13 * ((float)levelComponent.level - 2) + 1));
        auto diff = newHealth - oldHealth;
        std::string healthText = "+";
        healthText += std::to_string( int(diff) );
        auxPosX = 1080;
        auxPosY = 640;
        device.get()->renderText(healthText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, color, true);




        //Draw Damage Diff
        auto newStrg = statsComponent.damage;
        auto oldStrg = pow(1.66, (0.5 * ((float)levelComponent.level - 2)));
        diff = newStrg - oldStrg;
        std::string strgText = "+";
        strgText += std::to_string( int(diff) );
        auxPosX = 1080;
        auxPosY = 710;
        device.get()->renderText(strgText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, color, true);



        // Draw the damage text
        auxPosX = 225;
        auxPosY = 150;
        std::string damageText = std::to_string( int(statsComponent.damage) );
        device.get()->renderText(damageText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), true);


    }
    color = glm::vec3(0.0f, 0.0f, 0.0f);

    // Draw the level text
    uint32_t level = levelComponent.level;
    std::string levelText = std::to_string( level );
    auxPosX = 225;
    auxPosY = 110;
    device.get()->renderText(levelText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, color, true);
}

void PapayaEngine::drawDash(GameEngine &gameContext, HUDComponent &HUD) const {

    uint32_t playerID = gameContext.playerId;
    LevelComponent &level = gameContext.entityMan.getComponent<LevelComponent>(playerID);
    DashComponent &dash = gameContext.entityMan.getComponent<DashComponent>(playerID);


    if (level.dashUnlocked)
        if(!dash.dashing) {

            if (dash.timerCoolDown == 0) {

                //device.get()->draw2DImage(HUD.dashComplete, glm::vec2(658 * SCREEN_WIDTH / 1920.f, 103 * SCREEN_HEIGHT / 1080.f),
                // glm::vec2(53 * SCREEN_WIDTH / 1920.f, 53 * SCREEN_HEIGHT / 1080.f));
                device.get()->draw2DImage(HUD.dashComplete, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

                bool isSecure = gameContext.entityMan.getComponent<AtributesComponent>(gameContext.playerId).isSecure;
                if (isSecure)
                    //device.get()->draw2DImage(HUD.dashCancel, glm::vec2(658 * SCREEN_WIDTH / 1920.f, 103 * SCREEN_HEIGHT / 1080.f),
                    //glm::vec2(53 * SCREEN_WIDTH / 1920.f, 53 * SCREEN_HEIGHT / 1080.f));
                    device.get()->draw2DImage(HUD.dashCancel, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

            } else {
                float dashPerc = (dash.maxCoolDown - dash.timerCoolDown) / dash.maxCoolDown;
                uint16_t imageWidth = 371;
                uint16_t imageHeight = 16;

                dashPerc *= imageWidth;//dash image width
                device.get()->draw2DImage(HUD.dashRecharging, glm::vec2(285 * SCREEN_WIDTH / 1920.f, 130 * SCREEN_HEIGHT / 1080.f),
                                          glm::vec2(dashPerc * SCREEN_WIDTH / 1920.f, imageHeight * SCREEN_HEIGHT / 1080.f));
                device.get()->draw2DImage(HUD.dashFrame, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));


            }

        } else {

            device.get()->draw2DImage(HUD.dashFrame, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        }
}

void PapayaEngine::drawEffects(GameEngine &gameContext, HUDComponent &HUD) const {

    uint32_t playerID = gameContext.playerId;
    if (gameContext.entityMan.existsComponent<EffectParalysisComponent>(playerID))
        device.get()->draw2DImage(HUD.stun, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));


}

void PapayaEngine::drawStats(GameEngine &gameContext, HUDComponent &HUD) const {
    StatsComponent &stats {gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId)};
    InputComponent &input {gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId)};
    WalletComponent &bank {gameContext.entityMan.getComponent<WalletComponent>(gameContext.bankId)};
    InventoryComponent &i {gameContext.entityMan.getComponent<InventoryComponent>(gameContext.playerId)};
    if (!gameContext.playerBiome || input.lookingStats) {
        //For the tutorial
        if(i.primaryWeaponId != 0 || gameContext.debuging)
            device.get()->draw2DImage(HUD.stats, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));


        std::string bankCoinsText = std::to_string( int(bank.coins) );
        std::string totalKeysText = std::to_string( int(i.keysCollected.size()));
        totalKeysText += "/";
        totalKeysText += std::to_string( int(i.MAX_KEYS) );

        //For the tutorial
        if(i.primaryWeaponId != 0 || gameContext.debuging) {
            device.get()->renderText(bankCoinsText, 115 * SCREEN_WIDTH / 1920, 292 * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), false);
            device.get()->renderText(totalKeysText, 126 * SCREEN_WIDTH / 1920, 364 * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), false);
        }


        // Draw the damage text
        uint16_t auxPosX = 225;
        uint16_t auxPosY = 150;

        std::string damageText = std::to_string( int(stats.damage) );
        device.get()->renderText(damageText, auxPosX * SCREEN_WIDTH / 1920, auxPosY * SCREEN_HEIGHT / 1080, 1.f * SCREEN_WIDTH / 1920, glm::vec3(0.0f, 0.0f, 0.0f), true);
    }
}

void PapayaEngine::drawMap(GameEngine &gameContext, HUDComponent &HUD) const {
    InputComponent &input = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
    if (input.lookingMap) {
        MapComponent &map     = gameContext.entityMan.getComponent<MapComponent>(gameContext.mapId);



        // The Biome map is does not fill the whole screen, so
        // it is necessary to know how many pixels there's over
        // and down. This will depend on the SCREEN_HEIGHT. Once
        // we know it, we have to transform world position to
        // relative screen position
        float scaleFactor   =  0.89;
        float mapDiameter   =  SCREEN_HEIGHT * scaleFactor;
        float pixelsOverMap = (SCREEN_HEIGHT - mapDiameter) / 2;
        float pixelsByMap   = (SCREEN_WIDTH  - mapDiameter) / 2;

        float initX = 0;
        float endX = SCREEN_WIDTH;

        float responsiveWidth = (1920.f / 1080.f) * SCREEN_HEIGHT;
        initX -= (responsiveWidth - SCREEN_WIDTH) / 2.f;
        endX += (responsiveWidth - SCREEN_WIDTH);


        device.get()->draw2DImage(HUD.map, glm::vec2(initX, 0.f), glm::vec2(endX, SCREEN_HEIGHT));



        TerrainDataComponent  &terrainData  {gameContext.entityMan.getComponent<TerrainDataComponent>   (gameContext.terrainId)};
        float worldRadius = terrainData.lastBiome * terrainData.biomeSize + terrainData.baseSize;

        float posX = 0;
        float posZ = 0;

        // DISCOVERED AREAS
        uint16_t max = map.MAX_MAP_DIVISION;
        float areaSize = mapDiameter / max;

        for (size_t area = 0; area < map.discoveredAreas.size(); ++area) {
            if (!map.discoveredAreas.at(area)) {
                posX = (area % max) * areaSize;
                posX += pixelsByMap;
                posZ = (area / max) * areaSize;
                posZ += pixelsOverMap;
                device.get()->draw2DImage("./media/map/smoke.png", glm::vec2(posX, posZ), glm::vec2(areaSize, areaSize));

            }
        }




        device.get()->draw2DImage(HUD.mapBorders, glm::vec2(initX, 0.f), glm::vec2(endX, SCREEN_HEIGHT));


        for (uint32_t entity : map.discoveredEntities) {
            SituationComponent &sit = gameContext.entityMan.getComponent<SituationComponent>(entity);
            posX = sit.x + worldRadius;
            posZ = worldRadius - sit.z;

            posX *= mapDiameter;
            posX /= (worldRadius * 2);
            posZ *= mapDiameter;
            posZ /= (worldRadius * 2);

            posX += pixelsByMap - 32;
            posZ += pixelsOverMap - 32;

            // TEMP CHANGE THIS PLEASE
            auto entityTexture = "";
            switch (gameContext.getEntity(entity).getType()) {
            case DOOR:
                entityTexture = "./media/map/waypoint(door).png";
                break;
            case INTERESTPOINT:
                if (gameContext.entityMan.getComponent<IPDataComponent>(entity).isDiscoveredForPlayer)
                    entityTexture = "./media/map/waypoint(IP_Discover).png";
                else
                    entityTexture = "./media/map/waypoint(IP).png";
                break;
            case KEYEGG:
                if (gameContext.entityMan.getComponent<EggDataComponent>(entity).broken)
                    entityTexture = "./media/map/waypoint(brokenEgg).png";
                else
                    entityTexture = "./media/map/waypoint(egg).png";
                break;
            case WALL:
                entityTexture = "./media/map/waypoint(wall).png";
                break;
            default:
                entityTexture = "./media/map/waypoint(player).png";
                break;
            }
            device.get()->draw2DImage(entityTexture, glm::vec2(posX, posZ),
                                      glm::vec2(64.f, 64.f));
        }



    }
}

void PapayaEngine::drawStateHUD(GameEngine &gameContext) const {
    int8_t option = gameContext.stateOptionSelected;
    uint16_t dropedCoins = 0;
    std::string text;
    std::string coinText;

    switch (gameContext.getGameState()) {
    case PAUSE:
        device.get()->draw2DImage("./media/statesImages/background.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/statesImages/hand/pause.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        // SELECTOR
        if (!option)
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_1.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        else if (option == 1)
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_3.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        else
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_5.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        break;

    case GAMEOVER: {
        auto &playerStats = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
        if (playerStats.deadTime >= 2) {
            device.get()->draw2DImage("./media/statesImages/background.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            device.get()->draw2DImage("./media/statesImages/hand/gameOver.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_3.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

            dropedCoins = gameContext.dropedCoins;

            text = "Has perdido ";
            coinText = std::to_string( int(dropedCoins) );
            text += coinText;
            text += " cristales";


            device.get()->renderText(text, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);
        }
    }
    break;

    case EXIT:
        device.get()->draw2DImage("./media/statesImages/background.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/statesImages/hand/exit.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        // SELECTOR
        if (!option)
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_2.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        else
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_4.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        break;

    case WIN: {
        device.get()->draw2DImage("./media/statesImages/background.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        if (gameContext.finalDialogue == 5) {
            device.get()->draw2DImage("./media/statesImages/hand/win.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_3.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

            float playedTime = gameContext.playedTime;

            int hoursPlayed = playedTime / 3600;
            playedTime -= hoursPlayed * 3600;

            int minsPlayed = playedTime / 60;
            playedTime -= minsPlayed * 60;

            int secsPlayed = playedTime;

            std::string playedTimeText = "Has tardado ";
            playedTimeText += std::to_string( hoursPlayed);
            playedTimeText += " horas, ";
            playedTimeText += std::to_string( minsPlayed);
            playedTimeText += " minutos, ";
            playedTimeText += std::to_string( secsPlayed);
            playedTimeText += " segundos.";
            device.get()->renderText(playedTimeText, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 45, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);


            std::string deathsText = "Has muerto ";
            deathsText += std::to_string( int(gameContext.deaths));
            deathsText += " veces.";
            device.get()->renderText(deathsText, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 15, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);


            std::string totalDropedCoinsText = "Has perdido ";
            totalDropedCoinsText += std::to_string( int(gameContext.totalDropedCoins));
            totalDropedCoinsText += " cristales en total.";
            device.get()->renderText(totalDropedCoinsText, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 15, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);


            std::string enemiesDeathsText = "Has matado ";
            enemiesDeathsText += std::to_string( int(gameContext.enemiesDeaths));
            enemiesDeathsText += " enemigos en total.";
            device.get()->renderText(enemiesDeathsText, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 45, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);

        } else {
            std::string godMessage;
            std::string godName;
            glm::vec3 color;

            if (gameContext.finalDialogue == 4) {
                godName = "RENCOR:";
                color = {255.f / 255.f, 0.f / 255.f, 0.f / 255.f}; // Rojo
                godMessage = "\"No tienes mi perdón. Vuelve a intentarlo\"";
            } else if (gameContext.finalDialogue == 3) {
                godName = "ESPERANZA:";
                color = {0.f / 255.f, 255.f / 255.f, 0.f / 255.f}; // Verde
                godMessage = "\"Ningún sentimiento es final\"";
            } else if (gameContext.finalDialogue == 2) {
                godName = "VENGANZA:";
                color = {255.f / 255.f, 232.f / 255.f, 100.f / 255.f}; // Dorado
                godMessage = "\"Te enviamos al mundo que construimos juntos para que probases tu propia medicina...\"";
                std::string littleMessage = "\"El desgraciado ha sobrevivido...\"";
                device.get()->renderText(littleMessage, SCREEN_WIDTH / 2, (30 * SCREEN_HEIGHT / 1080) + (SCREEN_HEIGHT / 2), 0.5f * SCREEN_WIDTH / 1920, color, true);
            } else if (gameContext.finalDialogue == 1) {
                godName = "ENSEÑANZA:";
                color = {0.f / 255.f, 160.f / 255.f, 255.f / 255.f}; // Azul
                godMessage = "\"Ha sido un camino muy duro, pero estoy seguro de que ha merecido la pena\"";
            } else if (gameContext.finalDialogue == 0) {
                godName = "SIMPATÍA:";
                color = {200.f / 255.f, 200.f / 255.f, 200.f / 255.f}; // Gris
                godMessage = "\"Me alegra verte de nuevo, pensaba que no lo conseguirías\"";
            }

            device.get()->renderText(godMessage, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2), 1.f * SCREEN_WIDTH / 1920, color, true);

            color = {255.f / 255.f, 255.f / 255.f, 255.f / 255.f}; // Blanco
            device.get()->renderText(godName, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (40 * SCREEN_HEIGHT / 1080), 1.3f * SCREEN_WIDTH / 1920, color, true);
        }

        break;
    }

    case LOADING:
        if (gameContext.loadingProcess < 100) {
            device.get()->draw2DImage("./media/statesImages/hand/loading.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            std::string loadingText = gameContext.loadingText;
            loadingText += std::to_string( int(gameContext.loadingProcess) );
            loadingText += "%)";
            device.get()->renderText(loadingText, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.f * SCREEN_WIDTH / 1920, glm::vec3(1.f, 1.f, 1.f), true);

        } else {
            device.get()->draw2DImage("./media/statesImages/hand/loadingComplete.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_3.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        }
        break;

    case MAINMENU:
        //device.get()->draw2DImage("./media/statesImages/background.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/statesImages/hand/mainMenu.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        // SELECTOR
        if (!option)
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_1.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        else if (option == 1)
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_3.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        else
            device.get()->draw2DImage("./media/statesImages/hand/selector/selector_5.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

        break;
    }
}


void PapayaEngine::drawDialogues(GameEngine &gameContext) const {
    auto &lastDialogueID = gameContext.lastDialogueID;
    if(lastDialogueID != 0) {
        DialogueComponent &dialogue = gameContext.entityMan.getComponent<DialogueComponent>(lastDialogueID);
        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(lastDialogueID);

        if(dialogue.dialogueActive && dialogue.dialogueTimer > 0) {
            float rotationY = updateElement2DLookAtCamera(gameContext, situation, gameContext.cameraId);
            device.get()->renderText3d(dialogue.sentences[dialogue.currentSentence].sentence, glm::vec3(situation.x, situation.y + 50.f, -situation.z), glm::vec3(0.f, rotationY, 0.f), 0.25f, glm::vec3(1.f, 1.f, 1.f), "./media/boxes/speechBubble/speechBubble.png");

            dialogue.dialogueTimer = dialogue.dialogueTimer - 0.01;

        } else if(dialogue.dialogueActive && dialogue.dialogueTimer <= 0)
            dialogue.dialogueActive = false;
    }
}

void PapayaEngine::drawInfoInteroperable(GameEngine &gameContext) const {

    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(gameContext.playerId);

    if(playerInteraction.entityInteractibleId != 0) {

        auto eType = gameContext.entityMan.getEntity(playerInteraction.entityInteractibleId).getType();

        switch(eType) {

        case NPC:
            if(playerInteraction.entityInteractibleId != gameContext.npcShamanImprover)
                device.get()->draw2DImage("./media/hud/interoperable/interoperable_talk_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case BANK_DEPOSITOR:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_saveCristals_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case BANK_EXTRACTOR:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_shakeDown_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case KEY:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_getKey_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case MELEEWEAPON:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_getWeapon_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case DISTANCEWEAPON:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_getWeapon_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case DOOR:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_openDoor_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;

        case CHEST:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_openChest_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;
        case PALANTIR:
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_takePalantir_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
            break;
        }

        if(playerInteraction.entityInteractibleId == gameContext.npcShamanImprover)
            device.get()->draw2DImage("./media/hud/interoperable/interoperable_levelUp_fullScreen.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    }

}


void PapayaEngine::drawInfoActions(GameEngine &gameContext) const {

    auto &playerId = gameContext.playerId;
    auto &playerSecure = gameContext.entityMan.getComponent<AtributesComponent>(playerId);
    auto &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(playerId);
    auto &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(playerId);
    auto &playerLevel = gameContext.entityMan.getComponent<LevelComponent>(playerId);

    std::string resource = "";

    //ATTACK ACTION
    if(playerInventory.primaryWeaponId != 0 && !gameContext.desactiveInfoActions[0]) {
        resource = "./media/hud/interoperable/action_walk_fullScreen.png";
    }
    //ATTACK ACTION
    if(!playerSecure.isSecure && playerInventory.primaryWeaponId != 0 && gameContext.desactiveInfoActions[0] && !gameContext.desactiveInfoActions[1]) {
        resource = "./media/hud/interoperable/action_attack_fullScreen.png";
    }

    if(playerLevel.compassUnlocked && !gameContext.desactiveInfoActions[2]) {
        resource = "./media/hud/interoperable/action_compass_fullScreen.png";
    }

    if(!playerSecure.isSecure && playerLevel.dashUnlocked != 0 && !gameContext.desactiveInfoActions[3]) {
        resource = "./media/hud/interoperable/action_dash_fullScreen.png";
    }

    if(playerLevel.mapUnlocked && !gameContext.desactiveInfoActions[4]) {
        resource = "./media/hud/interoperable/action_map_fullScreen.png";
    }

    if(playerInventory.secondaryWeaponId != 0 && !gameContext.desactiveInfoActions[5]) {
        resource = "./media/hud/interoperable/action_changeWeapon_fullScreen.png";
    }

    if(!gameContext.palantirId && !gameContext.desactiveInfoActions[6]) {
        resource = "./media/hud/interoperable/action_dropPalantir_fullScreen.png";
    }

    if (resource != "")
        device.get()->draw2DImage(resource, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void PapayaEngine::drawInfoNotifications(GameEngine &gameContext) const {

    std::string resource = "";
    if(gameContext.activeInfoNotifications[0]) {
        resource = "./media/hud/notifications/notifications_saveCristals_fullScreen.png";
    } else if(gameContext.activeInfoNotifications[1]) {
        resource = "./media/hud/notifications/notifications_shakeDown_fullScreen.png";
    } else if(gameContext.activeInfoNotifications[2]) {
        resource = "./media/hud/notifications/notifications_noCristals_fullScreen.png";
    } else if(gameContext.activeInfoNotifications[3]) {
        resource = "./media/hud/notifications/notifications_openDoor_fullScreen.png";
    }

    if (resource != "")
        device.get()->draw2DImage(resource, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}




void PapayaEngine::drawTutorialPhases(GameEngine &gameContext) const {


    if (gameContext.tutorialPhases == 3) {
        device.get()->draw2DImage("./media/hud/tutorial/tutorial_interoperable.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/hud/tutorial/background_health.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        HUDComponent &HUD = gameContext.entityMan.getComponent<HUDComponent>(gameContext.HUDId);
        drawHealth(gameContext, HUD);
        drawLevel(gameContext, HUD);

    } else if (gameContext.tutorialPhases == 4) {
        device.get()->draw2DImage("./media/hud/tutorial/background_health.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        HUDComponent &HUD = gameContext.entityMan.getComponent<HUDComponent>(gameContext.HUDId);
        drawHealth(gameContext, HUD);
        drawLevel(gameContext, HUD);
        device.get()->draw2DImage("./media/hud/tutorial/tutorial_interoperable.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/hud/tutorial/background_strength.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        drawStats(gameContext, HUD);

    } else if (gameContext.tutorialPhases == 5) {
        device.get()->draw2DImage("./media/hud/tutorial/background_health.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        HUDComponent &HUD = gameContext.entityMan.getComponent<HUDComponent>(gameContext.HUDId);
        drawHealth(gameContext, HUD);
        drawLevel(gameContext, HUD);
        device.get()->draw2DImage("./media/hud/tutorial/background_strength.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        drawStats(gameContext, HUD);
        device.get()->draw2DImage("./media/hud/tutorial/tutorial_interoperable.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/hud/tutorial/background_coins.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        drawWalletCoins(gameContext);

    } else if (gameContext.tutorialPhases == 6 || gameContext.tutorialPhases == 7) {
        device.get()->draw2DImage("./media/hud/tutorial/tutorial_interoperable.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        device.get()->draw2DImage("./media/hud/tutorial/background_health.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        HUDComponent &HUD = gameContext.entityMan.getComponent<HUDComponent>(gameContext.HUDId);
        drawHealth(gameContext, HUD);
        drawLevel(gameContext, HUD);
        device.get()->draw2DImage("./media/hud/tutorial/background_strength.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        drawStats(gameContext, HUD);
        device.get()->draw2DImage("./media/hud/tutorial/background_coins.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
        drawWalletCoins(gameContext);

    } else {
        device.get()->draw2DImage("./media/hud/tutorial/tutorial_interoperable.png", glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));

    }
}


void PapayaEngine::updateWeapons2DTexture(GameEngine &gameContext) {


    uint32_t playerID = gameContext.playerId;
    InventoryComponent &playerInventory = gameContext.entityMan.getComponent<InventoryComponent>(playerID);

    if(playerInventory.primaryWeaponId != 0) {
        Drawable2DComponent &weaponDrawable2D = gameContext.entityMan.getComponent<Drawable2DComponent>(playerInventory.primaryWeaponId);
        weaponsPrimaryTexture = weaponDrawable2D.texture2D;
    } else {
        weaponsPrimaryTexture = "";
    }

    if(playerInventory.secondaryWeaponId != 0) {
        Drawable2DComponent &weaponSecondaryDrawable2D = gameContext.entityMan.getComponent<Drawable2DComponent>(playerInventory.secondaryWeaponId);
        weaponsSecondaryTexture = weaponSecondaryDrawable2D.secondTexture2D;
    } else {
        weaponsSecondaryTexture = "";
    }
}

void PapayaEngine::drawBiomeThings(GameEngine &gameContext) const {
    // DRAW BIOME NAME
    auto currentBiome = gameContext.biomeInAnimation;
    std::string biomeName = "./media/hud/biomes/B";
    biomeName += std::to_string( int(currentBiome) );
    biomeName += ".png";
    if (gameContext.timeInBiomes.at(currentBiome) <= 3) {
        device.get()->draw2DImage(biomeName, glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
    } else {
        auto &input = gameContext.entityMan.getComponent<InputComponent>(gameContext.playerId);
        if (input.lookingStats) {
            device.get()->draw2DImage(biomeName, glm::vec2(2 * SCREEN_WIDTH / 3, 0.f), glm::vec2(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3));
        }
    }
}

void PapayaEngine::updateEntities(GameEngine &gameContext, std::vector<uint32_t> entitiesId) {

    for (uint8_t i = 0; i < entitiesId.size(); ++i) {

        if(existsNode(entitiesId.at(i))) {
            if(gameContext.entityMan.existsComponent<DrawableComponent>(entitiesId.at(i)) ) { //(Yupipi add this check 07-may-20)
                if(gameContext.entityMan.existsComponent<SituationComponent>(entitiesId.at(i)) ) { //(Yupipi add this check 07-may-20)
                    SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(entitiesId.at(i));
                    DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(entitiesId.at(i));

                    // Acces to the node
                    //nodeMap[entitiesId.at(i)]->setTranslation( core::vector3df(situation.x,situation.y + (drawable.scaleY * 10) / 2, situation.z) );

                    //Set Position
                    if(drawable.model == "") {
                        nodeMap[entitiesId.at(i)]->setTranslation(glm::vec3(situation.x, situation.y + drawable.scaleY * 5.f, situation.z));
                    } else {
                        nodeMap[entitiesId.at(i)]->setTranslation(glm::vec3(situation.x, situation.y, situation.z));
                    }

                    if(drawable.model == "") {
                        nodeMap[entitiesId.at(i)]->setRotation(glm::vec3(0, 0, 0));
                    } else {
                        nodeMap[entitiesId.at(i)]->setRotation(glm::vec3(0, situation.rotation - 180, 0));
                    }

                    nodeMap[entitiesId.at(i)]->setScale(glm::vec3(drawable.scaleX, drawable.scaleY, drawable.scaleZ));
                }

            }

        }
    }
}


void PapayaEngine::updateTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) {
    for (uint8_t i = 0; i < entitiesId.size(); ++i) {

        if(gameContext.entityMan.existsComponent<DrawableComponent>(entitiesId.at(i)) ) { //(Yupipi add this check 07-may-20)
            //Get Drawable
            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(entitiesId.at(i));
            //Set Texture

            //#### -- PARA QUE NO INTENTE CARGAR TEXTURAS VACIAS "" -- ####//
            if(drawable.texture != "") {
                if(existsNode(entitiesId.at(i))) {
                    device.get()->setTextureSceneNode(nodeMap[entitiesId.at(i)], drawable.texture, drawable.transparency);
                }
            }
        }
    }
}

// VERY SPENSIVE PERFORMACE
void PapayaEngine::updateModel(GameEngine &gameContext, uint32_t entityId) {
    eraseEntity(entityId);
    createEntity(gameContext, entityId);
}

void PapayaEngine::updateTimeFrame(GameEngine &gameContext, uint32_t entityId, float timeFrame) {
    static_cast<pap::EntityAnimation *>(nodeMap[entityId]->getEntity())->timeFrame = timeFrame;
}

void PapayaEngine::setAnimationLoop(GameEngine &gameContext, uint32_t entityId, bool loop) {
    static_cast<pap::EntityAnimation *>(nodeMap[entityId]->getEntity())->animationLoop = loop;
}

void PapayaEngine::updateTerrainCubeTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) {

    for (uint8_t i = 0; i < entitiesId.size(); ++i) {
        if(gameContext.entityMan.existsComponent<DrawableComponent>(entitiesId.at(i)) ) { //(Yupipi add this check 07-may-20)

            //Get Drawable
            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(entitiesId.at(i));
            //Set Texture

            //#### -- PARA QUE NO INTENTE CARGAR TEXTURAS VACIAS "" -- ####//
            if(drawable.texture != "") {
                device.get()->setTextureSceneNode(nodeMap[entitiesId.at(i)], drawable.texture, drawable.transparency);
            }
        }
    }
}

void PapayaEngine::updateCamera(GameEngine &gameContext, SituationComponent &cameraSituation, SituationComponent &playerSituation) const {
    pap::SceneNode *cameraNode = device.get()->getActiveCameraNode();

    if(cameraNode) {
        cameraNode->setTranslation(glm::vec3(cameraSituation.x, cameraSituation.y + 100, cameraSituation.z));
        cameraNode->setRotation(glm::vec3(0, 0, 0));

        static_cast<pap::EntityCamera *>(cameraNode->getEntity())->cameraTarget = glm::vec3(playerSituation.x, playerSituation.y, -playerSituation.z);

    }
}

void PapayaEngine::updateBoxLookAtCamera(GameEngine &gameContext, uint32_t boxId, uint32_t cameraId) {
    SituationComponent &cameraSituation = gameContext.entityMan.getComponent<SituationComponent>(cameraId);
    SituationComponent &boxSituation = gameContext.entityMan.getComponent<SituationComponent>(boxId);

    float xdistance = boxSituation.x - cameraSituation.x;
    float ydistance = boxSituation.y - cameraSituation.y;
    float zdistance = boxSituation.z - cameraSituation.z;

    float angleYZ = Utils::radToDeg(atan2(ydistance, zdistance));
    float angleXZ = Utils::radToDeg(atan2(xdistance, zdistance));
    //float angleXY = Utils::radToDeg(atan2(xdistance, ydistance));

    /*if(angleXZ < -50) {
        angleXZ = -50;
    } else if(angleXZ > 50) {
        angleXZ = 50;
    }*/

    if(existsNode(boxId)) {
        nodeMap[boxId]->setRotation(glm::vec3(angleYZ - 90, angleXZ, 0));
    }
}

void PapayaEngine::updateDialogueLookAtCamera(GameEngine &gameContext, uint32_t cameraId) {
    auto &lastDialogueID = gameContext.lastDialogueID;
    if(lastDialogueID != 0) {

        if(gameContext.entityMan.existsComponent<DialogueComponent>(lastDialogueID) ) { //(Yupipi add this check 07-may-20)
            DialogueComponent &dialogue = gameContext.entityMan.getComponent<DialogueComponent>(lastDialogueID);

            if(dialogue.dialogueActive) {

                if(gameContext.entityMan.existsComponent<SituationComponent>(cameraId) ) { //(Yupipi add this check 07-may-20)
                    if(gameContext.entityMan.existsComponent<SituationComponent>(lastDialogueID) ) { //(Yupipi add this check 07-may-20)

                        SituationComponent &cameraSituation = gameContext.entityMan.getComponent<SituationComponent>(cameraId);
                        SituationComponent &dialogueSituation = gameContext.entityMan.getComponent<SituationComponent>(lastDialogueID);

                        float xdistance = dialogueSituation.x - cameraSituation.x;
                        float ydistance = dialogueSituation.y - cameraSituation.y;
                        float zdistance = -dialogueSituation.z + cameraSituation.z;

                        float angleYZ = Utils::radToDeg(atan2(ydistance, zdistance));
                        float angleXZ = Utils::radToDeg(atan2(xdistance, zdistance));
                        float angleXY = Utils::radToDeg(atan2(xdistance, ydistance));

                        /*if(angleXZ < -50) {
                            angleXZ = -50;
                        } else if(angleXZ > 50) {
                            angleXZ = 50;
                        }*/

                        device.get()->getTextRenderer().setDialogueRotation(glm::vec3(0.f, angleXZ + 180, 0.f));
                    }
                }
            }
        }
    }
}


float PapayaEngine::updateElement2DLookAtCamera(GameEngine &gameContext, SituationComponent &situation, uint32_t cameraId) const {
    float angleXZ = 0.f;
    if(gameContext.entityMan.existsComponent<SituationComponent>(cameraId) ) { //(Yupipi add this check 07-may-20)
        SituationComponent &cameraSituation = gameContext.entityMan.getComponent<SituationComponent>(cameraId);

        float xdistance = situation.x - cameraSituation.x;
        float zdistance = -situation.z + cameraSituation.z;

        angleXZ = Utils::radToDeg(atan2(xdistance, zdistance));
    }
    return angleXZ + 180;
}


/*scene::IAnimatedMeshSceneNode *PapayaEngine::loadModel(std::string filename) {
    scene::IAnimatedMeshSceneNode *model = nullptr;
    scene::IAnimatedMesh *m = device->getSceneManager()->getMesh(filename.data());
    if (!m) {
        return nullptr;
    }
    model = device->getSceneManager()->addAnimatedMeshSceneNode(m);
    model->setAnimationSpeed(30);
    //Set default material properties
    //model->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR); to Spectrum
    model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    model->setDebugDataVisible(false);
    return model;
}*/


//******************This method is here and PapayaDevice (line 476). Move to Utils******************

std::vector<std::string> PapayaEngine::splitIterator(const std::string &str, const char separator) {
    std::vector<std::string> result;

    std::string::const_iterator itBegin = str.begin();
    std::string::const_iterator itEnd   = str.end();

    int numItems = 1;
    for( std::string::const_iterator it = itBegin; it != itEnd; ++it )
        numItems += *it == separator;

    result.reserve(numItems);

    for( std::string::const_iterator it = itBegin; it != itEnd; ++it ) {
        if( *it == separator ) {
            result.push_back(std::string(itBegin, it));
            itBegin = it + 1;
        }
    }

    if( itBegin != itEnd )
        result.push_back(std::string(itBegin, itEnd));

    return result;
}


void PapayaEngine::createEntity(GameEngine &gameContext, uint32_t id) {

    if(gameContext.entityMan.existsComponent<DrawableComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)

            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(id);
            SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);

            pap::SceneNode *entity = nullptr;

            if(drawable.model == "") {
                std::string text = drawable.texture;
                // load a cube in the engine

                entity = device.get()->addGeometrySceneNode(nullptr, "cube", text, glm::vec3(situation.x, situation.y + drawable.scaleY * 5.f, situation.z), glm::vec3(0, situation.rotation + 180, 0), glm::vec3(drawable.scaleX, drawable.scaleY, drawable.scaleZ), drawable.transparency);

            } else if(splitIterator(drawable.model, '.').back() == "obj") {
                // load a model into the engine
                entity = device.get()->addModelSceneNode(nullptr, drawable.model, glm::vec3(situation.x, situation.y, situation.z), glm::vec3(0, situation.rotation + 180, 0), glm::vec3(drawable.scaleX, drawable.scaleY, drawable.scaleZ), drawable.transparency, drawable.hasShadow);
            } else {
                // load a model into the engine
                entity = device.get()->addAnimationSceneNode(nullptr, drawable.model, glm::vec3(situation.x, situation.y, situation.z), glm::vec3(0, situation.rotation + 180, 0), glm::vec3(drawable.scaleX, drawable.scaleY, drawable.scaleZ), drawable.transparency, drawable.hasShadow);
            }

            if(!existsNode(id))
                nodeMap[id] = entity;

        }
    }
}

void PapayaEngine::createPlaneEntity(GameEngine &gameContext, uint32_t id, std::string type) {

    if(gameContext.entityMan.existsComponent<DrawableComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)
            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(id);
            SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);

            pap::SceneNode *entity = nullptr;

            std::string text = drawable.texture;
            // load a cube in the engine
            entity = device.get()->addGeometrySceneNode(nullptr, type, text, glm::vec3(situation.x, situation.y + drawable.scaleY * 5.f, situation.z), glm::vec3(situation.rotation + 180, 0.f, 0.f), glm::vec3(drawable.scaleX, drawable.scaleY, drawable.scaleZ), drawable.transparency, drawable.hasShadow);

            if(!existsNode(id))
                nodeMap[id] = entity;

        }
    }
}

void PapayaEngine::eraseEntity(uint32_t id) {
    if(existsNode(id)) {
        device.get()->deleteSceneNode(nodeMap[id]);
        nodeMap.erase(id);
    }
}

bool PapayaEngine::existsNode(uint32_t id) const {
    if(nodeMap.find(id) == nodeMap.end())
        return false;
    return true;
}

int PapayaEngine::windowWidth() const {
    return device.get()->getWindow()->windowWidth;
}

int PapayaEngine::windowHeight() const {
    return device.get()->getWindow()->windowHeight;
}

void PapayaEngine::createCamera(GameEngine &gameContext, uint32_t id) const {
    if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)

        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);
        device.get()->addCameraSceneNode(nullptr, glm::vec3(situation.x, situation.y, situation.z), glm::vec3(0.f, 0.f, 0.f));
    }
}


size_t PapayaEngine::countRenderNodes() const {
    return nodeMap.size();
}

void PapayaEngine::goOverDir(std::string path) const {
    for (const auto &entry : fs::directory_iterator(path)) {
        std::string u8Path = entry.path().u8string();
        if (entry.is_directory()) {
            u8Path += "/";
            goOverDir(u8Path);
        } else {
            if (entry.path().extension() == ".obj")
                device.get()->getResourceManager().getResourceModel(u8Path);
        }
    }
}

void PapayaEngine::loadHUD(uint8_t percent) const {
    std::vector<std::string> directories;
    directories.push_back("./media/hud/");
    directories.push_back("./media/hud/biomes/");
    directories.push_back("./media/hud/enemies/");
    directories.push_back("./media/hud/interoperable/");
    directories.push_back("./media/hud/notifications/");
    directories.push_back("./media/hud/tutorial/");
    directories.push_back("./media/hud/weapons/");
    directories.push_back("./media/statesImages/");
    directories.push_back("./media/statesImages/hand/");
    directories.push_back("./media/statesImages/hand/selector/");
    directories.push_back("./media/map/");

    uint8_t lastResource = directories.size() - 1;
    uint8_t whichOne = percent * lastResource / 99; // the max percent here is 99%. 100 would be all already loaded
    auto whatToLoad = directories[whichOne];

    for (const auto &entry : fs::directory_iterator(whatToLoad)) {
        std::string u8Path = entry.path().u8string();

        if (entry.path().extension() == ".png")
            device.get()->draw2DImage(u8Path, glm::vec2(0, 0), glm::vec2(0, 0));
    }
}

void PapayaEngine::loadModels(uint8_t percent) const {
    std::string path = "./media/";
    std::vector<std::string> directories;

    for (const auto &entry : fs::directory_iterator(path)) {
        directories.emplace_back(entry.path().u8string() + "/");
    }

    uint8_t lastResource = directories.size() - 1;
    uint8_t whichOne = percent * lastResource / 99; // the max percent here is 99%. 100 would be all already loaded
    auto whatToLoad = directories[whichOne];

    goOverDir(whatToLoad);
}

void PapayaEngine::createDirectionalLight(GameEngine &gameContext, uint32_t id) {
    if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<LightComponent>(id) ) { //(Yupipi add this check 07-may-20)

            SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);
            LightComponent     &lightComp = gameContext.entityMan.getComponent<LightComponent>(id);

            pap::SceneNode *entity = nullptr;

            entity = device.get()->addDirectionalLightSceneNode(nullptr, glm::vec3(situation.x, situation.y, -situation.z)
                     , glm::vec3(lightComp.color[0], lightComp.color[1], lightComp.color[2])
                     , glm::vec3(lightComp.direction[0], lightComp.direction[1], lightComp.direction[2]));

            if(!existsNode(id) && entity)
                nodeMap[id] = entity;

        }
    }
}

void PapayaEngine::createPointLight(GameEngine &gameContext, uint32_t id) {
    if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<LightComponent>(id) ) { //(Yupipi add this check 07-may-20)

            SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(id);
            LightComponent     &lightComp = gameContext.entityMan.getComponent<LightComponent>(id);

            pap::SceneNode *entity = nullptr;

            entity = device.get()->addPointLightSceneNode(nullptr, glm::vec3(situation.x, situation.y, situation.z)
                     , glm::vec3(lightComp.color[0], lightComp.color[1], lightComp.color[2])
                     , lightComp.constant, lightComp.linear, lightComp.quadratic);

            if(!existsNode(id) && entity)
                nodeMap[id] = entity;

        }
    }
}


void PapayaEngine::updateDirectionalLight(GameEngine &gameContext, uint32_t id) {

    if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<LightComponent>(id) ) { //(Yupipi add this check 07-may-20)

            SituationComponent &lightSit  = gameContext.entityMan.getComponent<SituationComponent>(id);
            LightComponent     &lightComp = gameContext.entityMan.getComponent<LightComponent>(id);

            pap::SceneNode *lightNode = nodeMap[id];

            if(lightNode) {
                // Update position
                lightNode->setTranslation(glm::vec3(lightSit.x, lightSit.y, lightSit.z));

                // Update light parameters
                pap::LightDirectional *dirLightEntity = static_cast<pap::LightDirectional *>( lightNode->getEntity() );
                if(dirLightEntity) {
                    dirLightEntity->color     = glm::vec3(lightComp.color[0], lightComp.color[1],  lightComp.color[2]);
                    dirLightEntity->direction = glm::vec3(lightComp.direction[0], lightComp.direction[1], -lightComp.direction[2]);
                    dirLightEntity->objective = glm::vec3(lightComp.objective[0], lightComp.objective[1], -lightComp.objective[2]);
                }
            }
        }
    }
}

void PapayaEngine::updatePointLight(GameEngine &gameContext, uint32_t id) {

    if(gameContext.entityMan.existsComponent<SituationComponent>(id) ) { //(Yupipi add this check 07-may-20)
        if(gameContext.entityMan.existsComponent<LightComponent>(id) ) { //(Yupipi add this check 07-may-20)
            SituationComponent &lightSit  = gameContext.entityMan.getComponent<SituationComponent>(id);
            LightComponent     &lightComp = gameContext.entityMan.getComponent<LightComponent>(id);

            pap::SceneNode *lightNode = nodeMap[id];

            if(lightNode) {
                // Update position
                lightNode->setTranslation(glm::vec3(lightSit.x, lightSit.y, lightSit.z));

                // Update light parameters
                pap::LightPoint *pointLightEntity = static_cast<pap::LightPoint *>( lightNode->getEntity() );
                if(pointLightEntity) {
                    pointLightEntity->color = glm::vec3(lightComp.color[0], lightComp.color[1],  lightComp.color[2]);

                    pointLightEntity->constant  = lightComp.constant;
                    pointLightEntity->linear    = lightComp.linear;
                    pointLightEntity->quadratic = lightComp.quadratic;
                }
            }

        }
    }
}

void PapayaEngine::addColorToEntity(uint32_t id, std::array<float, 3> color) {
    if(existsNode(id))
        nodeMap[id]->color = glm::vec3(color[0], color[1], color[2]);
}