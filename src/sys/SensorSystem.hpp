#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class SensorSystem : public System {
public:
    SensorSystem();
    ~SensorSystem();

    void update(GameEngine &gameContext) const override;
private:
	void checkIsSensing(GameEngine &gameContext, uint32_t idA, BoundingComponent &boundingA, std::vector<uint32_t> boundingsIdInsideChunk) const;

};