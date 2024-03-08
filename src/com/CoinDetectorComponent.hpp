#pragma once

#include "BoundingComponent.hpp"
#include <enum/CollisionType.hpp>

class CoinDetectorComponent : public BoundingComponent {
public:
    explicit CoinDetectorComponent(const uint32_t id);
    ~CoinDetectorComponent() override;
};