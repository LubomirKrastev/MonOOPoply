#include "Property.h"
#include "Player.h"
#include <iostream>

Property::Property(const MyString& name, int propertyPrice, int propertyRent)
    : Field(name), price(propertyPrice), rent(propertyRent), owner(nullptr) {}

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
        else if (rent < 0) {
            std::cout << player.getName() << " must pay $" << -rent << " tax." << std::endl;
            player.payMoney(-rent);
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
        std::cout << player.getName() << " pays $" << rent << " rent to " << owner->getName() << std::endl;
        player.payMoney(rent);
        owner->receiveMoney(rent);
    }
    else {
        std::cout << player.getName() << " owns this property." << std::endl;
    }
}

int Property::getPrice() const {
    return price;
}

int Property::getRent() const {
    return rent;
}

Player* Property::getOwner() const {
    return owner;
}

void Property::setOwner(Player* newOwner) {
    owner = newOwner;
}

bool Property::isOwned() const {
    return owner != nullptr;
}