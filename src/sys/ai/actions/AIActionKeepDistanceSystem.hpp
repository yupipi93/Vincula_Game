#pragma once

#include <sys/System.hpp>

class AIActionKeepDistanceSystem : public System {
public:
    AIActionKeepDistanceSystem();
    ~AIActionKeepDistanceSystem();

    void update(GameEngine &gameContext) const override;

private:
	void calculateKeepDistancePosition(GameEngine &gameContext, uint32_t entityID, float &objPointX, float &objPointZ) const;


};