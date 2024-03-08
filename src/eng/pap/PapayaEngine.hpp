#pragma once

#include <fac/WindowFacade.hpp>
#include <PapayaEngine/PapayaDevice.hpp>
#include <PapayaEngine/Engine/LightPoint.hpp>
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>


class PapayaEngine : public WindowFacade {
public:
    PapayaEngine(int width, int height, bool windowsMode);
    ~PapayaEngine();

    const bool checkInput(const uint16_t key) const override;
    std::array<float, 2> getMousePosition() const override;
    std::array<float, 3> getProjectedCursor() const override;
    bool getLeftClick() const override;
    bool getRightClick() const override;
    bool getMiddleClick() const override;
    void resetScroll() const override;
    bool getUpScroll() const override;
    bool getDownScroll() const override;
    void render(GameEngine &gameContext) const override;
    void updateEntities(GameEngine &gameContext, std::vector<uint32_t> entitiesId) override;
    void updateTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) override;
    void updateModel(GameEngine &gameContext, uint32_t entityId) override;// VERY SPENSIVE PERFORMACE
    void updateTimeFrame(GameEngine &gameContext, uint32_t entityId, float timeFrame) override;
    void setAnimationLoop(GameEngine &gameContext, uint32_t entityId, bool loop) override;
    void updateTerrainCubeTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) override;
    void updateWeapons2DTexture(GameEngine &gameContext) override;
    void updateCamera(GameEngine &gameContext, SituationComponent &cameraSituation, SituationComponent &playerSituation) const override;
    void updateBoxLookAtCamera(GameEngine &gameContext, uint32_t boxId, uint32_t cameraId) override;
    float updateElement2DLookAtCamera(GameEngine &gameContext, SituationComponent &situation, uint32_t cameraId) const override;
    void updateDialogueLookAtCamera(GameEngine &gameContext, uint32_t cameraId) override;
    void createEntity(GameEngine &gameContext, uint32_t id) override;
    void createPlaneEntity(GameEngine &gameContext, uint32_t id, std::string type) override;
    void eraseEntity(uint32_t id) override;
    void createCamera(GameEngine &gameContext, uint32_t id) const override;
    void createDirectionalLight(GameEngine &gameContext, uint32_t id) override;
    void createPointLight(GameEngine &gameContext, uint32_t id) override;
    void updateDirectionalLight(GameEngine &gameContext, uint32_t id) override;
    void updatePointLight(GameEngine &gameContext, uint32_t id) override;
    void addColorToEntity(uint32_t id, std::array<float, 3> color) override;
    size_t countRenderNodes() const override;
    bool existsNode(uint32_t id) const override;

    int windowWidth() const override;
    int windowHeight() const override;

    void goOverDir(std::string path) const;
    void loadModels(uint8_t percent) const override;
    void loadHUD(uint8_t percent) const override;

private:
    void drawKeys(GameEngine &gameContext, HUDComponent &HUD) const;


    void drawDialogues(GameEngine &gameContext) const;
    void drawStats(GameEngine &gameContext, HUDComponent &HUD) const;

    void drawHealthHighlights(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawHudBackground(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawHealth(GameEngine &gameContext, HUDComponent &HUD) const;

    void drawEnemiesHealth(GameEngine &gameContext) const;
    void drawEggsHealth(GameEngine &gameContext) const;

    float calculateHealth(uint32_t currentHealth, uint32_t maxHealth) const;

    void drawWeapons(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawWalletCoins(GameEngine &gameContext) const;
    void drawLevel(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawDash(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawEffects(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawInfoInteroperable(GameEngine &gameContext) const;
    void drawInfoActions(GameEngine &gameContext) const;
    void drawInfoNotifications(GameEngine &gameContext) const;

    void drawTutorialPhases(GameEngine &gameContext) const;

    void drawMap(GameEngine &gameContext, HUDComponent &HUD) const;
    void drawBiomeThings(GameEngine &gameContext) const;

    void drawStateHUD(GameEngine &gameContext) const;
    std::vector<std::string> splitIterator(const std::string &str, const char separator);

    int asciiToGLFW(const uint8_t key) const;


    std::unique_ptr<pap::PapayaDevice> device;
    std::unordered_map<uint32_t, pap::SceneNode *> nodeMap;


    // TODO CHANGE THIS IN GAME
    std::string weaponsPrimaryTexture = "";
    std::string weaponsSecondaryTexture = "";

    int SCREEN_WIDTH = 1920; //Machine.Target 1280*800
    int SCREEN_HEIGHT = 1080;
};