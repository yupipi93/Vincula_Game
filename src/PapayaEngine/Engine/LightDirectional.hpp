#pragma once

#include <PapayaEngine/Engine/EntityLight.hpp>

namespace pap {

class LightDirectional : public pap::EntityLight {
public:
    LightDirectional(glm::vec3 dir) 
        :pap::EntityLight(DIRECTIONAL), direction{dir} {};
    ~LightDirectional() override {};

    glm::vec3 direction {-0.5f, -1.0f, -0.3f};
    glm::vec3 objective { 0.0f,  0.0f,  0.0f};
private:

};

} // NAMESPACE