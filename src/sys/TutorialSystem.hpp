#pragma once

#include "System.hpp"
#include <com/ComponentsIncluder>

class TutorialSystem : public System {
public:
    TutorialSystem();
    ~TutorialSystem();

    void update(GameEngine &gameContext) const override;

private:

};