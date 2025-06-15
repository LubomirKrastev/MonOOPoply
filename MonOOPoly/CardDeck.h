#pragma once
#include "Vector.hpp"
#include "Card.h"

class CardDeck {
private:
    Vector<Card*> cards;
    int currentIndex;

    void shuffle();

public:
    CardDeck();
    ~CardDeck();

    void addCard(Card* card);
    Card* drawCard();

    CardDeck(const CardDeck&) = delete;
    CardDeck& operator=(const CardDeck&) = delete;
};