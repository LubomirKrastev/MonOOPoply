#pragma once
#include "MyString.h"

class Player {
private:
    MyString name;
    int balance;
    int position;
    bool inJail;
    int turnsInJail;

public:
    Player(const MyString& playerName, int startingBalance = 1500);

    const MyString& getName() const;
    int getBalance() const;
    int getPosition() const;
    bool isInJail() const;

    void moveForward(int steps);
    void moveTo(int newPosition);
    void goToJail();
    bool tryToGetOutOfJail();

    bool canAfford(int amount) const;
    void payMoney(int amount);
    void receiveMoney(int amount);

    bool isBankrupt() const;
};