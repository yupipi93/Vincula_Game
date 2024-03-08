#pragma once
#include "Component.hpp"
#include <enum/EntityType.hpp>
#include <vector>


class ProximityComponent : public Component {
public:
    explicit ProximityComponent(const uint32_t id);
    ~ProximityComponent() override;

    std::vector<EntityType> attractiveTo {{ENEMY}};
};
