#include "PaymentCard.h"
#include "Player.h"
#include "Bank.h"
#include <iostream>

PaymentCard::PaymentCard(const MyString& desc, int paymentAmount)
    : Card(desc), amount(paymentAmount) {}

void PaymentCard::applyEffect(Player& player) {
    if (amount > 0) {
        std::cout << player.getName() << " receives $" << amount << std::endl;
        player.receiveMoney(amount);
        Bank::getInstance()->makePayment(amount);
    }
    else {
        std::cout << player.getName() << " pays $" << -amount << std::endl;
        player.payMoney(-amount);
        Bank::getInstance()->collectPayment(-amount);
    }
}