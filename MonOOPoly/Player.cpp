#include "Player.h"
#include <iostream>
#include <cstdlib>

const unsigned FIELDS = 40;
const unsigned JAIL_POSITION = 10;
const int JAIL_FEE = 100;

Player::Player(const MyString& playerName, int startingBalance)
    : name(playerName), balance(startingBalance), position(0),
    inJail(false), turnsInJail(0), getOutOfJailCards(0) {}

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

int Player::getTurnsInJail() const {
    return turnsInJail;
}

int Player::getGetOutOfJailCards() const {
    return getOutOfJailCards;
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

int Player::tryToGetOutOfJail() {
    turnsInJail++;

    if (hasGetOutOfJailCard()) {
        std::cout << name << " has a 'Get Out of Jail Free' card. Use it? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            useGetOutOfJailCard();
            inJail = false;
            turnsInJail = 0;

            int die1 = (rand() % 6) + 1;
            int die2 = (rand() % 6) + 1;
            std::cout << name << " rolls " << die1 << " and " << die2 << std::endl;
            moveForward(die1 + die2);

            return die1 + die2; 
        }
    }

    if (turnsInJail < 3 && canAfford(JAIL_FEE)) {
        std::cout << name << " is in jail (turn " << turnsInJail << "/3)." << std::endl;
        std::cout << "Pay $" << JAIL_FEE << " to get out? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'y' || choice == 'Y') {
            payMoney(JAIL_FEE);
            inJail = false;
            turnsInJail = 0;
            std::cout << name << " paid $" << JAIL_FEE << " and is free from jail!" << std::endl;

            int die1 = (rand() % 6) + 1;
            int die2 = (rand() % 6) + 1;
            std::cout << name << " rolls " << die1 << " and " << die2 << std::endl;
            moveForward(die1 + die2);

            return die1 + die2;  
        }
    }

    std::cout << name << " tries to roll doubles to get out of jail" << std::endl;

    int die1 = (rand() % 6) + 1;
    int die2 = (rand() % 6) + 1;

    std::cout << name << " rolls " << die1 << " and " << die2 << std::endl;

    if (die1 == die2) {
        inJail = false;
        turnsInJail = 0;
        std::cout << name << " rolled doubles and is free from jail!" << std::endl;
        moveForward(die1 + die2);
        return die1 + die2; 
    }

    if (turnsInJail >= 3) {
        std::cout << name << " served 3 turns in jail and must leave." << std::endl;
        inJail = false;
        turnsInJail = 0;
        moveForward(die1 + die2);
        return die1 + die2; 
    }

    std::cout << name << " remains in jail. (" << (3 - turnsInJail) << " turns left)" << std::endl;
    return 0;  
}

void Player::incrementJailTurn() {
    turnsInJail++;
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

void Player::addGetOutOfJailCard() {
    getOutOfJailCards++;
}

bool Player::hasGetOutOfJailCard() const {
    return getOutOfJailCards > 0;
}

void Player::useGetOutOfJailCard() {
    if (getOutOfJailCards > 0) {
        getOutOfJailCards--;
        std::cout << name << " used a 'Get Out of Jail Free' card!" << std::endl;
    }
}