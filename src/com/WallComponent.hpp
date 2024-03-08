#pragma once

#include "Component.hpp"

class WallComponent : public Component {
public:
    explicit WallComponent(const uint32_t id);
    ~WallComponent() override;
};
