#pragma once
#include "Card.h"

class MovePositionCard : public Card {
private:
    int steps;

public:
    MovePositionCard(const MyString& desc, int moveSteps);

    void applyEffect(Player& player) override;
};