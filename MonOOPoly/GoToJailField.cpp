#include "GoToJailField.h"
#include "Player.h"

GoToJailField::GoToJailField() : Field("Go to Jail") {}

void GoToJailField::onPlayerLanding(Player& player) {
    player.goToJail();
}