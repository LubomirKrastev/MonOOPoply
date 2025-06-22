#include "MovePositionCard.h"
#include "Player.h"
#include <iostream>

MovePositionCard::MovePositionCard(const MyString& desc, int moveSteps)
    : Card(desc), steps(moveSteps) {}

void MovePositionCard::applyEffect(Player& player) {
    if (description == "Advance to GO") {
        std::cout << player.getName() << " advances to GO." << std::endl;

        int currentPos = player.getPosition();
        if (currentPos != 0) {
            int stepsToGo = (40 - currentPos);
            player.moveForward(stepsToGo);
        }
    }
    else if (steps > 0) {
        std::cout << player.getName() << " moves forward " << steps << " spaces." << std::endl;
        player.moveForward(steps);
    }
    else {
        std::cout << player.getName() << " moves backward " << -steps << " spaces." << std::endl;
        int newPos = player.getPosition() + steps;
        if (newPos < 0) {
            newPos += 40;
        }
        player.moveTo(newPos);
    }
}