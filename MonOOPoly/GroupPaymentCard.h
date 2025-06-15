#pragma once
#include "Card.h"
#include "Vector.hpp"

class GroupPaymentCard : public Card {
private:
    int amountPerPlayer;
    Vector<Player*>* allPlayers;

public:
    GroupPaymentCard(const MyString& desc, int amount, Vector<Player*>* players);

    void applyEffect(Player& player) override;
};