#pragma once

#include "Component.hpp"
#include <enum/GameObjectType.hpp>

#include <vector>

class ObstacleDataComponent : public Component { 
public:
    explicit ObstacleDataComponent(const uint32_t id);
    ~ObstacleDataComponent() override;

	std::vector<GameObjectType>	obstaclesBiome1	{OBSTACLE_B1_1, OBSTACLE_B1_2, OBSTACLE_B1_3, OBSTACLE_B1_4, OBSTACLE_B1_5, OBSTACLE_B1_6};
	std::vector<GameObjectType>	obstaclesBiome2	{OBSTACLE_B2_1, OBSTACLE_B2_2, OBSTACLE_B2_3, OBSTACLE_B2_4}; 
	std::vector<GameObjectType>	obstaclesBiome3	{OBSTACLE_B3_1, OBSTACLE_B3_2, OBSTACLE_B3_3};
	std::vector<GameObjectType>	obstaclesBiome4	{OBSTACLE_B4_1, OBSTACLE_B4_2, OBSTACLE_B4_3, OBSTACLE_B4_4};
	std::vector<GameObjectType>	obstaclesBiome5	{OBSTACLE_B5_1, OBSTACLE_B5_2, OBSTACLE_B5_3, OBSTACLE_B5_4, OBSTACLE_B5_5, OBSTACLE_B5_6};
};
