#include "Bank.h"
#include "Player.h"
#include "Property.h"
#include <iostream>

const unsigned TOTAL_MONEY = 20580;

Bank* Bank::instance = nullptr;

Bank::Bank() : totalMoney(TOTAL_MONEY) {}

Bank* Bank::getInstance() {
    if (instance == nullptr) {
        instance = new Bank();
    }
    return instance;
}

void Bank::cleanup() {
    delete instance;
    instance = nullptr;
}

void Bank::distributeStartingMoney(Vector<Player*>& players) {
    for (size_t i = 0; i < players.size(); i++) {
        totalMoney -= 1500;
    }
}

void Bank::collectPayment(int amount) {
    totalMoney += amount;
}

void Bank::makePayment(int amount) {
    if (totalMoney >= amount) {
        totalMoney -= amount;
    }
    else {
        std::cout << "Bank is running low on money!" << std::endl;
    }
}

void Bank::handleBankruptcy(Player* player, Vector<Property*>& properties) {
    std::cout << player->getName() << " is bankrupt! All properties return to the bank." << std::endl;

    for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->getOwner() == player) {
            properties[i]->setOwner(nullptr);
        }
    }
}