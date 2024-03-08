#pragma once

#include "Component.hpp"

class IPPartComponent : public Component {
public:
    explicit IPPartComponent(const uint32_t id);
    ~IPPartComponent() override;

};
