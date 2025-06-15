#include "GroupPaymentCard.h"
#include "Player.h"
#include <iostream>

GroupPaymentCard::GroupPaymentCard(const MyString& desc, int amount, Vector<Player*>* players)
    : Card(desc), amountPerPlayer(amount), allPlayers(players) {}

void GroupPaymentCard::applyEffect(Player& player) {
    int totalAmount = 0;

    std::cout << player.getName() << " collects $" << amountPerPlayer
        << " from each other player." << std::endl;

    for (size_t i = 0; i < allPlayers->size(); i++) {
        Player* otherPlayer = (*allPlayers)[i];
        if (otherPlayer != &player && !otherPlayer->isBankrupt()) {
            otherPlayer->payMoney(amountPerPlayer);
            totalAmount += amountPerPlayer;
        }
    }

    player.receiveMoney(totalAmount);
}