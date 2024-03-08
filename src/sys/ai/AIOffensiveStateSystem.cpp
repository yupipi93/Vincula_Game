#include "AIOffensiveStateSystem.hpp"

#include <eng/GameEngine.hpp>
#include <tools/AIfunctions.hpp>
#include <tools/FuzzyLogic.hpp>
#include <tools/Utils.hpp>

//#####################################################
//           == AI OFFENSIVE STATE SYSTEM ==
//  - Get All entities whith AiOffensive Component
//  - Decide to Difise Action to execute
//
//   Using Componets:
//      - AI            (Read - Write)
//      - AiOffensive   (Read - Write)
//#####################################################

AIOffensiveStateSystem::AIOffensiveStateSystem() {}

AIOffensiveStateSystem::~AIOffensiveStateSystem() {}

void AIOffensiveStateSystem::update(GameEngine &gameContext) const {


    auto &allAiInOffensiveState {gameContext.entityMan.getComponents<AIOffensiveComponent>()};

    for(auto &offensiveComp : allAiInOffensiveState) {
        if (offensiveComp.targetId) {
            auto    &aIComp     {gameContext.entityMan.getComponent<AIComponent>(offensiveComp.id)};
            auto    &aIDifuse   {gameContext.entityMan.getComponent<AIDifuseActionComponent>(offensiveComp.id)};

            if(!offensiveComp.inAction) {
                AiAction action = determateOffensiveAction(gameContext, aIDifuse, offensiveComp.targetId);
                AIfunctions::setAction(gameContext, aIComp, action);
            }
        }
    }

}

AiAction AIOffensiveStateSystem::determateOffensiveAction(GameEngine &gameContext, AIDifuseActionComponent &aIDifuse, uint32_t targetId) const {
    // FuzzyLogic to determinate action
    SituationComponent &entitySit = gameContext.entityMan.getComponent<SituationComponent>(aIDifuse.id);
    SituationComponent &targetSit = gameContext.entityMan.getComponent<SituationComponent>(targetId);

    float distance = Utils::calculateDistance(entitySit.x, targetSit.x, entitySit.z, targetSit.z);


    vector<float> levels {};
    levels.reserve(5);

    float   meleeDistanceLevel          = FuzzyLogic::getInverseMembership(distance, aIDifuse.meleeX0, aIDifuse.meleeX1);


    float   distanceDistanceLevel       = FuzzyLogic::getMembership(distance, aIDifuse.distanceX0, aIDifuse.distanceX1, aIDifuse.distanceX2, aIDifuse.distanceX3);


    float   lungeDistanceLevel          = FuzzyLogic::getMembership(distance, aIDifuse.lungeX0, aIDifuse.lungeX1, aIDifuse.lungeX2, aIDifuse.lungeX3);


    float   followDistanceLevel         = FuzzyLogic::getMembership(distance, aIDifuse.followX0, aIDifuse.followX1);


    float   keepDistancesDistanceLevel  = FuzzyLogic::getMembership(distance, aIDifuse.keepDistanceX0, aIDifuse.keepDistanceX1, aIDifuse.keepDistanceX2, aIDifuse.keepDistanceX3);

    levels.emplace_back(meleeDistanceLevel);
    levels.emplace_back(lungeDistanceLevel);
    levels.emplace_back(distanceDistanceLevel);
    levels.emplace_back(keepDistancesDistanceLevel);
    levels.emplace_back(followDistanceLevel);

    float   maxLevel = 0;
    size_t  maxIndex    = 0;
    for (size_t i = 0; i < levels.size(); ++i) {
        if (levels[i] > maxLevel) {
            maxLevel = levels[i];
            maxIndex = i;
        }
    }


    switch (maxIndex) {
    case 0:
        return MELEE;

    case 1 :
        return LUNGE;

    case 2:
        return DISTANCE;

    case 3:
        return KEEPDISTANCE;

    case 4:
        return THREATEN;

    default:
        return NO_ACTION;
    }

}