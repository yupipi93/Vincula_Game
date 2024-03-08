#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class CameraSystem : public System {
public:
    CameraSystem();
    ~CameraSystem();

    void update(GameEngine &gameContext) const override;

private:
    void smart			(SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void chopped        (SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void choppedZoom	(SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void zenith 		(SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void zenithUp		(SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void air			(SituationComponent &cameraSituation, SituationComponent &playerSituation) const;
    void smartWallLimit	(SituationComponent &cameraSituation, TerrainDataComponent &terrainData) const;

    };