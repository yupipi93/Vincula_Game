#include "EntityAnimation.hpp"
#include <PapayaEngine/PapayaDevice.hpp>
#include <PapayaEngine/Engine/LightPoint.hpp>
//console.log


pap::EntityAnimation::EntityAnimation(std::vector <pap::EntityModel *> m)
    : pap::Entity(), models {m} {
    
    then = std::chrono::system_clock::now();

}

pap::EntityAnimation::~EntityAnimation() {

}

void pap::EntityAnimation::draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) {

    if( !models.empty() ) {

        std::chrono::time_point<std::chrono::system_clock> now;
        now = std::chrono::system_clock::now();
        std::chrono::duration<float, std::ratio<1>> elapsed_time = now - then;

        
        float framesToJump = elapsed_time.count() / timeFrame + timeRest;

        if(framesToJump >= 1){
            if(actualFrame+framesToJump>=models.size()-1 && animationLoop)
                actualFrame = 0;
            else if (actualFrame+framesToJump>=models.size()-1 && !animationLoop)
                actualFrame = models.size()-1;
            else
                actualFrame += (int)framesToJump;

            timeRest = framesToJump - (int)framesToJump;
            then = std::chrono::system_clock::now();
        }
        models[actualFrame]->draw(transfMatrix, device, color);
    }
}

void pap::EntityAnimation::drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) {

    if( !models.empty() ) {
        
        models[actualFrame]->drawLight(transfMatrix, device);
    }
}

std::vector <pap::EntityModel *> pap::EntityAnimation::getEntityModel(){
    return models;
}

void pap::EntityAnimation::setEntityModel(std::vector <pap::EntityModel *> m){
    models = m;
}