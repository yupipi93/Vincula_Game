#pragma once

#include <sys/System.hpp>

class AIActionRegroupSystem : public System {
public:
    AIActionRegroupSystem();
    ~AIActionRegroupSystem();

    void update(GameEngine &gameContext) const override;
private:
    uint32_t getSatandByFriend				(GameEngine &gameContext, uint32_t entityId)const;

	void moveToTarget 						(GameEngine &gameContext, AIMoveToComponent &moveToComp, uint32_t targetId) const;
    bool isSensingFriend					(GameEngine &gameContext, uint32_t entityId, uint32_t friendId) const;
    void shareHostileTargetWithFriend		(GameEngine &gameContext, AIDefensiveComponent &defensiveComp, uint32_t friendId) const;
    void entityTurnAgressive				(GameEngine &gameContext, uint32_t entityId) const;
    
    bool isInStandBy						(GameEngine &gameContext, uint32_t entityId) const;
    bool isSameSubType						(GameEngine &gameContext, GameObjectType entitySubType, uint32_t friendId) const;


};