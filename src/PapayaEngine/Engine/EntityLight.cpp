#include "EntityLight.hpp"


pap::EntityLight::EntityLight(LightType t)
    : pap::Entity(), type {t} {

}

pap::EntityLight::~EntityLight() {

}

void pap::EntityLight::draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) {

}
void pap::EntityLight::drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) {

}