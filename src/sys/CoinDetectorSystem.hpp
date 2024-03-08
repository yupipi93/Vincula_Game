#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

template<class>
class Storage;

class CoinDetectorSystem : public System {
public:
    CoinDetectorSystem();
    ~CoinDetectorSystem();

    void update(GameEngine &gameContext) const override;
private:
	void updateCoinDetectors(GameEngine &gameContext, Storage<CoinComponent> &coins) const ;
	void checkIsSensing(GameEngine &gameContext, BoundingComponent &coinDetector, SituationComponent &detecorSituation, uint32_t coinId) const;

};