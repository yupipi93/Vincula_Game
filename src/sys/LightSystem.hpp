#pragma once

#include "System.hpp"


class LightSystem : public System {
public:
    LightSystem();
    ~LightSystem();

    void update(GameEngine &gameContext) const override;

private:

    void updateCristalLights(GameEngine &gameContext) const;
    void updateCristalLightColor(GameEngine &gameContext, SecureZoneComponent &secureZoneComp) const;

    void updateDirectionalLight(GameEngine &gameContext) const;
    void updatePosition(GameEngine &gameContext, uint32_t dirLightID) const;
    void updateDirection(GameEngine &gameContext, uint32_t dirLightID) const;
	void updateColor(GameEngine &gameContext, uint32_t dirLightID) const;
};