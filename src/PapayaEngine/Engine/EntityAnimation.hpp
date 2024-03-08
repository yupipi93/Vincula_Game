#pragma once

#include <PapayaEngine/Engine/Entity.hpp>
#include <PapayaEngine/Engine/EntityModel.hpp>
#include <vector>
#include <chrono>

namespace pap {

class EntityAnimation : public pap::Entity
{
public:
	EntityAnimation(std::vector <pap::EntityModel *> models);
	~EntityAnimation() override;

	void draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) override;
	void drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) override;
	std::vector <pap::EntityModel *> getEntityModel();
	void setEntityModel(std::vector <pap::EntityModel *> m);

	float timeFrame = 1.f/30.f;
	bool animationLoop = false;
private:

	std::vector <pap::EntityModel *> models;
	int actualFrame = 0;
	std::chrono::time_point<std::chrono::system_clock> then;
	float timeRest = 0.f;

};

} // NAMESPACE