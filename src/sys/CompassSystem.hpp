#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CompassSystem: public System {
public:
    CompassSystem();
    ~CompassSystem();

    void update(GameEngine &gameContext) const override;

private:
    bool isCallCompass(GameEngine &gameContext) const;
    void createCompass(GameEngine &gameContext) const;
    void removeCompass(GameEngine &gameContext) const;

    void checkTakingPalantir(GameEngine &gameContext, InteractionComponent &playerInteraction) const;
    bool checkDropingPalantir(GameEngine &gameContext, InputComponent &playerInput, SituationComponent &playerSit) const;
    void updatePalantirRotation(GameEngine &gameContext) const;


    void updateCompassAnimation(GameEngine &gameContext, SituationComponent &fCompassSit, CompassComponent &compassComp) const;
    void updateScale(GameEngine &gameContext, CompassComponent &compassComp, DrawableComponent &compassDrawable) const ;
    void updateRotation(GameEngine &gameContext, CompassComponent &compassComp, SituationComponent &compassSit) const ;

    void updateCompassLookAtPoint(GameEngine &gameContext, SituationComponent &fCompassSit) const;
    void updateCompassPosition(GameEngine &gameContext, SituationComponent &fCompassSit, SituationComponent &playerSit) const;
};