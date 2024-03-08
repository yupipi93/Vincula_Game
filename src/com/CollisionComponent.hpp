#pragma once

#include "BoundingComponent.hpp"
#include <enum/CollisionType.hpp>

class CollisionComponent : public BoundingComponent {
public:
    explicit CollisionComponent(const uint32_t id);
    ~CollisionComponent() override;

    CollisionType	type 	{STATIC};
};