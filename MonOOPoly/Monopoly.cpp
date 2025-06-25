#include "Monopoly.h"
#include "Player.h"
#include "Bank.h"
#include "Property.h"
#include "SpecialCard.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include "Mortgage.h"
#include "ColorGroup.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>

Monopoly::Monopoly() : currentPlayerIndex(0), gameEnded(false), doublesCount(0) {
    srand(static_cast<unsigned>(time(nullptr)));

    board = new Board();
    bank = Bank::getInstance();
    chanceDeck = new CardDeck();
    communityChestDeck = new CardDeck();
}

Monopoly::~Monopoly() {
    delete chanceDeck;
    delete communityChestDeck;
    delete board;

    for (size_t i = 0; i < players.size(); i++) {
        delete players[i];
    }
}

void Monopoly::initializePlayers() {
    int numPlayers;
    std::cout << "Enter number of players (2-6): ";
    std::cin >> numPlayers;
    std::cin.ignore();

    if (numPlayers < 2) numPlayers = 2;
    if (numPlayers > 6) numPlayers = 6;

    for (int i = 0; i < numPlayers; i++) {
        char nameBuffer[100];
        std::cout << "Enter name for Player " << (i + 1) << ": ";
        std::cin.getline(nameBuffer, 99);

        Player* newPlayer = new Player(MyString(nameBuffer));
        players.pushBack(newPlayer);
    }

    bank->distributeStartingMoney(players);
}

void Monopoly::initializeDecks() {
    chanceDeck->addCard(new SpecialCard("Advance to GO", CardType::ADVANCE_TO_GO));
    chanceDeck->addCard(new SpecialCard("Advance to Illinois Ave", CardType::ADVANCE_TO_ILLINOIS));
    chanceDeck->addCard(new SpecialCard("Advance to Boardwalk", CardType::ADVANCE_TO_BOARDWALK));
    chanceDeck->addCard(new SpecialCard("Advance to nearest Railroad", CardType::ADVANCE_TO_RAILROAD));
    chanceDeck->addCard(new SpecialCard("Advance to nearest Utility", CardType::ADVANCE_TO_UTILITY));
    chanceDeck->addCard(new SpecialCard("Go back 3 spaces", CardType::GO_BACK_3_SPACES));
    chanceDeck->addCard(new SpecialCard("Go to Jail", CardType::GO_TO_JAIL));
    chanceDeck->addCard(new SpecialCard("Get Out of Jail Free", CardType::GET_OUT_OF_JAIL));
    chanceDeck->addCard(new PaymentCard("Bank pays you dividend of $50", 50));
    chanceDeck->addCard(new PaymentCard("Pay poor tax of $15", -15));

    communityChestDeck->addCard(new PaymentCard("Bank error in your favor. Collect $200", 200));
    communityChestDeck->addCard(new PaymentCard("Doctor's fee. Pay $50", -50));
    communityChestDeck->addCard(new PaymentCard("From sale of stock you get $50", 50));
    communityChestDeck->addCard(new PaymentCard("Holiday fund matures. Receive $100", 100));
    communityChestDeck->addCard(new PaymentCard("Income tax refund. Collect $20", 20));
    communityChestDeck->addCard(new PaymentCard("Life insurance matures. Collect $100", 100));
    communityChestDeck->addCard(new PaymentCard("Pay hospital fees of $100", -100));
    communityChestDeck->addCard(new PaymentCard("Pay school fees of $150", -150));
    communityChestDeck->addCard(new PaymentCard("Receive $25 consultancy fee", 25));
    communityChestDeck->addCard(new SpecialCard("Advance to GO", CardType::ADVANCE_TO_GO));
    communityChestDeck->addCard(new SpecialCard("Go to Jail", CardType::GO_TO_JAIL));
    communityChestDeck->addCard(new SpecialCard("Get Out of Jail Free", CardType::GET_OUT_OF_JAIL));
    communityChestDeck->addCard(new GroupPaymentCard("Grand Opera Night. Collect $50 from every player",
        50, &players));

    board->setCardDeck(chanceDeck, communityChestDeck);
}

void Monopoly::startGame() {
    std::cout << "\nWelcome to Monopoly!" << std::endl;
    std::cout << std::endl;

    initializePlayers();
    initializeDecks();

    std::cout << std::endl << "Game started with " << players.size() << " players!" << std::endl;
    std::cout << "Each player starts with $1500" << std::endl;
}

Vector<Property*> Monopoly::getPlayerProperties(Player* player) {
    Vector<Property*> playerProperties;

    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr && prop->getOwner() == player && prop->getPrice() > 0) {
            playerProperties.pushBack(prop);
        }
    }

    return playerProperties;
}

int Monopoly::calculatePlayerNetWorth(Player* player) {
    int netWorth = player->getBalance();

    Vector<Property*> properties = getPlayerProperties(player);
    for (size_t i = 0; i < properties.size(); i++) {
        Property* prop = properties[i];

        if (!prop->getIsMortgaged()) {
            netWorth += prop->getPrice() / 2;
        }

        if (prop->getColorGroup() != nullptr) {
            netWorth += prop->getHouseCount() * (prop->getColorGroup()->getHouseCost() / 2);
            netWorth += prop->getHotelCount() * (prop->getColorGroup()->getHotelCost() / 2);
        }
    }

    return netWorth;
}

void Monopoly::sellBuildings(Player* player) {
    Vector<Property*> properties = getPlayerProperties(player);
    bool hasBuildings = false;

    for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->getHouseCount() > 0 || properties[i]->getHotelCount() > 0) {
            hasBuildings = true;
            break;
        }
    }

    if (!hasBuildings) {
        std::cout << "You have no buildings to sell." << std::endl;
        return;
    }

    std::cout << "\nProperties with buildings:" << std::endl;
    for (size_t i = 0; i < properties.size(); i++) {
        Property* prop = properties[i];
        if (prop->getHouseCount() > 0 || prop->getHotelCount() > 0) {
            std::cout << i + 1 << ". " << prop->getName() << " - ";
            if (prop->getHotelCount() > 0) {
                std::cout << "1 Hotel (sell for $" << prop->getColorGroup()->getHotelCost() / 2 << ")";
            }
            else {
                std::cout << prop->getHouseCount() << " Houses (sell for $"
                    << prop->getColorGroup()->getHouseCost() / 2 << " each)";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "Select property (0 to cancel): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice > 0 && choice <= properties.size()) {
        Property* selectedProp = properties[choice - 1];
        if (selectedProp->getHotelCount() > 0) {
            selectedProp->sellHotel();
        }
        else if (selectedProp->getHouseCount() > 0) {
            std::cout << "How many houses to sell? (1-" << selectedProp->getHouseCount() << "): ";
            int housesToSell;
            std::cin >> housesToSell;
            std::cin.ignore();

            if (housesToSell > 0 && housesToSell <= selectedProp->getHouseCount()) {
                for (int i = 0; i < housesToSell; i++) {
                    selectedProp->sellHouse();
                }
            }
        }
    }
}

void Monopoly::mortgageProperty(Player* player) {
    Vector<Property*> properties = getPlayerProperties(player);
    Vector<Property*> mortgageable;

    for (size_t i = 0; i < properties.size(); i++) {
        if (!properties[i]->getIsMortgaged() &&
            properties[i]->getHouseCount() == 0 &&
            properties[i]->getHotelCount() == 0) {
            mortgageable.pushBack(properties[i]);
        }
    }

    if (mortgageable.empty()) {
        std::cout << "No properties available to mortgage." << std::endl;
        return;
    }

    std::cout << "\nProperties available to mortgage:" << std::endl;
    for (size_t i = 0; i < mortgageable.size(); i++) {
        std::cout << i + 1 << ". " << mortgageable[i]->getName()
            << " - Mortgage value: $" << mortgageable[i]->getPrice() / 2 << std::endl;
    }

    std::cout << "Select property to mortgage (0 to cancel): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice > 0 && choice <= mortgageable.size()) {
        mortgageable[choice - 1]->mortgageProperty();
    }
}

void Monopoly::unmortgageProperty(Player* player) {
    Vector<Property*> properties = getPlayerProperties(player);
    Vector<Property*> mortgaged;

    for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->getIsMortgaged()) {
            mortgaged.pushBack(properties[i]);
        }
    }

    if (mortgaged.empty()) {
        std::cout << "No mortgaged properties." << std::endl;
        return;
    }

    std::cout << "\nMortgaged properties:" << std::endl;
    for (size_t i = 0; i < mortgaged.size(); i++) {
        int unmortgageCost = (int)((mortgaged[i]->getPrice() / 2) * 1.1);
        std::cout << i + 1 << ". " << mortgaged[i]->getName()
            << " - Unmortgage cost: $" << unmortgageCost << std::endl;
    }

    std::cout << "Select property to unmortgage (0 to cancel): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice > 0 && choice <= mortgaged.size()) {
        mortgaged[choice - 1]->unmortgageProperty();
    }
}

void Monopoly::offerPropertyManagement(Player* player) {
    while (true) {
        std::cout << "\nProperty Management Options:" << std::endl;
        std::cout << "Current balance: $" << player->getBalance() << std::endl;
        std::cout << "1. Sell buildings" << std::endl;
        std::cout << "2. Mortgage property" << std::endl;
        std::cout << "3. Unmortgage property" << std::endl;
        std::cout << "0. Done" << std::endl;
        std::cout << "Choice: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            sellBuildings(player);
            break;
        case 2:
            mortgageProperty(player);
            break;
        case 3:
            unmortgageProperty(player);
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

bool Monopoly::handlePlayerDebt(Player* debtor, int amountOwed, Player* creditor) {
    if (debtor->getBalance() >= amountOwed) {
        return true;
    }

    std::cout << "\n" << debtor->getName() << " owes $" << amountOwed
        << " but only has $" << debtor->getBalance() << "!" << std::endl;

    int netWorth = calculatePlayerNetWorth(debtor);
    std::cout << "Total net worth: $" << netWorth << std::endl;

    if (netWorth < amountOwed) {
        std::cout << debtor->getName() << " cannot raise enough money even by selling everything!" << std::endl;
        return false;
    }

    std::cout << "You must raise $" << (amountOwed - debtor->getBalance()) << " to pay this debt." << std::endl;

    while (debtor->getBalance() < amountOwed) {
        offerPropertyManagement(debtor);

        if (debtor->getBalance() >= amountOwed) {
            std::cout << "Debt paid successfully!" << std::endl;
            return true;
        }

        std::cout << "\nStill need $" << (amountOwed - debtor->getBalance()) << std::endl;
        std::cout << "Continue trying to raise money? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice != 'y' && choice != 'Y') {
            return false;
        }
    }

    return true;
}

void Monopoly::handlePayment(Player* payer, Player* receiver, int amount, const MyString& reason) {
    std::cout << payer->getName() << " must pay $" << amount;
    if (receiver != nullptr) {
        std::cout << " to " << receiver->getName();
    }
    std::cout << " for " << reason << std::endl;

    if (payer->canAfford(amount)) {
        payer->payMoney(amount);
        if (receiver != nullptr) {
            receiver->receiveMoney(amount);
        }
        else {
            bank->collectPayment(amount);
        }
    }
    else {
        std::cout << payer->getName() << " cannot afford this payment!" << std::endl;

        if (handlePlayerDebt(payer, amount, receiver)) {
            payer->payMoney(amount);
            if (receiver != nullptr) {
                receiver->receiveMoney(amount);
            }
            else {
                bank->collectPayment(amount);
            }
            std::cout << "Payment made after managing properties." << std::endl;
        }
        else {
            std::cout << payer->getName() << " is BANKRUPT!" << std::endl;

            int remainingMoney = payer->getBalance();
            if (remainingMoney > 0) {
                payer->payMoney(remainingMoney);
                if (receiver != nullptr) {
                    receiver->receiveMoney(remainingMoney);
                }
                else {
                    bank->collectPayment(remainingMoney);
                }
            }

            if (receiver != nullptr) {
                Vector<Property*> bankruptProperties = getPlayerProperties(payer);
                for (size_t i = 0; i < bankruptProperties.size(); i++) {
                    bankruptProperties[i]->removeAllBuildings();
                    bankruptProperties[i]->setOwner(receiver);
                    if (bankruptProperties[i]->getIsMortgaged()) {
                        std::cout << receiver->getName() << " receives "
                            << bankruptProperties[i]->getName() << " (mortgaged)" << std::endl;
                    }
                    else {
                        std::cout << receiver->getName() << " receives "
                            << bankruptProperties[i]->getName() << std::endl;
                    }
                }
            }
            else {
                Vector<Property*> allProperties;
                for (int i = 0; i < board->getSize(); i++) {
                    Property* prop = dynamic_cast<Property*>(board->getField(i));
                    if (prop != nullptr) {
                        allProperties.pushBack(prop);
                    }
                }
                bank->handleBankruptcy(payer, allProperties);
            }

            payer->payMoney(payer->getBalance() + 1);
        }
    }
}

bool Monopoly::rollDice(Player& player) {
    int die1 = (rand() % 6) + 1;
    int die2 = (rand() % 6) + 1;
    int total = die1 + die2;
    bool isDouble = (die1 == die2);

    std::cout << player.getName() << " rolls " << die1 << " and " << die2
        << " (Total: " << total << ")";

    if (isDouble) {
        std::cout << " - DOUBLES!";
    }
    std::cout << std::endl;

    player.moveForward(total);

    int newPosition = player.getPosition();
    Field* field = board->getField(newPosition);

    std::cout << player.getName() << " lands on " << field->getName() << std::endl;

    int positionBeforeField = player.getPosition();

    Property* prop = dynamic_cast<Property*>(field);
    if (prop != nullptr) {
        MyString fieldName = prop->getName();

        if (fieldName == "Income Tax") {
            handlePayment(&player, nullptr, 200, "income tax");
        }
        else if (fieldName == "Luxury Tax") {
            handlePayment(&player, nullptr, 100, "luxury tax");
        }
        // Handle rent payments
        else if (prop->isOwned() && prop->getOwner() != &player && !prop->getIsMortgaged()) {
            if (!prop->getOwner()->isInJail()) {
                int rentAmount = 0;

                if (fieldName == "Electric Company" || fieldName == "Water Works") {
                    rentAmount = total * prop->getRent();
                    handlePayment(&player, prop->getOwner(), rentAmount, "utility rent");
                }
                else {
                    rentAmount = prop->getRent();
                    handlePayment(&player, prop->getOwner(), rentAmount, "rent");
                }
            }
            else {
                std::cout << prop->getOwner()->getName() << " is in jail and cannot collect rent." << std::endl;
            }
        }
        else {
            field->onPlayerLanding(player, total);
        }
    }
    else {
        field->onPlayerLanding(player, total);
    }

    if (player.getPosition() != positionBeforeField && !player.isInJail()) {
        Field* newField = board->getField(player.getPosition());
        std::cout << player.getName() << " lands on " << newField->getName() << std::endl;
        newField->onPlayerLanding(player, total);
    }

    Property* utilityCheck = dynamic_cast<Property*>(field);
    if (utilityCheck != nullptr) {
        MyString fieldName = utilityCheck->getName();
        if (fieldName == "Electric Company" || fieldName == "Water Works") {
            if (utilityCheck->getOwner() == &player) {
                updateUtilityRents();
            }
        }
    }

    return isDouble;
}

void Monopoly::playTurn() {
    Player* currentPlayer = players[currentPlayerIndex];
    doublesCount = 0;
    bool continueRolling = true;

    while (continueRolling && !currentPlayer->isBankrupt()) {
        std::cout << "\n" << currentPlayer->getName() << "'s turn";
        if (doublesCount > 0) {
            std::cout << " (Double #" << doublesCount << ")";
        }
        std::cout << ":" << std::endl;
        std::cout << "Current balance: $" << currentPlayer->getBalance() << std::endl;
        std::cout << "Current position: " << currentPlayer->getPosition() << std::endl;

        if (currentPlayer->isInJail()) {
            std::cout << currentPlayer->getName() << " is in jail!" << std::endl;
            int diceTotal = currentPlayer->tryToGetOutOfJail();

            if (diceTotal > 0) {
                Field* field = board->getField(currentPlayer->getPosition());
                std::cout << currentPlayer->getName() << " lands on " << field->getName() << std::endl;
                field->onPlayerLanding(*currentPlayer, diceTotal);
            }

            continueRolling = false;
        }
        else {
            bool rolledDoubles = rollDice(*currentPlayer);

            if (rolledDoubles) {
                doublesCount++;

                if (doublesCount >= 3) {
                    std::cout << currentPlayer->getName()
                        << " rolled 3 doubles in a row and goes to JAIL!" << std::endl;
                    currentPlayer->goToJail();
                    continueRolling = false;
                }
                else {
                    std::cout << currentPlayer->getName()
                        << " gets another turn for rolling doubles!" << std::endl;

                    if (currentPlayer->isInJail()) {
                        continueRolling = false;
                    }
                }
            }
            else {
                continueRolling = false;
            }
        }

        if (currentPlayer->isBankrupt()) {
            std::cout << currentPlayer->getName() << " is bankrupt and out of the game!" << std::endl;

            Vector<Property*> allProperties;
            for (int i = 0; i < board->getSize(); i++) {
                Property* prop = dynamic_cast<Property*>(board->getField(i));
                if (prop != nullptr) {
                    allProperties.pushBack(prop);
                }
            }

            bank->handleBankruptcy(currentPlayer, allProperties);
            continueRolling = false;
        }
    }
}

void Monopoly::checkWinCondition() {
    int activePlayers = 0;
    Player* lastPlayer = nullptr;

    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i]->isBankrupt()) {
            activePlayers++;
            lastPlayer = players[i];
        }
    }

    if (activePlayers == 1) {
        std::cout << "\nGAME OVER" << std::endl;
        std::cout << lastPlayer->getName() << " wins with $"
            << lastPlayer->getBalance() << "!" << std::endl;
        gameEnded = true;
    }
}

void Monopoly::displayGameState() {
    std::cout << "\nGame status:" << std::endl;

    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i]->isBankrupt()) {
            std::cout << "\n" << players[i]->getName() << ":" << std::endl;
            std::cout << "  Balance: $" << players[i]->getBalance();
            std::cout << " | Position: " << players[i]->getPosition();

            Field* currentField = board->getField(players[i]->getPosition());
            if (currentField != nullptr) {
                std::cout << " (" << currentField->getName() << ")";
            }

            if (players[i]->isInJail()) {
                std::cout << " [IN JAIL]";
            }
            if (players[i]->getGetOutOfJailCards() > 0) {
                std::cout << " [" << players[i]->getGetOutOfJailCards() << " Get Out of Jail card(s)]";
            }
            std::cout << std::endl;

            displayPlayerProperties(players[i]);
        }
    }

}

void Monopoly::displayPlayerProperties(Player* player) {
    std::cout << "  Properties:" << std::endl;

    bool hasAnyProperty = false;

    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr && prop->getOwner() == player && prop->getPrice() > 0) {
            if (!hasAnyProperty) {
                std::cout << "    ";
            }
            else {
                std::cout << ", ";
            }

            std::cout << prop->getName();

            if (prop->getHouseCount() > 0) {
                std::cout << " [" << prop->getHouseCount() << "H]";
            }
            if (prop->getHotelCount() > 0) {
                std::cout << " [HOTEL]";
            }
            if (prop->getIsMortgaged()) {
                std::cout << " [MORTGAGED]";
            }

            hasAnyProperty = true;
        }
    }

    if (!hasAnyProperty) {
        std::cout << "    None";
    }

    std::cout << std::endl;

    checkAndDisplayMonopolies(player);
}

void Monopoly::checkAndDisplayMonopolies(Player* player) {
    std::cout << "  Monopolies: ";

    bool hasMonopoly = false;
    MyString monopolyNames;

    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr && prop->getOwner() == player && prop->hasMonopoly()) {
            ColorGroup* group = prop->getColorGroup();
            if (group != nullptr) {
                MyString groupName = group->getName();
                if (monopolyNames.c_str()[0] == '\0' ||
                    strstr(monopolyNames.c_str(), groupName.c_str()) == nullptr) {
                    if (hasMonopoly) {
                        std::cout << ", ";
                    }
                    std::cout << groupName;
                    monopolyNames = monopolyNames + " " + groupName;
                    hasMonopoly = true;
                }
            }
        }
    }

    if (!hasMonopoly) {
        std::cout << "None";
    }

    std::cout << std::endl;
}

void Monopoly::updateUtilityRents() {
    Property* electric = nullptr;
    Property* water = nullptr;

    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr) {
            if (prop->getName() == "Electric Company") {
                electric = prop;
            }
            else if (prop->getName() == "Water Works") {
                water = prop;
            }
        }
    }

    if (electric != nullptr && water != nullptr) {
        if (electric->getOwner() == water->getOwner() && electric->isOwned()) {
            electric->setRent(10);
            water->setRent(10);
        }
        else {
            if (electric->isOwned()) electric->setRent(4);
            if (water->isOwned()) water->setRent(4);
        }
    }
}

void Monopoly::mainGameLoop() {
    while (!gameEnded) {
        int startingPlayer = currentPlayerIndex;

        do {
            playTurn();
            checkWinCondition();

            if (!gameEnded) {
                do {
                    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
                } while (players[currentPlayerIndex]->isBankrupt());
            }

        } while (!gameEnded && currentPlayerIndex != startingPlayer);

        if (!gameEnded) {
            displayGameState();

            std::cout << "\nPress Enter to continue";
            std::cin.get();
        }
    }
}

void Monopoly::saveGame(const MyString& filename) {
    std::ofstream file(filename.c_str());

    if (!file.is_open()) {
        std::cout << "Could not save game!" << std::endl;
        return;
    }

    file << players.size() << std::endl;

    for (size_t i = 0; i < players.size(); i++) {
        file << players[i]->getName() << " "
            << players[i]->getBalance() << " "
            << players[i]->getPosition() << " "
            << players[i]->isInJail() << std::endl;
    }

    file << currentPlayerIndex << std::endl;

    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr && prop->isOwned()) {
            for (size_t j = 0; j < players.size(); j++) {
                if (players[j] == prop->getOwner()) {
                    file << i << " " << j << std::endl;
                    break;
                }
            }
        }
    }

    file.close();
    std::cout << "Game saved successfully!" << std::endl;
}

void Monopoly::loadGame(const MyString& filename) {
    std::ifstream file(filename.c_str());

    if (!file.is_open()) {
        std::cout << "Could not load game!" << std::endl;
        return;
    }

    for (size_t i = 0; i < players.size(); i++) {
        delete players[i];
    }
    players.clear();

    int numPlayers;
    file >> numPlayers;

    for (int i = 0; i < numPlayers; i++) {
        char nameBuffer[100];
        int balance, position;
        bool inJail;

        file >> nameBuffer >> balance >> position >> inJail;

        Player* player = new Player(MyString(nameBuffer), balance);
        player->moveTo(position);
        if (inJail) {
            player->goToJail();
        }

        players.pushBack(player);
    }

    file >> currentPlayerIndex;

    int fieldIndex, ownerIndex;
    while (file >> fieldIndex >> ownerIndex) {
        Property* prop = dynamic_cast<Property*>(board->getField(fieldIndex));
        if (prop != nullptr && ownerIndex >= 0 && ownerIndex < players.size()) {
            prop->setOwner(players[ownerIndex]);
        }
    }

    initializeDecks();
    updateUtilityRents();

    file.close();
    std::cout << "Game loaded successfully!" << std::endl;
}