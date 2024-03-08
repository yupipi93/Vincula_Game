#pragma once

#include "Component.hpp"
#include <string>
#include <cstdint>

class Drawable2DComponent : public Component {
public:
    explicit Drawable2DComponent(const uint32_t id);
    ~Drawable2DComponent() override;

    std::string	texture2D		{""};
    std::string secondTexture2D {""};
};
