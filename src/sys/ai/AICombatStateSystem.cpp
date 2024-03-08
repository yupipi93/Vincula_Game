
#include "AICombatStateSystem.hpp"

#include <tools/Utils.hpp>
#include <tools/FuzzyLogic.hpp>

#include <eng/GameEngine.hpp>

//#####################################################
//     == AI ACTION THREATEN SYSTEM [OFFENSIVE] ==
//  - Get All entities whith AiThreaten Component
//  - Get each offensive component
//  - Get holstile entity id from Offensive Componet
//  - Set Target in MoveTo Component
//
//   Using Componets:
//      - MoveTo            (Read - Write)
//      - AiThreaten        (Read)
//      - AiOffensive       (Read)
//#####################################################

AICombatStateSystem::AICombatStateSystem() {}

AICombatStateSystem::~AICombatStateSystem() {}

void AICombatStateSystem::update(GameEngine &gameContext) const {

    // Get all combat component
    auto &combatComponents = gameContext.entityMan.getComponents<AICombatComponent>();

    // Iterate on those components
    for (auto &combatComponent : combatComponents) {
        if (combatComponent.targetId) {
            if (gameContext.entityMan.existsComponent<AIComponent>(combatComponent.id)) {
                auto &aIComponent = gameContext.entityMan.getComponent<AIComponent>(combatComponent.id);
                // For each one, decide by FuzzyLogic if it should change to Offensive or Defensive
                AiState state = determinateCombatState(gameContext, combatComponent);
                setState(gameContext, aIComponent, combatComponent.targetId, state);
            }
        }
    }
}

void AICombatStateSystem::setState(GameEngine &gameContext, AIComponent &aIComponent, uint32_t entityTarget, AiState state) const {
    uint32_t entityId = aIComponent.id;

    if(aIComponent.lastState != state) {
        //Set Red texture to enemy
        if (gameContext.entityMan.existsComponent<DrawableComponent>(entityId)) {
            DrawableComponent &drawable = gameContext.entityMan.getComponent<DrawableComponent>(entityId);

            if (state == OFFENSIVE) {
                //drawable.texture = "./media/enemy-red.png";//TEMP
                gameContext.getSoundFacadeRef().setParameterEventByID(entityId, OFFENSIVE_SOUND);
                AITargetComponent &offensiveComponent = gameContext.entityMan.createComponent<AIOffensiveComponent>(entityId);
                offensiveComponent.targetId = entityTarget;
                gameContext.entityMan.eraseComponent<AIDefensiveComponent>(entityId);
            } else if (state == DEFENSIVE) {
                //drawable.texture = "./media/enemy-blue.png";//TEMP
                gameContext.getSoundFacadeRef().setParameterEventByID(entityId, DEFENSIVE_SOUND);
                AITargetComponent &defensiveComponent = gameContext.entityMan.createComponent<AIDefensiveComponent>(entityId);
                defensiveComponent.targetId = entityTarget;
                gameContext.entityMan.eraseComponent<AIOffensiveComponent>(entityId);
            }

            gameContext.getWindowFacadeRef().updateTextures(gameContext, {entityId});
            aIComponent.lastState = state;
        }
    }
}

AiState AICombatStateSystem::determinateCombatState(GameEngine &gameContext, AICombatComponent &combatComp) const {
    AiState state = DEFENSIVE;
    uint32_t entityId = combatComp.id;
    uint32_t targetId = combatComp.targetId;

    bool allComponentExist = true;
    if (!gameContext.entityMan.existsComponent<AIDifuseStateComponent>(entityId)) {
        allComponentExist = false;
    }
    if (!gameContext.entityMan.existsComponent<StatsComponent>(entityId)) {
        allComponentExist = false;
    }
    if (!gameContext.entityMan.existsComponent<StatsComponent>(targetId)) {
        allComponentExist = false;
    }

    if (!gameContext.entityMan.existsComponent<WalletComponent>(entityId)) {
        allComponentExist = false;
    }
    if (!gameContext.entityMan.existsComponent<WalletComponent>(targetId)) {
        allComponentExist = false;
    }


    if(allComponentExist && combatComp.targetId) {

        auto &difuseStateComp = gameContext.entityMan.getComponent<AIDifuseStateComponent>(entityId);
        StatsComponent &entityStats = gameContext.entityMan.getComponent<StatsComponent>(entityId);
        StatsComponent &targetStats = gameContext.entityMan.getComponent<StatsComponent>(targetId);

        WalletComponent &entityWallet = gameContext.entityMan.getComponent<WalletComponent>(entityId);
        WalletComponent &targetWallet = gameContext.entityMan.getComponent<WalletComponent>(targetId);

        // FUZZY LOGIC TO DETERMINATE THE COMBAT STATE
        float offensiveLevel = 0;
        float defensiveLevel = 0;




        float healtPerc         = entityStats.currentHealth * 100 / entityStats.maxHealth;
        float dyingLevel        = FuzzyLogic::getInverseMembership(healtPerc, difuseStateComp.dyingX0,   difuseStateComp.dyingX1);
        float injuredLevel      = FuzzyLogic::getMembership(healtPerc, difuseStateComp.injuredX0, difuseStateComp.injuredX1, difuseStateComp.injuredX2, difuseStateComp.injuredX3);
        float healthyLevel      = FuzzyLogic::getMembership(healtPerc, difuseStateComp.healthyX0, difuseStateComp.healthyX1);



        float strengthDiff      = entityStats.damage - targetStats.damage;
        float looserLevel       = FuzzyLogic::getInverseMembership(strengthDiff, difuseStateComp.looserX0,   difuseStateComp.looserX1);
        float strongerLevel     = FuzzyLogic::getMembership(strengthDiff, difuseStateComp.strongerX0, difuseStateComp.strongerX1);



        float moneyDiff         = entityWallet.coins - targetWallet.coins;
        float poorerLevel       = FuzzyLogic::getInverseMembership(moneyDiff, difuseStateComp.poorerX0, difuseStateComp.poorerX1);
        float richerLevel       = FuzzyLogic::getMembership(moneyDiff, difuseStateComp.richerX0, difuseStateComp.richerX1);
        
        
        // OFFENSIVE when (healty OR (injured AND (stronger OR poorer)))
        offensiveLevel = FuzzyLogic::OR(healthyLevel, FuzzyLogic::AND(injuredLevel, FuzzyLogic::OR(strongerLevel, poorerLevel)));
        // DEFENSIVE when (dying OR (injured AND (looser OR richer)))
        defensiveLevel = FuzzyLogic::OR(dyingLevel, FuzzyLogic::AND(injuredLevel, FuzzyLogic::OR(looserLevel, richerLevel)));



        if (offensiveLevel > defensiveLevel)
            state = OFFENSIVE;

    }
    return state;
}

