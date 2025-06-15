#include "Monopoly.h"
#include "Player.h"
#include "Bank.h"
#include "Property.h"
#include "MovePositionCard.h"
#include "PaymentCard.h"
#include "GroupPaymentCard.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

Monopoly::Monopoly() : currentPlayerIndex(0), gameEnded(false) {
    srand(static_cast<unsigned>(time(nullptr)));

    board = new Board();
    bank = Bank::getInstance();
    chanceDeck = new CardDeck();
    communityChestDeck = new CardDeck();

    initializeDecks();
}

Monopoly::~Monopoly() {
    for (size_t i = 0; i < players.size(); i++) {
        delete players[i];
    }
    delete board;
    delete chanceDeck;
    delete communityChestDeck;
}

void Monopoly::initializePlayers() {
    int numPlayers;
    std::cout << "Enter number of players (2-6): ";
    std::cin >> numPlayers;

    if (numPlayers < 2) numPlayers = 2;
    if (numPlayers > 6) numPlayers = 6;

    for (int i = 0; i < numPlayers; i++) {
        char nameBuffer[50];
        std::cout << "Enter name for Player " << (i + 1) << ": ";
        std::cin >> nameBuffer;

        Player* newPlayer = new Player(MyString(nameBuffer));
        players.pushBack(newPlayer);
    }

    bank->distributeStartingMoney(players);
}

void Monopoly::initializeDecks() {
    // Chance cards
    chanceDeck->addCard(new MovePositionCard("Advance to GO", 40));
    chanceDeck->addCard(new MovePositionCard("Go back 3 spaces", -3));
    chanceDeck->addCard(new PaymentCard("Bank pays you dividend of $50", 50));
    chanceDeck->addCard(new PaymentCard("Pay poor tax of $15", -15));

    // Community Chest cards
    communityChestDeck->addCard(new PaymentCard("Bank error in your favor. Collect $200", 200));
    communityChestDeck->addCard(new PaymentCard("Doctor's fee. Pay $50", -50));
    communityChestDeck->addCard(new PaymentCard("From sale of stock you get $50", 50));
    communityChestDeck->addCard(new GroupPaymentCard("Grand Opera Night. Collect $50 from every player", 50, &players));

    board->setCardDeck(chanceDeck, communityChestDeck);
}

void Monopoly::startGame() {
    std::cout << "\nWelcome to Monopoly!" << std::endl;
    std::cout << std::endl;

    initializePlayers();

    std::cout << std::endl << "Game started with " << players.size() << " players!" << std::endl;
    std::cout << "Each player starts with $1500" << std::endl << std::endl;
}

void Monopoly::rollDice(Player& player) {
    int die1 = (rand() % 6) + 1;
    int die2 = (rand() % 6) + 1;
    int total = die1 + die2;

    std::cout << player.getName() << " rolls " << die1 << " and " << die2
        << " (Total: " << total << ")" << std::endl;

    player.moveForward(total);

    int newPosition = player.getPosition();
    Field* field = board->getField(newPosition);

    std::cout << player.getName() << " lands on " << field->getName() << std::endl;
    field->onPlayerLanding(player);
}

void Monopoly::playTurn() {
    Player* currentPlayer = players[currentPlayerIndex];

    std::cout << "\n--- " << currentPlayer->getName() << "'s Turn ---" << std::endl;
    std::cout << "Current balance: $" << currentPlayer->getBalance() << std::endl;
    std::cout << "Current position: " << currentPlayer->getPosition() << std::endl;

    if (currentPlayer->isInJail()) {
        std::cout << currentPlayer->getName() << " is in jail!" << std::endl;
        if (currentPlayer->tryToGetOutOfJail()) {
            rollDice(*currentPlayer);
        }
    }
    else {
        rollDice(*currentPlayer);
    }

    // Check for bankruptcy
    if (currentPlayer->isBankrupt()) {
        std::cout << currentPlayer->getName() << " is bankrupt and out of the game!" << std::endl;

        // Find all properties owned by this player
        Vector<Property*> allProperties;
        for (int i = 0; i < board->getSize(); i++) {
            Property* prop = dynamic_cast<Property*>(board->getField(i));
            if (prop != nullptr) {
                allProperties.pushBack(prop);
            }
        }

        bank->handleBankruptcy(currentPlayer, allProperties);
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
        std::cout << "\n\n*** GAME OVER ***" << std::endl;
        std::cout << lastPlayer->getName() << " wins with $"
            << lastPlayer->getBalance() << "!" << std::endl;
        gameEnded = true;
    }
}

void Monopoly::displayGameState() {
    std::cout << "\nCurrent Game State" << std::endl;
    for (size_t i = 0; i < players.size(); i++) {
        if (!players[i]->isBankrupt()) {
            std::cout << players[i]->getName() << ": $" << players[i]->getBalance()
                << " (Position: " << players[i]->getPosition() << ")" << std::endl;
        }
    }
}

void Monopoly::mainGameLoop() {
    while (!gameEnded) {
        playTurn();
        checkWinCondition();

        if (!gameEnded) {
            displayGameState();

            // Move to next player
            do {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            } while (players[currentPlayerIndex]->isBankrupt());

            std::cout << "\nPress Enter to continue to next turn...";
            std::cin.ignore();
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

    // Save number of players
    file << players.size() << std::endl;

    // Save player data
    for (size_t i = 0; i < players.size(); i++) {
        file << players[i]->getName() << " "
            << players[i]->getBalance() << " "
            << players[i]->getPosition() << " "
            << players[i]->isInJail() << std::endl;
    }

    // Save current player index
    file << currentPlayerIndex << std::endl;

    // Save property ownership
    for (int i = 0; i < board->getSize(); i++) {
        Property* prop = dynamic_cast<Property*>(board->getField(i));
        if (prop != nullptr && prop->isOwned()) {
            file << i << " ";
            // Find owner index
            for (size_t j = 0; j < players.size(); j++) {
                if (players[j] == prop->getOwner()) {
                    file << j << std::endl;
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

    // Clear existing players
    for (size_t i = 0; i < players.size(); i++) {
        delete players[i];
    }
    players.clear();

    // Load number of players
    int numPlayers;
    file >> numPlayers;

    // Load player data
    for (int i = 0; i < numPlayers; i++) {
        char nameBuffer[50];
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

    // Load current player index
    file >> currentPlayerIndex;

    // Load property ownership
    int fieldIndex, ownerIndex;
    while (file >> fieldIndex >> ownerIndex) {
        Property* prop = dynamic_cast<Property*>(board->getField(fieldIndex));
        if (prop != nullptr && ownerIndex >= 0 && ownerIndex < players.size()) {
            prop->setOwner(players[ownerIndex]);
        }
    }

    file.close();
    std::cout << "Game loaded successfully!" << std::endl;
}