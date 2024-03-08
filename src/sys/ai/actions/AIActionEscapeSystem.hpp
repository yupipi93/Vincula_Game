#pragma once

#include <sys/System.hpp>

class AIActionEscapeSystem : public System {
public:
    AIActionEscapeSystem();
    ~AIActionEscapeSystem();

    void update(GameEngine &gameContext) const override;

private:
	void calculateEscapePosition(GameEngine &gameContext, uint32_t entityID, float &objPointX, float &objPointZ) const;


};