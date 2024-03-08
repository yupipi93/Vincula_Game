#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class MapSystem : public System {
public:
    MapSystem();
    ~MapSystem();

    void update(GameEngine &gameContext) const override;

private:
    void updateMap(GameEngine &gameContext, MapComponent &map) const;
	void discoverPosition(GameEngine &gameContext, MapComponent &map) const;

};