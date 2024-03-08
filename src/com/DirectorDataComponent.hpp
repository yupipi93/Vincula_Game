#pragma once

#include <enum/GameObjectType.hpp>
#include "Component.hpp"
#include <vector>

struct enemyPercent{
	GameObjectType enemyType;
	uint8_t percent; //max 100 (Probability of apparition)
};

class DirectorDataComponent : public Component {
public:
    explicit DirectorDataComponent(const uint32_t id);
    ~DirectorDataComponent() override;

    void addSpawn(const float timer, const uint8_t max);
    void resetEnemies();

    std::vector<float>		spawnTimer			{};
    std::vector<float>		spawnCurrentTimer	{};
    std::vector<uint8_t>	enemiesSpawned		{};
    std::vector<uint8_t>	maxEnemiesSpawned	{};

    std::vector<enemyPercent> enemysInBiome1 {enemyPercent{ENEMY_PIG, 60}, enemyPercent{ENEMY_RODENT, 40}};
    std::vector<enemyPercent> enemysInBiome2 {enemyPercent{ENEMY_PINEAPPLE, 100}};
    std::vector<enemyPercent> enemysInBiome3 {enemyPercent{ENEMY_SPIDER, 100}};
    std::vector<enemyPercent> enemysInBiome4 {enemyPercent{ENEMY_MONKEY, 100}};
    std::vector<enemyPercent> enemysInBiome5 {enemyPercent{ENEMY_GHOST, 100}};
};