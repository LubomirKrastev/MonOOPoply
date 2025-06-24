#include "Trade.h"
#include "Player.h"
#include "Property.h"
#include "ColorGroup.h"
#include <iostream>

Trade::Trade(Player* p1, Player* p2)
    : player1(p1), player2(p2), moneyFrom1(0), moneyFrom2(0) {}

void Trade::addPropertyFromPlayer1(Property* property) {
    if (property != nullptr && property->getOwner() == player1) {
        propertiesFrom1.pushBack(property);
    }
}

void Trade::addPropertyFromPlayer2(Property* property) {
    if (property != nullptr && property->getOwner() == player2) {
        propertiesFrom2.pushBack(property);
    }
}

void Trade::setMoneyFromPlayer1(int amount) {
    if (amount >= 0) {
        moneyFrom1 = amount;
    }
}

void Trade::setMoneyFromPlayer2(int amount) {
    if (amount >= 0) {
        moneyFrom2 = amount;
    }
}

bool Trade::validateTrade() {
    if (player1->getBalance() < moneyFrom1) {
        std::cout << player1->getName() << " doesn't have $" << moneyFrom1 << std::endl;
        return false;
    }

    if (player2->getBalance() < moneyFrom2) {
        std::cout << player2->getName() << " doesn't have $" << moneyFrom2 << std::endl;
        return false;
    }

    for (size_t i = 0; i < propertiesFrom1.size(); i++) {
        if (propertiesFrom1[i]->getOwner() != player1) {
            std::cout << player1->getName() << " doesn't own "
                << propertiesFrom1[i]->getName() << std::endl;
            return false;
        }

        if (propertiesFrom1[i]->getHouseCount() > 0 || propertiesFrom1[i]->getHotelCount() > 0) {
            std::cout << "Cannot trade " << propertiesFrom1[i]->getName()
                << " - it has buildings!" << std::endl;
            return false;
        }
    }

    for (size_t i = 0; i < propertiesFrom2.size(); i++) {
        if (propertiesFrom2[i]->getOwner() != player2) {
            std::cout << player2->getName() << " doesn't own "
                << propertiesFrom2[i]->getName() << std::endl;
            return false;
        }

        if (propertiesFrom2[i]->getHouseCount() > 0 || propertiesFrom2[i]->getHotelCount() > 0) {
            std::cout << "Cannot trade " << propertiesFrom2[i]->getName()
                << " - it has buildings!" << std::endl;
            return false;
        }
    }

    if (propertiesFrom1.empty() && propertiesFrom2.empty() && moneyFrom1 == 0 && moneyFrom2 == 0) {
        std::cout << "Nothing to trade!" << std::endl;
        return false;
    }

    return true;
}

void Trade::transferProperties() {
    for (size_t i = 0; i < propertiesFrom1.size(); i++) {
        propertiesFrom1[i]->setOwner(player2);
        std::cout << propertiesFrom1[i]->getName() << " transferred to "
            << player2->getName() << std::endl;
    }

    for (size_t i = 0; i < propertiesFrom2.size(); i++) {
        propertiesFrom2[i]->setOwner(player1);
        std::cout << propertiesFrom2[i]->getName() << " transferred to "
            << player1->getName() << std::endl;
    }
}

void Trade::transferMoney() {
    if (moneyFrom1 > 0) {
        player1->payMoney(moneyFrom1);
        player2->receiveMoney(moneyFrom1);
        std::cout << player1->getName() << " paid $" << moneyFrom1
            << " to " << player2->getName() << std::endl;
    }

    if (moneyFrom2 > 0) {
        player2->payMoney(moneyFrom2);
        player1->receiveMoney(moneyFrom2);
        std::cout << player2->getName() << " paid $" << moneyFrom2
            << " to " << player1->getName() << std::endl;
    }
}

void Trade::displayTradeDetails() const {
    std::cout << "\n=== TRADE DETAILS ===" << std::endl;
    std::cout << player1->getName() << " offers:" << std::endl;

    if (moneyFrom1 > 0) {
        std::cout << "  - $" << moneyFrom1 << std::endl;
    }

    for (size_t i = 0; i < propertiesFrom1.size(); i++) {
        std::cout << "  - " << propertiesFrom1[i]->getName()
            << " ($" << propertiesFrom1[i]->getPrice() << ")" << std::endl;
    }

    if (moneyFrom1 == 0 && propertiesFrom1.empty()) {
        std::cout << "  - Nothing" << std::endl;
    }

    std::cout << "\n" << player2->getName() << " offers:" << std::endl;

    if (moneyFrom2 > 0) {
        std::cout << "  - $" << moneyFrom2 << std::endl;
    }

    for (size_t i = 0; i < propertiesFrom2.size(); i++) {
        std::cout << "  - " << propertiesFrom2[i]->getName()
            << " ($" << propertiesFrom2[i]->getPrice() << ")" << std::endl;
    }

    if (moneyFrom2 == 0 && propertiesFrom2.empty()) {
        std::cout << "  - Nothing" << std::endl;
    }
}

bool Trade::executeTrade() {
    displayTradeDetails();

    if (!validateTrade()) {
        std::cout << "Trade is invalid!" << std::endl;
        return false;
    }

    std::cout << player1->getName() << ", do you accept this trade? (y/n): ";
    char choice1;
    std::cin >> choice1;
    std::cin.ignore();

    if (choice1 != 'y' && choice1 != 'Y') {
        std::cout << player1->getName() << " rejected the trade!" << std::endl;
        return false;
    }

    std::cout << player2->getName() << ", do you accept this trade? (y/n): ";
    char choice2;
    std::cin >> choice2;
    std::cin.ignore();

    if (choice2 != 'y' && choice2 != 'Y') {
        std::cout << player2->getName() << " rejected the trade!" << std::endl;
        return false;
    }

    transferMoney();
    transferProperties();

    std::cout << "\nTrade executed successfully!" << std::endl;

    return true;
}