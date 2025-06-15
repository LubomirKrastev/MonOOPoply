#pragma once
#include "Vector.hpp"

class Player;
class Property;

class Bank {
private:
    static Bank* instance;
    int totalMoney;

    Bank();

public:
    static Bank* getInstance();

    void distributeStartingMoney(Vector<Player*>& players);
    void collectPayment(int amount);
    void makePayment(int amount);

    void handleBankruptcy(Player* player, Vector<Property*>& properties);

    Bank(const Bank&) = delete;
    Bank& operator=(const Bank&) = delete;
};