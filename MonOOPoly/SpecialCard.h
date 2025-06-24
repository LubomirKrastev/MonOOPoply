#pragma once
#include "Card.h"

enum class CardType {
    GET_OUT_OF_JAIL,
    GO_TO_JAIL,
    ADVANCE_TO_GO,
    ADVANCE_TO_RAILROAD,
    ADVANCE_TO_UTILITY,
    ADVANCE_TO_BOARDWALK,
    ADVANCE_TO_ILLINOIS,
    ADVANCE_TO_ST_CHARLES,
    GO_BACK_3_SPACES,
};

class SpecialCard : public Card {
private:
    CardType type;

public:
    SpecialCard(const MyString& desc, CardType cardType);
    void applyEffect(Player& player) override;
};