#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class WalletSystem : public System {
public:
    WalletSystem();
    ~WalletSystem();

    void update(GameEngine &gameContext) const override;
private:
	void processCoinsSensing(GameEngine &gameContext, uint32_t entityId) const;
	void processCoinsColliding(GameEngine &gameContext, uint32_t entityId) const;

	void setTarget(GameEngine &gameContext, uint32_t entityId, uint32_t coinId) const;
	void catchCoin(GameEngine &gameContext, uint32_t entityId, uint32_t coinId) const;



};