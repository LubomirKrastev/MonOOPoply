#pragma once
#include "Field.h"

class CardDeck;

class CardField : public Field {
private:
    CardDeck* deck;

public:
    CardField(const MyString& name, CardDeck* cardDeck);

    void onPlayerLanding(Player& player, int diceRoll = 0) override;
};