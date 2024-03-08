#pragma once

#include "Component.hpp"
#include <string>
#include <cstdint>
#include <array>

class DrawableComponent : public Component {
public:
    explicit DrawableComponent(const uint32_t id);
    ~DrawableComponent() override;
    // Position
    std::string model           {""};
    std::string modelSecondary  {""};
    std::string	texture	        {""};

    //std::array<float, 4>  colorRgba {1.f, 1.f, 1.f, 1.f};

    float 		scaleX		    {1};
    float 		scaleY		    {1};
    float 		scaleZ		    {1};

    bool 		mustBeDrawn     {true};

    bool        transparency    {false};  // All objects with transparency must have this on true
    bool        hasShadow       {true};
};
