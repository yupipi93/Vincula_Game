#pragma once

#include "Component.hpp"
#include <enum/SentenceType.hpp>
#include <enum/WeaponType.hpp>

#include <vector>
#include <string>
#include <cstdint>

struct Sentence {
    std::string  sentence     {"Default Sentence"};
    SentenceType sentenceType {REPORT};
};

class DialogueComponent : public Component {
public:
    explicit DialogueComponent(const uint32_t id);
    ~DialogueComponent() override;

    bool wantToTalk     {false};  //if wantToTalk. you can interaction with him.
    bool loopDialogue   {false};  //if loopDialogue, he repeats all the dialogue in loop.
    bool uniqueSentence {false};  //if uniqueSentence, his wantToTalk turn to true when dialogue has finished.

    bool dialogueActive {false};  //if dialogueActive, the dialogue will be drawn
    float dialogueTimer {0.f};

    uint8_t currentSentence         	{0};
    bool startedDialogue				{false};
    std::vector<Sentence> sentences 	{};

    bool givingCoinsToPlayer			{false};

    WeaponType weaponForSale        	{UNKNOWN};
    uint16_t    coinsToReceive   	   	{0};

    uint8_t 	biome                  	{0};

    uint32_t	interrogation		   	{0};

};