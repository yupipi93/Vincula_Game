#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class ProximitySystem : public System {
public:
    ProximitySystem();
    ~ProximitySystem();

    void update(GameEngine &gameContext) const override;
private:
    void processEntitiesSensing(GameEngine &gameContext, ProximityComponent &proximityComp) const;
    bool isAttractiveType(GameEngine &gameContext, uint32_t entiyId, std::vector<EntityType> &reactiveTypes) const;
    void setTarget(GameEngine &gameContext, uint32_t entityId, uint32_t aimId) const;
    void setNewSpeed(GameEngine &gameContext, uint32_t entityId) const;

};