#pragma once

#include "System.hpp"


class TimeSystem : public System {
public:
    TimeSystem();
    ~TimeSystem();

    void update(GameEngine &gameContext) const override;
private:
	
};