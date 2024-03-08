#pragma once

#include "Component.hpp"

#include <string>

class NotificationComponent : public Component {
public:
    explicit NotificationComponent(const uint32_t id);
    ~NotificationComponent() override;

    float lifeTime	 {0};
};
