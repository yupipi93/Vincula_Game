#pragma once

#include "Component.hpp"
#include <enum/EntityType.hpp>
#include <vector>

class MapComponent : public Component {
public:
    explicit MapComponent(const uint32_t id);
    ~MapComponent() override;

    bool discoverEntity(uint32_t id);
    void discoverArea(uint16_t pos);

    std::vector<uint32_t> 	discoveredEntities;
    std::vector<bool> 		discoveredAreas;

    const uint16_t MAX_MAP_DIVISION {25};

};
