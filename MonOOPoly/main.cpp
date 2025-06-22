#include "Monopoly.h"
#include <iostream>

int main() {
    Monopoly game;

    /*std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "Choose option: ";

    int choice;
    std::cin >> choice;

    if (choice == 2) {
        char filename[100];
        std::cout << "Enter filename to load: ";
        std::cin >> filename;
        game.loadGame(MyString(filename));
    }
   */ //else {
        game.startGame();
   // }

    game.mainGameLoop();

    /*std::cout << "\nWould you like to save the game? (y/n): ";
    char saveChoice;
    std::cin >> saveChoice;

    if (saveChoice == 'y' || saveChoice == 'Y') {
        char filename[100];
        std::cout << "Enter filename to save: ";
        std::cin >> filename;
        game.saveGame(MyString(filename));
    }*/

    std::cout << std::endl<< "Thank you for playing Monopoly!" << std::endl;

    Bank::cleanup();

    return 0;
}