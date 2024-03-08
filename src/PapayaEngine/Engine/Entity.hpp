#pragma once
#include <glm.hpp> // OpenGL mathematics

namespace pap {
class PapayaDevice;
} // NAMESPACE

namespace pap {

class Entity {
public:
    explicit Entity() {}
    virtual ~Entity() = default;

    virtual void draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) = 0;
    virtual void drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) = 0;
private:
};

} // NAMESPACE