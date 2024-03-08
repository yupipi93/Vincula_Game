#pragma once

#include <PapayaEngine/Engine/EntityLight.hpp>

namespace pap {

class LightPoint : public pap::EntityLight {
public:
    LightPoint() 
        :pap::EntityLight(POINT) {};
    ~LightPoint() override {};

    //1.f, 0.014f, 0.0007f
    float constant;
    float linear;        // with this, we get the max distance of this light
    float quadratic;		// in (https://learnopengl.com/Lighting/Light-casters) we have the data
private:

};

} // NAMESPACE