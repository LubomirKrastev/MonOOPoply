#pragma once
#include "MyString.h"

class Player {
private:
    MyString name;
    int balance;
    int position;
    bool inJail;
    int turnsInJail;
    int getOutOfJailCards;

public:
    Player(const MyString& playerName, int startingBalance = 1500);

    const MyString& getName() const;
    int getBalance() const;
    int getPosition() const;
    bool isInJail() const;
    int getTurnsInJail() const;
    int getGetOutOfJailCards() const;

    void moveForward(int steps);
    void moveTo(int newPosition);
    void goToJail();
    int tryToGetOutOfJail();
    void incrementJailTurn();

    bool canAfford(int amount) const;
    void payMoney(int amount);
    void receiveMoney(int amount);

    bool tryToPayDebt(int amount);
    int calculateNetWorth() const;
    bool handleDebt(int amountOwed, Player* creditor = nullptr);

    bool isBankrupt() const;

    void addGetOutOfJailCard();
    bool hasGetOutOfJailCard() const;
    void useGetOutOfJailCard();
};