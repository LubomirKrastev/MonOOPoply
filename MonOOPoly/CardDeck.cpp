#include "CardDeck.h"

CardDeck::CardDeck() : currentIndex(0) {}

CardDeck::~CardDeck() {
    for (size_t i = 0; i < cards.size(); i++) {
        delete cards[i];
    }
}

void CardDeck::addCard(Card* card) {
    cards.pushBack(card);
}

Card* CardDeck::drawCard() {
    if (cards.empty()) {
        return nullptr;
    }

    Card* drawnCard = cards[currentIndex];

    currentIndex++;

    if (currentIndex >= cards.size()) {
        currentIndex = 0;
    }

    return drawnCard;
}