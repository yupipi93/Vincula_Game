#pragma once

#include "Component.hpp"

class EnemyComponent : public Component {
public:
    explicit EnemyComponent(const uint32_t id);
    ~EnemyComponent() override;

    float healthHeight		{0.f};
};

	