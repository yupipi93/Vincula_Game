#pragma once
#include "Component.hpp"
#include <enum/EffectType.hpp>
#include <enum/WeaponType.hpp>

#include <vector>
#include <stdint.h>
#include <chrono>
#include <ctime>
#include <string>

class WeaponComponent : public Component {
public:
    explicit WeaponComponent(const uint32_t id);
    ~WeaponComponent() override;

    std::string     eventName           {"0"};


    float       damage                  {1};
    uint8_t     damageRange             {5};
    float       criticalPercent         {20};
    float       effectPercent           {20};

    uint8_t     biome                   {0};
    float       attackSize              {10};
    float       sizeSeparationAttacks   {0};

    uint8_t     posOrigin               {10};
    uint8_t     numAttacks              {1};

    float       maxCoolDown             {0.5}; //Cool down of Action 
    float       coolDown                {0};    
    float       attackApparitionTime    {0.5}; //Cool down of attackAparition

    EffectType  effect                  {NOEFFECT};
    WeaponType  weapon                  {UNKNOWN};

    std::vector<float> defaultPosition{0.f, 0.f, 0.f}; 
};