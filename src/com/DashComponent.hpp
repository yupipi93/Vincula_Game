#pragma once

#include "Component.hpp"
#include <chrono>
#include <ctime>

class DashComponent : public Component {
public:
    explicit DashComponent(const uint32_t id);
    ~DashComponent() override;

    void activateDash();
    void deactivateDash();

    bool    dashing         {false};
    float   velocityFactor  {2.85};
    float   dashMaxTime     {0.5};
    float   timerDash       {dashMaxTime};
    float   maxCoolDown     {0.6};
    float   timerCoolDown   {0};
};