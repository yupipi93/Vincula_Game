#pragma once

#include <PapayaEngine/Engine/Entity.hpp>
#include <PapayaEngine/Engine/ResourceModel.hpp>

namespace pap {

class EntityModel : public pap::Entity
{
public:
	EntityModel(pap::ResourceModel *m = nullptr);
	~EntityModel() override;

	void draw(glm::mat4 transfMatrix, pap::PapayaDevice &device, glm::vec3 color) override;
	void drawLight(glm::mat4 transfMatrix, pap::PapayaDevice &device) override;
	pap::ResourceModel *getResourceModel();
	void setResourceModel(pap::ResourceModel *m);


	bool 	  	hasShadow = false;
	
private:
	pap::ResourceModel *model;
};

} // NAMESPACE