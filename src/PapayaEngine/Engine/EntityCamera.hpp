#pragma once

#include <PapayaEngine/Engine/Entity.hpp>

namespace pap {

class EntityCamera : public pap::Entity {
public:
    EntityCamera(bool isPers = true);
    ~EntityCamera() override;

    void draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) override;
    void drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) override;

    /*void setPerspectiva(float, float...);
    void setParalela(float, float...);*/
    glm::mat4 updateProjection();
    bool getActiveCamera();
    void setActiveCamera(bool state);

    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 cameraTarget {0.0f, 0.0f, 0.0f};

private:
    bool isPerspective;
    float close = 0.f;
    bool activeCamera = false;
    float fov {45.0f};
};

} // NAMESPACE