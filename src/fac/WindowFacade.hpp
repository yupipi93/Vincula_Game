#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <unordered_map>
#include <com/ComponentsIncluder>

class GameEngine;


class WindowFacade {
public:
    virtual ~WindowFacade() = default;

    virtual const bool checkInput(const uint16_t key) const = 0;
    virtual std::array<float, 2> getMousePosition() const = 0;
    virtual std::array<float, 3> getProjectedCursor() const = 0;
    virtual bool getLeftClick() const = 0;
    virtual bool getRightClick() const = 0;
    virtual bool getMiddleClick() const = 0;
    virtual void resetScroll() const = 0;
    virtual bool getUpScroll() const = 0;
    virtual bool getDownScroll() const = 0;
    virtual void render(GameEngine &gameContext) const = 0;
    virtual void updateEntities(GameEngine &gameContext, std::vector<uint32_t> entitiesId) = 0;
    virtual void updateTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) = 0;
    virtual void updateModel(GameEngine &gameContext, uint32_t entityId) = 0;// VERY SPENSIVE PERFORMACE
    virtual void updateTimeFrame(GameEngine &gameContext, uint32_t entityId, float timeFrame) = 0;
    virtual void setAnimationLoop(GameEngine &gameContext, uint32_t entityId, bool loop) = 0;
    virtual void updateTerrainCubeTextures(GameEngine &gameContext, std::vector<uint32_t> entitiesId) = 0;
    virtual void updateWeapons2DTexture(GameEngine &gameContext) = 0;
    virtual void updateCamera(GameEngine &gameContext, SituationComponent &cameraSituation, SituationComponent &playerSituation) const = 0;
    virtual void updateBoxLookAtCamera(GameEngine &gameContext, uint32_t boxId, uint32_t cameraId) = 0;
    virtual void updateDialogueLookAtCamera(GameEngine &gameContext, uint32_t cameraId) = 0;
    virtual float updateElement2DLookAtCamera(GameEngine &gameContext, SituationComponent &situation, uint32_t cameraId) const = 0;
    virtual void createEntity(GameEngine &gameContext, uint32_t id) = 0;
    virtual void createPlaneEntity(GameEngine &gameContext, uint32_t id, std::string type) = 0;
    virtual void eraseEntity(uint32_t id) = 0;
    virtual void createCamera(GameEngine &gameContext, uint32_t id) const = 0;
    virtual void createDirectionalLight(GameEngine &gameContext, uint32_t id) = 0;
    virtual void createPointLight(GameEngine &gameContext, uint32_t id) = 0;
    virtual void updateDirectionalLight(GameEngine &gameContext, uint32_t id) = 0;
    virtual void updatePointLight(GameEngine &gameContext, uint32_t id) = 0;
    virtual void addColorToEntity(uint32_t id, std::array<float, 3> color) = 0;
    virtual size_t countRenderNodes() const = 0;
    virtual bool existsNode(uint32_t id) const = 0;

    virtual void drawInfoInteroperable(GameEngine &gameContext) const = 0;
    virtual void drawInfoActions(GameEngine &gameContext) const = 0;


    virtual int windowWidth() const = 0;
    virtual int windowHeight() const = 0;

    virtual void loadModels(uint8_t percent) const = 0;
    virtual void loadHUD(uint8_t percent) const = 0;



};
