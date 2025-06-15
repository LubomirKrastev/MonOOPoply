#include "Trade.h"
#include "Player.h"
#include "Property.h"
#include <iostream>

Trade::Trade(Player* p1, Player* p2)
    : player1(p1), player2(p2), moneyFrom1(0), moneyFrom2(0) {}

void Trade::addPropertyFromPlayer1(Property* property) {
    if (property->getOwner() == player1) {
        propertiesFrom1.pushBack(property);
    }
}

void Trade::addPropertyFromPlayer2(Property* property) {
    if (property->getOwner() == player2) {
        propertiesFrom2.pushBack(property);
    }
}

void Trade::setMoneyFromPlayer1(int amount) {
    moneyFrom1 = amount;
}

void Trade::setMoneyFromPlayer2(int amount) {
    moneyFrom2 = amount;
}

bool Trade::validateTrade() {
    if (player1->getBalance() < moneyFrom1 || player2->getBalance() < moneyFrom2) {
        return false;
    }

    for (size_t i = 0; i < propertiesFrom1.size(); i++) {
        if (propertiesFrom1[i]->getOwner() != player1) {
            return false;
        }
    }

    for (size_t i = 0; i < propertiesFrom2.size(); i++) {
        if (propertiesFrom2[i]->getOwner() != player2) {
            return false;
        }
    }

    return true;
}

bool Trade::executeTrade() {
    if (!validateTrade()) {
        std::cout << "Trade is invalid!" << std::endl;
        return false;
    }

    if (moneyFrom1 > 0) {
        player1->payMoney(moneyFrom1);
        player2->receiveMoney(moneyFrom1);
    }
    if (moneyFrom2 > 0) {
        player2->payMoney(moneyFrom2);
        player1->receiveMoney(moneyFrom2);
    }

    for (size_t i = 0; i < propertiesFrom1.size(); i++) {
        propertiesFrom1[i]->setOwner(player2);
    }

    for (size_t i = 0; i < propertiesFrom2.size(); i++) {
        propertiesFrom2[i]->setOwner(player1);
    }

    std::cout << "Trade executed successfully!" << std::endl;
    return true;
}