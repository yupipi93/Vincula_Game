#pragma once

#include "Component.hpp"

class WaypointComponent : public Component {
public:
    explicit WaypointComponent(const uint32_t id);
    ~WaypointComponent() override;

    uint32_t	target_of	{};
};
