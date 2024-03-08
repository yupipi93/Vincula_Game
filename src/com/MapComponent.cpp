#include "MapComponent.hpp"

//console.log


MapComponent::MapComponent(const uint32_t id)
    : Component(id) {
    //discoveredAreas.reserve(MAX_MAP_DIVISION * MAX_MAP_DIVISION);
    discoveredAreas.resize(MAX_MAP_DIVISION * MAX_MAP_DIVISION);
}

MapComponent::~MapComponent() {}

bool MapComponent::discoverEntity(uint32_t id) {
    size_t i = 0;
    for (; i < discoveredEntities.size(); ++i) {
        if (discoveredEntities[i] > id)
            break;
        if (discoveredEntities[i] == id)
            return false;
    }

    discoveredEntities.emplace(discoveredEntities.begin()+i, id);
    return true;
}

void MapComponent::discoverArea(uint16_t pos) {
    if (pos < discoveredAreas.size())
        discoveredAreas.at(pos) = true;
}

