#pragma once
#include "Component.hpp"
#include <vector>


class BoundingComponent : public Component {
public:
    explicit BoundingComponent(const uint32_t id);
    ~BoundingComponent() override;

    void removeEntityColliding(const uint32_t id);

    float 					sizeX 				{10};
    float 					sizeY 				{10};
    float 					sizeZ 				{10};
    std::vector<uint32_t>	entitiesColliding	{};
    
	//float 					auxX 				{sizeX};//used to saved actual size sensor (in PhysicsSystem) for resize sensor when dashing
	//float 					auxZ 				{sizeZ};//used to saved actual size sensor (in PhysicsSystem) for resize sensor when dashing

};