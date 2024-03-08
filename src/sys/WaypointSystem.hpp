#pragma once

#include <sys/System.hpp>

class WaypointSystem : public System {
public:
    WaypointSystem();
    ~WaypointSystem();

    void 	update(GameEngine &gameContext) const override;

private:
	bool isWaypointReached(GameEngine &gameContext, uint32_t waypointId, uint32_t ownerId) const;

};