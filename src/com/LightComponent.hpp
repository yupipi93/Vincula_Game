#pragma once

#include "Component.hpp"
#include <array>


class LightComponent : public Component {
public:
    explicit LightComponent(const uint32_t id);
    ~LightComponent() override;

    std::array<float, 3>  color {1.f, 1.f, 1.f};

    // for the Point Lights
    float	 constant		{0.f};
    float	 linear			{0.f};        // with this, we get the max distance of this light
    float	 quadratic		{0.f};		// in (https://learnopengl.com/Lighting/Light-casters) we have the data

    // for the Directional Light
    std::array<float, 3> direction {0.f, 0.f, 0.f};
    std::array<float, 3> objective {0.f, 0.f, 0.f};
};