#pragma once
#include "Vector.hpp"
#include "Board.h"
#include "CardDeck.h"
#include "MyString.h"

class Player;
class Bank;
class Property;

class Monopoly {
private:
    Vector<Player*> players;
    Board* board;
    Bank* bank;
    CardDeck* chanceDeck;
    CardDeck* communityChestDeck;
    int currentPlayerIndex;
    bool gameEnded;
    int doublesCount;

    void initializePlayers();
    void initializeDecks();
    void playTurn();
    bool rollDice(Player& player);
    void checkWinCondition();
    void displayGameState();
    void displayPlayerProperties(Player* player);
    void checkAndDisplayMonopolies(Player* player);
    void updateUtilityRents();

    bool handlePlayerDebt(Player* debtor, int amountOwed, Player* creditor = nullptr);
    void offerPropertyManagement(Player* player);
    void mortgageProperty(Player* player);
    void unmortgageProperty(Player* player);
    void sellBuildings(Player* player);
    Vector<Property*> getPlayerProperties(Player* player);
    int calculatePlayerNetWorth(Player* player);
    void handlePayment(Player* payer, Player* receiver, int amount, const MyString& reason);

public:
    Monopoly();
    ~Monopoly();

    void startGame();
    void mainGameLoop();

    void saveGame(const MyString& filename);
    void loadGame(const MyString& filename);

    Monopoly(const Monopoly&) = delete;
    Monopoly& operator=(const Monopoly&) = delete;
};