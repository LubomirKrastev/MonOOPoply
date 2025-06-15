#pragma once
#include "MyString.h"

class Player;

class Field {
protected:
    MyString name;

public:
    Field(const MyString& fieldName);
    virtual ~Field() = default;

    virtual void onPlayerLanding(Player& player) = 0;
    const MyString& getName() const;
};