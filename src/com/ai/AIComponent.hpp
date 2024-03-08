#pragma once
#include <com/Component.hpp>
#include <enum/EntityType.hpp>
#include <enum/AiAction.hpp>
#include <enum/AiState.hpp>
#include <enum/EnemyType.hpp>

#include <vector>

class AIComponent : public Component {
public:
    explicit AIComponent(const uint32_t id);
    ~AIComponent() override;

    AiState  				lastState			{NO_STATE};
    AiAction 				lastAction			{NO_ACTION};
    std::vector<EntityType>	hostileTo			{PLAYER};
    uint32_t				aggredBy			{0};

    float    				currentAlertState	{0};
    float    				defaultAlertState	{100};

    AiAction standByAction		{WANDER};
    AiAction defensiveAction	{ESCAPE};

    bool forceCombat {false};
	bool forceStandBy {false};

    EnemyType enemyType {CHICK}; 
};