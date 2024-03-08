#include "EntityCamera.hpp"
#include <gtc/matrix_transform.hpp>
#include <PapayaEngine/Engine/Window.hpp>

//console.log



pap::EntityCamera::EntityCamera(bool isPers)
    : pap::Entity(), isPerspective{isPers} {

    projection 	= glm::mat4(1.0f);
    view 		= glm::mat4(1.0f);
}

pap::EntityCamera::~EntityCamera() {

}

void pap::EntityCamera::draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) {
}
void pap::EntityCamera::drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) {
}

bool pap::EntityCamera::getActiveCamera() {

    return activeCamera;
}

void pap::EntityCamera::setActiveCamera(bool state) {

    activeCamera = state;
}

glm::mat4 pap::EntityCamera::updateProjection() {
	// Calculate projection
	float auxWidth = (float)Window::windowWidth;

	if(auxWidth<=0)
		auxWidth = 0.000000000001;

    projection = glm::perspective(glm::radians(fov), auxWidth / (float)Window::windowHeight, 0.1f, 1000.0f);
    return projection;
}

