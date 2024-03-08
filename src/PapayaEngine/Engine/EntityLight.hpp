#pragma once

#include <PapayaEngine/Engine/Entity.hpp>
#include <PapayaEngine/Engine/enum/LightType.hpp>

namespace pap {

class EntityLight : public pap::Entity {
public:
    explicit EntityLight(LightType t);
    ~EntityLight() override;

    void draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) override final;
    void drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) override final;

    bool activeLight = true;
    glm::vec3 color {1.f, 1.f, 1.f};

    LightType type;

private:
};

} // NAMESPACE