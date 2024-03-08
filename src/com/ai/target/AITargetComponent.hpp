
#pragma once

#include <com/Component.hpp>


class AITargetComponent : public Component {
public:
    explicit AITargetComponent(const uint32_t id);
    ~AITargetComponent() override;

    uint32_t targetId {0};
};