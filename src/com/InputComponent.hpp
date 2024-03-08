#pragma once

#include "Component.hpp"
#include <chrono>
#include <array>
#include <ctime>

class InputComponent : public Component {
public:
    explicit InputComponent(const uint32_t id);
    ~InputComponent() override;

    void resetActions();
    void resetMenuActions();
    void subDeltaTime(const float deltaTime);

    bool    attacking               {false}; 
    bool    setingTarget             {false}; 
    bool    dancing                 {false};
    bool    interacting             {false};
    bool    lookingMap              {false};
    bool    lookingStats            {false};
    bool    dashing                 {false};
    bool    changingWeapon          {false};
    bool    callingCompass          {false};
    bool    dropingPalantir         {false};
    bool    storageCoin             {false};
    bool    obtainCoin              {false};
    bool    choppedZoomCamera       {false};
    bool    choppedCamera           {false};
    bool    smartCamera             {true};
    bool    zenithCamera            {false};
    bool    zenithUpCamera          {false};
    bool    airCamera               {false};

    bool    pressingW               {false};
    bool    pressingS               {false};
    bool    pressingA               {false};
    bool    pressingD               {false};

    bool    controlClickLeft        {false};
    bool    leftMouseClick          {false};

    bool    controlClickMiddle      {false};
    bool    middleMouseClick        {false};

    bool    controlEspKeyPress      {false};
    bool    espKeyPress             {false};

    std::array<float, 2> mousePosition          {0, 0};

    float   boredTime               {0}; 
    float   maxBoredTime            {12}; 
    bool    getBored                {false};
    bool    boring                  {false};

    float   changingWeaponTimer     {0};
    float   changingWeaponMaxTimer  {0.25};

    float   interactingTimer        {0};
    float   interactingMaxTimer     {0.75};

    float   callingCompassTimer     {0};
    float   callingCompassMaxTimer  {0.75};

    float   bankCoinsMaxTimer       {0.05};

    bool    upKeyPress              {false};
    bool    controlUpKeyPress       {false};
    bool    downKeyPress            {false};
    bool    controlDownKeyPress     {false};
    bool    returnKeyPress          {false};
    bool    controlReturnKeyPress   {false};
    bool    escKeyPress             {false};
    bool    controlEscKeyPress      {false};

    bool    doingAttack             {false}; //True only while attack animation is active
    float   attackingTimer          {0};     //Increases while attack animation is active and reset when finished
    bool    attacked                {false}; //True when attackEntity appear and false when attack animation finish

    bool    doingDance              {false}; //True only while dance animation is active


};