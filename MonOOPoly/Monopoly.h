#pragma once
#include "Vector.hpp"
#include "Board.h"
#include "CardDeck.h"
#include "MyString.h"

class Player;
class Bank;

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