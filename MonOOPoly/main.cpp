#include "Monopoly.h"
#include "Bank.h"
#include <iostream>

int main() {
    Monopoly game;

    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "Choose option: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 2) {
        char filename[256];
        std::cout << "Enter filename to load: ";
        std::cin.getline(filename, 255);
        game.loadGame(MyString(filename));
    }
    else {
        game.startGame();
    }

    game.mainGameLoop();

    std::cout << "\nWould you like to save the game? (y/n): ";
    char saveChoice;
    std::cin >> saveChoice;
    std::cin.ignore();

    if (saveChoice == 'y' || saveChoice == 'Y') {
        char filename[256];
        std::cout << "Enter filename to save: ";
        std::cin.getline(filename, 255);
        game.saveGame(MyString(filename));
    }

    std::cout << "\nThank you for playing Monopoly!" << std::endl;

    Bank::cleanup();

    return 0;
}