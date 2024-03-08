#include "WaypointSystem.hpp"

#include <eng/GameEngine.hpp>


//#####################################################
//      == AI ACTION WANDER SYSTEM [STANDBY] ==
//  - Get All entities whith Wander Component
//	- Remove Target
//
//   Using Componets:
//      - MoveTo            	(Read - Write)
//#####################################################

WaypointSystem::WaypointSystem() {}

WaypointSystem::~WaypointSystem() {}

void WaypointSystem::update(GameEngine &gameContext) const {

    auto &waypoints = gameContext.entityMan.getComponents<WaypointComponent>();
    std::vector<uint32_t> waypointsReached;

    for (auto &waypoint : waypoints) {
        uint32_t waypointId = waypoint.id;
        uint32_t targetOf = waypoint.target_of;

        if (isWaypointReached(gameContext, waypointId, targetOf)) {
            waypointsReached.emplace_back(waypointId);

            gameContext.entityMan.getComponent<AIMoveToComponent>(targetOf).targetId = 0;
        }
    }

    for (uint32_t waypointToDeleteId : waypointsReached) {
        gameContext.eraseEntityByID(waypointToDeleteId);
    }


}

bool WaypointSystem::isWaypointReached(GameEngine &gameContext, uint32_t waypointId, uint32_t ownerId) const {
    auto &sensor = gameContext.entityMan.getComponent<BoundingComponent>(waypointId);

    for (uint32_t entityInSensorId : sensor.entitiesColliding) {
        if (entityInSensorId == ownerId) {
            return true;
        }
    }
    return false;
}

