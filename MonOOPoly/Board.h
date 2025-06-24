#pragma once
#include "Vector.hpp"
#include "Field.h"

class CardDeck;
class ColorGroup;

class Board {
private:
    Vector<Field*> fields;
    Vector<ColorGroup*> colorGroups;

public:
    Board();
    ~Board();

    Field* getField(int position) const;
    int getSize() const;

    void initializeBoard();
    void setCardDeck(CardDeck* chanceDeck, CardDeck* communityDeck);

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
};