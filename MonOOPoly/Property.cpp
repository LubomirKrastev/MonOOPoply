#include "Property.h"
#include "Player.h"
#include "Mortgage.h"
#include "ColorGroup.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

Property::Property(const MyString& name, int propertyPrice, int propertyRent)
    : Field(name), price(propertyPrice), baseRent(propertyRent),
    currentRent(propertyRent), owner(nullptr), mortgage(nullptr),
    colorGroup(nullptr), houseCount(0), hotelCount(0), isMortgaged(false),
    rentWith1House(0), rentWith2Houses(0), rentWith3Houses(0),
    rentWith4Houses(0), rentWithHotel(0) {}

Property::Property(const MyString& name, int propertyPrice, int propertyRent,
    int rent1H, int rent2H, int rent3H, int rent4H, int rentHotel)
    : Field(name), price(propertyPrice), baseRent(propertyRent),
    currentRent(propertyRent), owner(nullptr), mortgage(nullptr),
    colorGroup(nullptr), houseCount(0), hotelCount(0), isMortgaged(false),
    rentWith1House(rent1H), rentWith2Houses(rent2H),
    rentWith3Houses(rent3H), rentWith4Houses(rent4H),
    rentWithHotel(rentHotel) {}

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
            std::cin.ignore();
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
        if (isMortgaged) {
            std::cout << name << " is mortgaged - no rent due!" << std::endl;
        }
        else if (owner->isInJail()) {
            std::cout << owner->getName() << " is in jail and cannot collect rent." << std::endl;
        }
        else {
            if (name == "Electric Company" || name == "Water Works") {
                std::cout << player.getName() << " must pay utility rent to " << owner->getName() << std::endl;

                int die1 = (rand() % 6) + 1;
                int die2 = (rand() % 6) + 1;
                int diceTotal = die1 + die2;

                std::cout << player.getName() << " rolls " << die1 << " and " << die2
                    << " (Total: " << diceTotal << ")" << std::endl;

                int utilityRent = diceTotal * currentRent;

                std::cout << "Rent: " << diceTotal << " x " << currentRent
                    << " = $" << utilityRent << std::endl;

                player.payMoney(utilityRent);
                owner->receiveMoney(utilityRent);
            }
            else {
                std::cout << player.getName() << " pays $" << currentRent << " rent to "
                    << owner->getName() << std::endl;
                player.payMoney(currentRent);
                owner->receiveMoney(currentRent);
            }
        }
    }
    else {
        std::cout << player.getName() << " owns this property." << std::endl;

        if (isMortgaged) {
            std::cout << "This property is MORTGAGED." << std::endl;
            std::cout << "Unmortgage for $" << (int)((price / 2) * 1.1) << "? (y/n): ";
            char choice;
            std::cin >> choice;
            std::cin.ignore();
            if (choice == 'y' || choice == 'Y') {
                unmortgageProperty();
            }
            return;
        }

        if (colorGroup != nullptr) {
            std::cout << "Property group: " << colorGroup->getName() << std::endl;

            if (hasMonopoly()) {
                std::cout << "You have a MONOPOLY on this color group!" << std::endl;
                std::cout << "Current development: " << houseCount << " houses";
                if (hotelCount > 0) {
                    std::cout << ", 1 hotel";
                }
                std::cout << std::endl;

                if (hotelCount == 0) {  
                    int maxHousesCanBuild = 0;
                    int houseCost = colorGroup->getHouseCost();

                    const Vector<Property*>& groupProps = colorGroup->getProperties();
                    int minHouses = 4;

                    for (size_t i = 0; i < groupProps.size(); i++) {
                        if (groupProps[i] != this && groupProps[i]->houseCount < minHouses) {
                            minHouses = groupProps[i]->houseCount;
                        }
                    }

                    int maxAllowedHouses = minHouses + 1;
                    if (maxAllowedHouses > 4) maxAllowedHouses = 4;

                    maxHousesCanBuild = maxAllowedHouses - houseCount;

                    int canAffordHouses = player.getBalance() / houseCost;
                    if (maxHousesCanBuild > canAffordHouses) {
                        maxHousesCanBuild = canAffordHouses;
                    }

                    if (maxHousesCanBuild > 0) {
                        std::cout << "You can build up to " << maxHousesCanBuild
                            << " house(s) on this property." << std::endl;
                        std::cout << "House cost: $" << houseCost << " each." << std::endl;
                        std::cout << "How many houses would you like to build? (0-"
                            << maxHousesCanBuild << "): ";

                        int housesToBuild;
                        std::cin >> housesToBuild;
                        std::cin.ignore();

                        if (housesToBuild > 0 && housesToBuild <= maxHousesCanBuild) {
                            int totalCost = housesToBuild * houseCost;
                            std::cout << "Building " << housesToBuild << " house(s) for $"
                                << totalCost << "..." << std::endl;

                            player.payMoney(totalCost);
                            for (int i = 0; i < housesToBuild; i++) {
                                buildHouse();
                            }

                            std::cout << "Total houses on " << name << ": " << houseCount << std::endl;
                        }
                    }
                    else if (houseCount == 4) {
                        if (canBuildHotel()) {
                            int hotelCost = colorGroup->getHotelCost();
                            if (player.canAfford(hotelCost)) {
                                std::cout << "All properties in this group have 4 houses!" << std::endl;
                                std::cout << "Build a hotel for $" << hotelCost << "? (y/n): ";
                                char buildChoice;
                                std::cin >> buildChoice;
                                std::cin.ignore();

                                if (buildChoice == 'y' || buildChoice == 'Y') {
                                    player.payMoney(hotelCost);
                                    buildHotel();
                                }
                            }
                            else {
                                std::cout << "You need $" << hotelCost << " to build a hotel." << std::endl;
                            }
                        }
                        else {
                            std::cout << "You need 4 houses on ALL properties in this group before building hotels." << std::endl;
                        }
                    }
                    else {
                        std::cout << "You must build evenly across all properties." << std::endl;
                        
                        const Vector<Property*>& groupProps = colorGroup->getProperties();
                        std::cout << "\nCurrent development in " << colorGroup->getName() << " group:" << std::endl;
                        for (size_t i = 0; i < groupProps.size(); i++) {
                            std::cout << "  " << groupProps[i]->getName() << ": "
                                << groupProps[i]->houseCount << " houses";
                            if (groupProps[i]->hotelCount > 0) {
                                std::cout << " (HOTEL)";
                            }
                            std::cout << std::endl;
                        }
                    }
                }
                else {
                    std::cout << "This property has a hotel - maximum development reached!" << std::endl;
                }

                if (!hasMortgage() && player.canAfford(50)) {
                    std::cout << "\nAdd special improvements? (c)ottage +$50 or (C)astle +$100? (n for no): ";
                    char improveChoice;
                    std::cin >> improveChoice;
                    std::cin.ignore();

                    if (improveChoice == 'c' && player.canAfford(50)) {
                        player.payMoney(50);
                        addMortgage(new Cottage());
                        std::cout << "Cottage added! Rent increased by 15%." << std::endl;
                    }
                    else if (improveChoice == 'C' && player.canAfford(100)) {
                        player.payMoney(100);
                        addMortgage(new Castle());
                        std::cout << "Castle added! Rent increased by 50%." << std::endl;
                    }
                }
            }
            else {
                std::cout << "You need a monopoly to build. Properties in " << colorGroup->getName() << " group:" << std::endl;

                const Vector<Property*>& groupProps = colorGroup->getProperties();
                for (size_t i = 0; i < groupProps.size(); i++) {
                    std::cout << "  - " << groupProps[i]->getName() << ": ";
                    if (groupProps[i]->getOwner() == nullptr) {
                        std::cout << "UNOWNED";
                    }
                    else if (groupProps[i]->getOwner() == &player) {
                        std::cout << "You own this";
                    }
                    else {
                        std::cout << "Owned by " << groupProps[i]->getOwner()->getName();
                    }
                    std::cout << std::endl;
                }
            }
        }
        else {
            if (name.c_str()[0] != '\0' &&
                (strstr(name.c_str(), "Railroad") != nullptr ||
                    strstr(name.c_str(), "Electric") != nullptr ||
                    strstr(name.c_str(), "Water") != nullptr)) {
                std::cout << "Special property - no building allowed." << std::endl;
            }
        }
        }
}

int Property::getPrice() const { return price; }
int Property::getRent() const { return currentRent; }
int Property::getBaseRent() const { return baseRent; }
Player* Property::getOwner() const { return owner; }
bool Property::isOwned() const { return owner != nullptr; }
int Property::getHouseCount() const { return houseCount; }
int Property::getHotelCount() const { return hotelCount; }
ColorGroup* Property::getColorGroup() const { return colorGroup; }
bool Property::getIsMortgaged() const { return isMortgaged; }
Mortgage* Property::getMortgage() const { return mortgage; }

void Property::setOwner(Player* newOwner) {
    owner = newOwner;

    if (newOwner == nullptr) {
        removeAllBuildings();
        removeMortgage();
        isMortgaged = false;
    }
}

void Property::setColorGroup(ColorGroup* group) {
    colorGroup = group;
    if (group != nullptr) {
        group->addProperty(this);
    }
}

void Property::setRent(int newRent) {
    currentRent = newRent;
}

bool Property::hasMonopoly() const {
    if (colorGroup == nullptr || owner == nullptr) {
        return false;
    }
    return colorGroup->isMonopoly(owner);
}

bool Property::canBuildHouse() const {
    if (!hasMonopoly() || houseCount >= 4 || hotelCount > 0 || isMortgaged) {
        return false;
    }

    const Vector<Property*>& groupProps = colorGroup->getProperties();
    for (size_t i = 0; i < groupProps.size(); i++) {
        if (groupProps[i] != this && groupProps[i]->houseCount < houseCount) {
            return false;
        }
    }

    return true;
}

bool Property::canBuildHotel() const {
    if (!hasMonopoly() || hotelCount > 0 || isMortgaged) {
        return false;
    }

    const Vector<Property*>& groupProps = colorGroup->getProperties();
    for (size_t i = 0; i < groupProps.size(); i++) {
        if (groupProps[i]->houseCount != 4) {
            return false;
        }
    }

    return true;
}

void Property::buildHouse() {
    if (canBuildHouse()) {
        houseCount++;
        updateRent();
        std::cout << "House built on " << name << "! Total houses: " << houseCount << std::endl;
    }
}

void Property::buildHotel() {
    if (canBuildHotel()) {
        houseCount = 0;
        hotelCount = 1;
        updateRent();
        std::cout << "Hotel built on " << name << "!" << std::endl;
    }
}

void Property::sellHouse() {
    if (houseCount > 0 && colorGroup != nullptr) {
        houseCount--;
        int sellPrice = colorGroup->getHouseCost() / 2;
        if (owner != nullptr) {
            owner->receiveMoney(sellPrice);
        }
        updateRent();
        std::cout << "Sold 1 house for $" << sellPrice << std::endl;
    }
}

void Property::sellHotel() {
    if (hotelCount > 0 && colorGroup != nullptr) {
        hotelCount = 0;
        houseCount = 4; 
        int sellPrice = colorGroup->getHotelCost() / 2;
        if (owner != nullptr) {
            owner->receiveMoney(sellPrice);
        }
        updateRent();
        std::cout << "Sold hotel for $" << sellPrice << " (converted to 4 houses)" << std::endl;
    }
}

void Property::sellAllBuildings() {
    int totalMoney = 0;

    if (colorGroup != nullptr) {
        if (hotelCount > 0) {
            totalMoney += colorGroup->getHotelCost() / 2;
            hotelCount = 0;
        }

        totalMoney += (houseCount * colorGroup->getHouseCost()) / 2;
        houseCount = 0;

        if (totalMoney > 0 && owner != nullptr) {
            owner->receiveMoney(totalMoney);
            updateRent();
            std::cout << "Sold all buildings for $" << totalMoney << std::endl;
        }
    }
}

void Property::mortgageProperty() {
    if (!isMortgaged && houseCount == 0 && hotelCount == 0 && owner != nullptr) {
        isMortgaged = true;
        int mortgageValue = price / 2;
        owner->receiveMoney(mortgageValue);
        std::cout << name << " mortgaged for $" << mortgageValue << std::endl;
    }
}

void Property::unmortgageProperty() {
    if (isMortgaged && owner != nullptr) {
        int unmortgageCost = (int)((price / 2) * 1.1);
        if (owner->canAfford(unmortgageCost)) {
            owner->payMoney(unmortgageCost);
            isMortgaged = false;
            std::cout << name << " unmortgaged for $" << unmortgageCost << std::endl;
        }
        else {
            std::cout << "You need $" << unmortgageCost << " to unmortgage this property." << std::endl;
        }
    }
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
    if (isMortgaged) {
        currentRent = 0;
        return;
    }

    if (hotelCount > 0 && rentWithHotel > 0) {
        currentRent = rentWithHotel;
    }
    else if (houseCount > 0 && rentWith1House > 0) {
        switch (houseCount) {
        case 1: currentRent = rentWith1House; break;
        case 2: currentRent = rentWith2Houses; break;
        case 3: currentRent = rentWith3Houses; break;
        case 4: currentRent = rentWith4Houses; break;
        }
    }
    else if (hasMonopoly() && baseRent > 0) {
        currentRent = baseRent * 2; 
    }
    else {
        currentRent = baseRent;
    }

    if (mortgage != nullptr) {
        currentRent = mortgage->calculateNewRent(currentRent);
    }
}

void Property::removeAllBuildings() {
    houseCount = 0;
    hotelCount = 0;
    updateRent();
}