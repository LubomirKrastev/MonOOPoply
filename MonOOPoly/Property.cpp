#include "Property.h"
#include "Player.h"
#include "Mortgage.h"
#include <iostream>

Property::Property(const MyString& name, int propertyPrice, int propertyRent)
    : Field(name), price(propertyPrice), baseRent(propertyRent),
    currentRent(propertyRent), owner(nullptr), mortgage(nullptr) {}

Property::~Property() {
    delete mortgage;
}

void Property::onPlayerLanding(Player& player) {
    if (price == 0) {
        if (name == "GO") {
            std::cout << player.getName() << " is on GO." << std::endl;
        }
        else if (name == "Jail/Just Visiting") {
            std::cout << player.getName() << " is just visiting jail." << std::endl;
        }
        else if (name == "Free Parking") {
            std::cout << player.getName() << " is on Free Parking." << std::endl;
        }
        else if (baseRent < 0) {
            std::cout << player.getName() << " must pay $" << -baseRent << " tax." << std::endl;
            player.payMoney(-baseRent);
        }
        return;
    }

    if (!isOwned()) {
        if (player.canAfford(price)) {
            std::cout << player.getName() << " can buy " << name << " for $" << price << ". Buy? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                player.payMoney(price);
                setOwner(&player);
                std::cout << player.getName() << " bought " << name << "!" << std::endl;
            }
        }
        else {
            std::cout << player.getName() << " cannot afford " << name << std::endl;
        }
    }
    else if (owner != &player) {
        std::cout << player.getName() << " pays $" << currentRent << " rent to " << owner->getName() << std::endl;
        player.payMoney(currentRent);
        owner->receiveMoney(currentRent);
    }
    else {
        std::cout << player.getName() << " owns this property." << std::endl;

        // Опция за добавяне на подобрения
        if (!hasMortgage() && player.canAfford(50)) {
            std::cout << "Would you like to build on this property? ";
            std::cout << "(c)ottage for $50 or (C)astle for $100? (n for no): ";
            char buildChoice;
            std::cin >> buildChoice;

            if (buildChoice == 'c' && player.canAfford(50)) {
                player.payMoney(50);
                addMortgage(new Cottage());
                std::cout << "Cottage built! Rent increased to $" << currentRent << std::endl;
            }
            else if (buildChoice == 'C' && player.canAfford(100)) {
                player.payMoney(100);
                addMortgage(new Castle());
                std::cout << "Castle built! Rent increased to $" << currentRent << std::endl;
            }
        }
    }
}

int Property::getPrice() const {
    return price;
}

int Property::getRent() const {
    return currentRent;
}

int Property::getBaseRent() const {
    return baseRent;
}

Player* Property::getOwner() const {
    return owner;
}

void Property::setOwner(Player* newOwner) {
    owner = newOwner;
    
    if (newOwner == nullptr) {
        removeMortgage();
    }
}

bool Property::isOwned() const {
    return owner != nullptr;
}

void Property::addMortgage(Mortgage* newMortgage) {
    delete mortgage;
    mortgage = newMortgage;
    updateRent();
}

void Property::removeMortgage() {
    delete mortgage;
    mortgage = nullptr;
    updateRent();
}

bool Property::hasMortgage() const {
    return mortgage != nullptr;
}

void Property::updateRent() {
    if (mortgage != nullptr) {
        currentRent = mortgage->calculateNewRent(baseRent);
    }
    else {
        currentRent = baseRent;
    }
}