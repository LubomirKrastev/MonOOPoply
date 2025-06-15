#pragma once
#include "Card.h"

class PaymentCard : public Card {
private:
    int amount;

public:
    PaymentCard(const MyString& desc, int paymentAmount);

    void applyEffect(Player& player) override;
};