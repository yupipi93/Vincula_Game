#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class BaseSystem : public System {
public:
    BaseSystem();
    ~BaseSystem();

    void update(GameEngine &gameContext) const override;
private:
    void restorePlayerHealth	(GameEngine &gameContext) const;
    void bankActions            (GameEngine &gameContext) const;
    bool isPlayerInSensor		(GameEngine &gameContext, uint32_t sensorId) const;
	void depositeCoins			(GameEngine &gameContext) const;
	void extractCoins			(GameEngine &gameContext) const;
    void exchangeCoins			(GameEngine &gameContext, uint32_t entityId, uint32_t targetId) const;
    void dropCoinWithTarget		(GameEngine &gameContext, uint32_t entityId, uint32_t targetId, GameObjectType coinType) const;
    
    void sourceAnimation        (GameEngine &gameContext) const;
    
    void setDefaultModel        (GameEngine &gameContext, uint32_t entityId) const;
    void setSecundaryModel		(GameEngine &gameContext, uint32_t entityId) const;
};