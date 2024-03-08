#pragma once

#include <sys/System.hpp>

class AIStateMachineSystem : public System {
public:
    AIStateMachineSystem();
    ~AIStateMachineSystem();

    void 	update(GameEngine &gameContext) const override;

private:
    void 					changeEntityState			(GameEngine &gameContext, AIComponent &aIComponent) const;
    uint32_t 				getDetectDangerousEntity	(GameEngine &gameContext, uint32_t entityId, AIComponent &aIComponent) const;
    void 					setCombatState				(GameEngine &gameContext, AIComponent &aIComponent, uint32_t entityTarget) const;
    void 					setStandByState				(GameEngine &gameContext, AIComponent &aIComponent) const;
    void 					deleteLastStateComponent	(GameEngine &gameContext, uint32_t entityId) const;
    bool 					checkIfDangerousType		(GameEngine &gameContext, std::vector<EntityType> &dangerousEntityType, uint32_t entityIdSensing) const;
    std::vector<EntityType> getAggressiveToEntitiesType	(GameEngine &gameContext, uint32_t entityId) const;
    std::vector<uint32_t> 	getEntitiesSensing			(GameEngine &gameContext, uint32_t entityId) const;

};