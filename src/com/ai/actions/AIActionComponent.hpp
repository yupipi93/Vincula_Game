#pragma once

#include <com/Component.hpp>


class AIActionComponent : public Component {
public:
    explicit AIActionComponent(const uint32_t id);
    ~AIActionComponent() override;
};