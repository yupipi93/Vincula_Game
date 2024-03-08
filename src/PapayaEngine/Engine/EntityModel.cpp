#include "EntityModel.hpp"
#include <PapayaEngine/PapayaDevice.hpp>
#include <PapayaEngine/Engine/LightPoint.hpp>

//console.log


pap::EntityModel::EntityModel(pap::ResourceModel *m)
    : pap::Entity(), model {m} {

}

pap::EntityModel::~EntityModel() {

}

void pap::EntityModel::draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) {

    if( model ) {
        // Get the shaders to draw the model
        pap::Shader &shader = device.getModelShader();

        // Set model in shader
        shader.setMat4("model", transfMatrix);
        // Set color data in shader
        shader.setVec3("objectColor", color);


        model -> draw(shader);
    }
}

void pap::EntityModel::drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) {

    if( model && hasShadow) {
        // Get the shaders to draw the model
        pap::Shader &shader = device.getDepthShader();

        shader.setMat4("model", transfMatrix);
        
        model -> draw(shader);
    }
}

pap::ResourceModel *pap::EntityModel::getResourceModel() {
    return model;
}

void pap::EntityModel::setResourceModel(pap::ResourceModel *m) {
    model = m;
}



/*void pap::EntityModel::setPointLights(pap::PapayaDevice &device, pap::Shader &shader) const {
    glm::vec3 lightPos   {0.f, 0.f, -700.f};
    glm::vec3 lightColor     {0.2f, 1.0f, 0.4f};
    // Setting the light properties
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.95f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.8f);


    float constant = 1.f;
    float linear = 0.014f;
    float quadratic = 0.0007f;

    shader.setInt("sizePLights", 1);

    shader.setVec3("pointLights[0].position", lightPos);
    shader.setFloat("pointLights[0].constant", constant);
    shader.setFloat("pointLights[0].linear", linear);
    shader.setFloat("pointLights[0].quadratic", quadratic);
    shader.setVec3("pointLights[0].ambient",   ambientColor);
    shader.setVec3("pointLights[0].diffuse",   diffuseColor);
    shader.setVec3("pointLights[0].specular",  1.0f, 1.0f, 1.0f); // Normaly set on all 1.f
}*/