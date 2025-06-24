#include "CardField.h"
#include "CardDeck.h"
#include "Player.h"
#include <iostream>

CardField::CardField(const MyString& name, CardDeck* cardDeck)
    : Field(name), deck(cardDeck) {}

void CardField::onPlayerLanding(Player& player) {
    std::cout << player.getName() << " draws a " << name << " card." << std::endl;

    if (deck != nullptr) {
        Card* card = deck->drawCard();
        std::cout << "Card: " << card->getDescription() << std::endl;
        card->applyEffect(player);
    }
}   