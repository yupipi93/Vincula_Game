#include "DialogueSystem.hpp"
#include <tools/Utils.hpp>
//console.log

//#####################################################
//               == DIALOGUE SYSTEM ==
//  - ????????????????????
//  - ????????????????????
//  - ????????????????????
//
//   Using Componets:
//      - ??????    (Read - Write)
//      - ??????    (Read)
//      - ??????    (Write)
//
//#####################################################

DialogueSystem::DialogueSystem() {}

DialogueSystem::~DialogueSystem() {}

void DialogueSystem::update(GameEngine &gameContext) const {
    uint32_t playerId = gameContext.playerId;
    InteractionComponent &playerInteraction = gameContext.entityMan.getComponent<InteractionComponent>(playerId);

    if(playerInteraction.dialogueInteracting != 0) {
        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(playerInteraction.dialogueInteracting);
        updateNPCRotation(gameContext, situation);
        DialogueComponent &dialogue = gameContext.entityMan.getComponent<DialogueComponent>(playerInteraction.dialogueInteracting);
        updateDialogue(gameContext, dialogue);
    }

    giveCoinsToPlayer(gameContext);
    updateLevelUpHUDEffect(gameContext);
    checkAdditionalDialogues(gameContext);
}

void DialogueSystem::checkAdditionalDialogues(GameEngine &gameContext) const {
    auto &dialogues = gameContext.entityMan.getComponents<DialogueComponent>();
    for (auto &dialogue : dialogues) {
        auto go = gameContext.entityMan.getEntity(dialogue.id).getGameObjectType();
        switch (go) {
        case NPC_GENTLEMAN_B1_KEY:
            if (dialogue.sentences.size() == 8 && Utils::isKeyComplete(gameContext)) {
                if (dialogue.currentSentence == dialogue.sentences.size() - 1) {
                    SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(dialogue.id);
                    float posX = situation.x;
                    float posZ = situation.z;
                    dialogue.interrogation = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);
                }
                dialogue.sentences.emplace_back(Sentence{"Vaya, parece que la@colección está completa.@Me pregunto qué abrirán."});
                dialogue.sentences.emplace_back(Sentence{"Quizá Bip-bop sepa algo@acerca de estas llaves;@debe estar en la sabana."});
            }
            break;
        case NPC_ROBOT_B4_INFORMATION:
            if (dialogue.sentences.size() == 4 && Utils::isKeyComplete(gameContext)) {
                if (dialogue.currentSentence == dialogue.sentences.size() - 1) {
                    SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(dialogue.id);
                    float posX = situation.x;
                    float posZ = situation.z;
                    dialogue.interrogation = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);
                }
                dialogue.sentences.emplace_back(Sentence{"Quizá tenga algo que@ver con esa colección@de llaves que tienes."});
            }
            break;
        case NPC_GUITARIST_B0_COMPASS:
            if (dialogue.sentences.size() == 10) {
                if (!gameContext.palantirId || gameContext.entityMan.getComponent<DialogueComponent>(gameContext.cowboyId).currentSentence >= 2) {
                    if (dialogue.currentSentence == dialogue.sentences.size() - 1) {
                        SituationComponent &situation = gameContext.entityMan.getComponent<SituationComponent>(dialogue.id);
                        float posX = situation.x;
                        float posZ = situation.z;
                        dialogue.interrogation = gameContext.entityMan.createDetectionSign(gameContext, posX, 45, posZ, 0, DETECTIONSIGN_INTERROGATION);
                    }
                    auto aux = dialogue.sentences[dialogue.sentences.size() - 1];
                    dialogue.sentences[dialogue.sentences.size() - 1] = {"Así que la brújula era@atraída por ese orbe."};
                    dialogue.sentences.emplace_back(Sentence{"Quizá sea útil para@recordar la ubicación@de algo."});
                    dialogue.sentences.emplace_back(Sentence{"..."});
                    dialogue.sentences.emplace_back(aux);
                    if (dialogue.currentSentence == 9)
                        --dialogue.currentSentence;
                }
            }
            break;

        }
    }
}

void DialogueSystem::updateNPCRotation(GameEngine &gameContext, SituationComponent &situation) const {
    auto npc = gameContext.entityMan.getEntity(situation.id).getGameObjectType();

    if (npc != NPC_GUITARIST_B0_COMPASS && npc != NPC_INDIAN_B3_BLOWGUN) {
        auto &playerSituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);
        auto angle = Utils::angleToTarget(situation, playerSituation);
        situation.rotation = Utils::radToDeg(angle);
        gameContext.getWindowFacadeRef().updateEntities(gameContext, {situation.id});
    }
}

void DialogueSystem::updateDialogue(GameEngine &gameContext, DialogueComponent &dialogue) const {

    if (dialogue.startedDialogue) {
        if(!updateActiveDialogue(gameContext, dialogue.id)) {

            updateCurrentSentence(gameContext, dialogue);

            bool repeatSentence = false;

            if(dialogue.sentences[dialogue.currentSentence].sentenceType == DEMANDCOINS) {
                repeatSentence = demandCoins(gameContext, dialogue);
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == DEMANDCOINSTOIMPROVE) {
                repeatSentence = demandCoinsToLvlUp(gameContext, dialogue);
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == GIVECOINS) {
                dialogue.givingCoinsToPlayer = true;
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == GIVEWEAPON) {
                giveWeapon(gameContext, dialogue);
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == GIVEFRAGMENTKEY) {
                giveFragmentKey(gameContext, dialogue);
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == UNLOCKCOMPASS) {
                unlockCompass(gameContext, dialogue);
            } else if(dialogue.sentences[dialogue.currentSentence].sentenceType == UNLOCKMAP) {
                unlockMap(gameContext, dialogue);
            }

            if (repeatSentence)
                dialogue.currentSentence--;
        }
    } else {
        dialogue.startedDialogue = true;
        updateActiveDialogue(gameContext, dialogue.id);
    }

    dialogue.dialogueActive = true;
    dialogue.dialogueTimer = 5.f;
}

// COINS GIVEAWAY
void DialogueSystem::giveCoinsToPlayer(GameEngine &gameContext) const {
    auto &dialogues = gameContext.entityMan.getComponents<DialogueComponent>();

    for (auto &dialogue : dialogues)
        if (dialogue.givingCoinsToPlayer) {
            auto &npcWallet = gameContext.entityMan.getComponent<WalletComponent>(dialogue.id);
            if(npcWallet.coins >= 1) {
                Utils::dropCoinWithTarget(gameContext, npcWallet.id, gameContext.playerId);
            } else
                dialogue.givingCoinsToPlayer = false;
        }
}



// DIALOGUES CONTROL
void DialogueSystem::updateCurrentSentence(GameEngine &gameContext, DialogueComponent &dialogue) const {
    ++dialogue.currentSentence;


    if (dialogue.currentSentence == dialogue.sentences.size() - 1) {
        // Delete Interrogation
        gameContext.eraseEntityByID(dialogue.interrogation);
    }

    if (dialogue.currentSentence == dialogue.sentences.size()) {

        if(dialogue.uniqueSentence) {
            dialogue.wantToTalk = false;
            dialogue.currentSentence = 0;
        } else if(dialogue.loopDialogue) {
            dialogue.currentSentence = 0;
        } else {
            --dialogue.currentSentence;
        }
    }
}

// THIS METHOD MAKES REPEAT THE LAS SENTENCE SAID IF YOU HAVE SPOKEN
// WITH ANOTHER NPC SINCE THE LAST TIME YOU SPOKE THAT ONE
bool DialogueSystem::updateActiveDialogue(GameEngine &gameContext, uint32_t dialogue) const {
    if(gameContext.lastDialogueID == dialogue)
        return false;

    gameContext.lastDialogueID = dialogue;
    return true;
}





// WEAPON GIVEAWAY
void DialogueSystem::giveWeapon(GameEngine &gameContext, DialogueComponent &dialogue) const {
    SituationComponent &npcSituation = gameContext.entityMan.getComponent<SituationComponent>(dialogue.id);

    float distance = 15;
    float angle = npcSituation.rotation;
    float auxAngle = Utils::degToRad(angle);
    float x = npcSituation.x + distance * cos(auxAngle);
    float z = npcSituation.z + distance * sin(auxAngle);
    Utils::dropWeapon(gameContext, dialogue.weaponForSale, x, npcSituation.y, z);
}


// KEY GIVEAWAY
void DialogueSystem::giveFragmentKey(GameEngine &gameContext, DialogueComponent &dialogue) const {
    SituationComponent &npcSituation = gameContext.entityMan.getComponent<SituationComponent>(dialogue.id);
    SituationComponent &plySituation = gameContext.entityMan.getComponent<SituationComponent>(gameContext.playerId);


    float distance = 15;
    float angle = npcSituation.rotation;
    float auxAngle = Utils::degToRad(angle);
    float x = npcSituation.x + distance * cos(auxAngle);
    float z = npcSituation.z + distance * sin(auxAngle);
    Utils::dropKey(gameContext, dialogue.biome, x, npcSituation.y, z);
}






// UNLOCK ITEMS
void DialogueSystem::unlockCompass(GameEngine &gameContext, DialogueComponent &dialogue) const {
    LevelComponent &playerLevel = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);
    playerLevel.compassUnlocked = true;
}
void DialogueSystem::unlockMap(GameEngine &gameContext, DialogueComponent &dialogue) const {
    LevelComponent &playerLevel = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);
    playerLevel.mapUnlocked = true;
}



bool DialogueSystem::demandCoins(GameEngine &gameContext, DialogueComponent &dialogue) const {
    bool shouldRepeatSentence = true;

    WalletComponent &playerWallet = gameContext.entityMan.getComponent<WalletComponent>(gameContext.playerId);

    if (playerWallet.coins >= dialogue.coinsToReceive) {
        if (dialogue.dialogueActive) {
            WalletComponent &npcWallet    = gameContext.entityMan.getComponent<WalletComponent>(dialogue.id);

            shouldRepeatSentence = false;
            playerWallet.coins -= dialogue.coinsToReceive;
            npcWallet.coins    += dialogue.coinsToReceive;

            giveWeapon(gameContext, dialogue);
        }
    } else {
        Utils::activeInfoNotification(gameContext, 2);
    }

    return shouldRepeatSentence;
}


bool DialogueSystem::demandCoinsToLvlUp(GameEngine &gameContext, DialogueComponent &dialogue) const {
    bool shouldRepeatSentence = true;
    --dialogue.currentSentence;

    WalletComponent &playerWallet = gameContext.entityMan.getComponent<WalletComponent>(gameContext.playerId);
    LevelComponent  &playerLevel  = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);
    WalletComponent &npcWallet    = gameContext.entityMan.getComponent<WalletComponent>(dialogue.id);

    uint32_t coinsNeededToImprove = pow(2, (0.6 * (playerLevel.level + 5)));


    //dialogue.sentences[dialogue.currentSentence].sentence = " Vida: " + std::to_string(playerStats.maxHealth) + " / Fuerza: " + std::to_string(playerStats.damage) + dialogue.demandCoinSentence + "@Cuesta " + std::to_string(coinsNeededToImprove) + " cristales.";

    //updateActiveDialogue(gameContext, dialogue.id);

    if(playerWallet.coins >= coinsNeededToImprove) {
        if (dialogue.dialogueActive) {
            playerWallet.coins -= coinsNeededToImprove;
            StatsComponent  &playerStats  = gameContext.entityMan.getComponent<StatsComponent>(gameContext.playerId);
            improveStats(gameContext, playerStats, playerLevel);
            checkUnlockHabilities(playerLevel);
            if (playerLevel.level < 20) {
                dialogue.sentences[dialogue.currentSentence].sentence = "¿Subir al nivel ";
                dialogue.sentences[dialogue.currentSentence].sentence += std::to_string(playerLevel.level + 1);
                dialogue.sentences[dialogue.currentSentence].sentence += "?@Para ello necesitaría@";
                coinsNeededToImprove = pow(2, (0.6 * (playerLevel.level + 5)));
                dialogue.sentences[dialogue.currentSentence].sentence += std::to_string(coinsNeededToImprove);
                dialogue.sentences[dialogue.currentSentence].sentence += " cristales.";
            } else
                shouldRepeatSentence = false;
        }
    } else {
        Utils::activeInfoNotification(gameContext, 2);
    }
    ++dialogue.currentSentence;

    return shouldRepeatSentence;
}


void DialogueSystem::improveStats(GameEngine &gameContext, StatsComponent &playerStats, LevelComponent &playerLevel) const {
    gameContext.getSoundFacadeRef().setParameterEventByID(gameContext.playerActionsEventId, LEVEL_UP_SOUND);
    playerLevel.levelingUp = true;

    playerLevel.level         += 1;
    uint8_t newDamage         = pow(1.66, (0.5 * ((float)playerLevel.level + 1)));
    uint16_t newHealth        = pow(10.f, (0.13 * ((float)playerLevel.level - 1) + 1));
    playerStats.maxHealth     = newHealth;
    playerStats.currentHealth = playerStats.maxHealth;
    playerStats.damage        = newDamage;
}



bool DialogueSystem::checkUnlockHabilities(LevelComponent &playerLevel) const {
    if(playerLevel.level == 3) {
        playerLevel.dashUnlocked = true;
        return true;
    }
    return false;
}


void DialogueSystem::updateLevelUpHUDEffect(GameEngine &gameContext) const {
    LevelComponent &playerLevel = gameContext.entityMan.getComponent<LevelComponent>(gameContext.playerId);
    if (playerLevel.levelingUp) {
        if (playerLevel.levelingTime >= 5) {
            playerLevel.levelingUp = false;
            playerLevel.levelingTime = 0;
        } else {
            playerLevel.levelingTime += gameContext.getDeltaTime();
        }
    }
}