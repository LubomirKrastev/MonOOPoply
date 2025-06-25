#pragma once
#include "Field.h"

class GoToJailField : public Field {
public:
    GoToJailField();

    void onPlayerLanding(Player& player, int diceRoll = 0) override;
};