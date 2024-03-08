#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class DetectionSignSystem : public System {
public:
    DetectionSignSystem();
    ~DetectionSignSystem();

    void update(GameEngine &gameContext) const override;
    
private:
	void updateDetectionSignsAnimation(GameEngine &gameContext) const;
	void updateRotation(GameEngine &gameContext, DetectionSignComponent &detectionSign, SituationComponent &detectionSignSit) const;
	void updateLevitation(GameEngine &gameContext, DetectionSignComponent &detectionSign, SituationComponent &detectionSignSit) const;
};