#include "CardDeck.h"
#include <cstdlib>
#include <ctime>

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
    if (currentIndex >= cards.size()) {
        shuffle();
        currentIndex = 0;
    }
    return cards[currentIndex++];
}

void CardDeck::shuffle() {
    for (size_t i = cards.size() - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        Card* temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}