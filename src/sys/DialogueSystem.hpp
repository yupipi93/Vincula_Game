#pragma once

#include "System.hpp"
#include <tools/Storage.hpp>

class DialogueSystem : public System {
public:
    DialogueSystem();
    ~DialogueSystem();

    void update(GameEngine &gameContext) const override;

private:
    void updateNPCRotation(GameEngine &gameContext, SituationComponent &situation) const;
    void updateDialogue(GameEngine &gameContext, DialogueComponent &dialogue) const;
    void giveCoinsToPlayer(GameEngine &gameContext) const;
    
    bool demandCoins(GameEngine &gameContext, DialogueComponent &dialogue) const;
    bool demandCoinsToLvlUp(GameEngine &gameContext, DialogueComponent &dialogue) const;
    
    void updateCurrentSentence(GameEngine &gameContext, DialogueComponent &dialogue) const;
    bool updateActiveDialogue(GameEngine &gameContext, uint32_t dialogue) const;

    void unlockCompass(GameEngine &gameContext, DialogueComponent &dialogue) const;
    void unlockMap(GameEngine &gameContext, DialogueComponent &dialogue) const;

    void giveFragmentKey(GameEngine &gameContext, DialogueComponent &dialogue) const;
    void giveWeapon(GameEngine &gameContext, DialogueComponent &dialogue) const;

    void checkAdditionalDialogues(GameEngine &gameContext) const;
    void updateLevelUpHUDEffect(GameEngine &gameContext) const;


    void improveStats(GameEngine &gameContext, StatsComponent &playerStats, LevelComponent &playerLevel) const;
    bool checkUnlockHabilities(LevelComponent &playerLevel) const;


};