#pragma once

#include "Component.hpp"

class TerrainCubeComponent : public Component {
public:
    explicit TerrainCubeComponent(const uint32_t id);
    ~TerrainCubeComponent() override;
};
