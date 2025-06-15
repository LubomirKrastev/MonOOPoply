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

    void initializePlayers();
    void initializeDecks();
    void playTurn();
    void rollDice(Player& player);
    void checkWinCondition();
    void displayGameState();

public:
    Monopoly();
    ~Monopoly();

    void startGame();
    void mainGameLoop();

    // Save and load functionality
    void saveGame(const MyString& filename);
    void loadGame(const MyString& filename);

    Monopoly(const Monopoly&) = delete;
    Monopoly& operator=(const Monopoly&) = delete;
};