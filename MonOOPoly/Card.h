#pragma once
#include "MyString.h"

class Player;

class Card {
protected:
    MyString description;

public:
    Card(const MyString& desc);
    virtual ~Card() = default;

    virtual void applyEffect(Player& player) = 0;
    const MyString& getDescription() const;
};