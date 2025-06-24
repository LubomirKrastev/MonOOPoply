#include "SpecialCard.h"
#include "Player.h"
#include <iostream>

SpecialCard::SpecialCard(const MyString& desc, CardType cardType)
    : Card(desc), type(cardType) {}

void SpecialCard::applyEffect(Player& player) {
    switch (type) {
    case CardType::GET_OUT_OF_JAIL:
        std::cout << player.getName() << " received a 'Get Out of Jail Free' card!" << std::endl;
        player.addGetOutOfJailCard();
        break;

    case CardType::GO_TO_JAIL:
        std::cout << player.getName() << " must go directly to jail!" << std::endl;
        player.goToJail();
        break;

    case CardType::ADVANCE_TO_GO:
        std::cout << player.getName() << " advances to GO." << std::endl;
        {
            int currentPos = player.getPosition();
            if (currentPos != 0) {
                int stepsToGo = (40 - currentPos);
                player.moveForward(stepsToGo);
            }
        }
        break;

    case CardType::ADVANCE_TO_RAILROAD: {
        std::cout << player.getName() << " advances to the nearest Railroad." << std::endl;
        int pos = player.getPosition();
        int nearestRR = 0;

        if (pos < 5 || pos >= 35) nearestRR = 5;
        else if (pos < 15) nearestRR = 15;
        else if (pos < 25) nearestRR = 25;
        else nearestRR = 35;

        int steps = nearestRR - pos;
        if (steps < 0) steps += 40;
        player.moveForward(steps);
        break;
    }

    case CardType::ADVANCE_TO_UTILITY: {
        std::cout << player.getName() << " advances to the nearest Utility." << std::endl;
        int pos = player.getPosition();
        int nearestUtil = 0;

        if (pos < 12 || pos >= 28) nearestUtil = 12;
        else nearestUtil = 28;

        int steps = nearestUtil - pos;
        if (steps < 0) steps += 40;
        player.moveForward(steps);
        break;
    }

    case CardType::ADVANCE_TO_BOARDWALK:
        std::cout << player.getName() << " advances to Boardwalk." << std::endl;
        player.moveTo(39);
        break;

    case CardType::ADVANCE_TO_ILLINOIS:
        std::cout << player.getName() << " advances to Illinois Ave." << std::endl;
        {
            int pos = player.getPosition();
            if (pos > 24) {
                int steps = (40 - pos) + 24;
                player.moveForward(steps);
            }
            else {
                player.moveTo(24);
            }
        }
        break;

    case CardType::ADVANCE_TO_ST_CHARLES:
        std::cout << player.getName() << " advances to St. Charles Place." << std::endl;
        {
            int pos = player.getPosition();
            if (pos > 11) {
                int steps = (40 - pos) + 11;
                player.moveForward(steps);
            }
            else {
                player.moveTo(11);
            }
        }
        break;

    case CardType::GO_BACK_3_SPACES:
        std::cout << player.getName() << " goes back 3 spaces." << std::endl;
        {
            int newPos = player.getPosition() - 3;
            if (newPos < 0) newPos += 40;
            player.moveTo(newPos);
        }
        break;
    }
}