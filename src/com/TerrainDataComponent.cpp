#include "TerrainDataComponent.hpp"

TerrainDataComponent::TerrainDataComponent(const uint32_t id)
    : Component(id) {
    cubeIdsToMove.reserve(terrainWidth * terrainHeight);
}

TerrainDataComponent::~TerrainDataComponent() {}

void TerrainDataComponent::resetMovements() {
    movedUp 	= false;
    movedDown 	= false;
    movedLeft 	= false;
    movedRight 	= false;
}
