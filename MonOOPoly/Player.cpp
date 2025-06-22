#include "Player.h"
#include <iostream>
#include <cstdlib>

const unsigned FIELDS = 40;
const unsigned JAIL_POSITION = 10;

Player::Player(const MyString& playerName, int startingBalance)
    : name(playerName), balance(startingBalance), position(0), inJail(false), turnsInJail(0) {}

const MyString& Player::getName() const {
    return name;
}

int Player::getBalance() const {
    return balance;
}

int Player::getPosition() const {
    return position;
}

bool Player::isInJail() const {
    return inJail;
}

void Player::moveForward(int steps) {
    if (!inJail) {
        int oldPosition = position;
        position = (position + steps) % FIELDS; 
        if (position < oldPosition) {
            receiveMoney(200);
            std::cout << name << " passed GO and collected $200!" << std::endl;
        }
    }
}

void Player::moveTo(int newPosition) {
    position = newPosition;
}

void Player::goToJail() {
    inJail = true;
    turnsInJail = 0;
    position = JAIL_POSITION;
    std::cout << name << " is sent to jail!" << std::endl;
}

bool Player::tryToGetOutOfJail() {
    turnsInJail++;

    int die1 = (rand() % 6) + 1;
    int die2 = (rand() % 6) + 1;

    std::cout << name << " rolls " << die1 << " and " << die2 << std::endl;

    if (die1 == die2) {
        inJail = false;
        turnsInJail = 0;
        std::cout << name << " rolled doubles and is free from jail!" << std::endl;

        moveForward(die1 + die2);
        return true;
    }

    if (turnsInJail == 3) {
        std::cout << name << " served 3 turns in jail and must leave." << std::endl;
        inJail = false;
        turnsInJail = 0;

        moveForward(die1 + die2);
        return true;
    }

    std::cout << name << " remains in jail." << std::endl;
    return false;
}

bool Player::canAfford(int amount) const {
    return balance >= amount;
}

void Player::payMoney(int amount) {
    balance -= amount;
    if (balance < 0) {
        std::cout << name << " is bankrupt!" << std::endl;
    }
}

void Player::receiveMoney(int amount) {
    balance += amount;
}

bool Player::isBankrupt() const {
    return balance < 0;
}