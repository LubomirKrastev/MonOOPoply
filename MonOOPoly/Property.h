#pragma once
#include "Field.h"

class Property : public Field {
private:
    int price;
    int rent;
    Player* owner;

public:
    Property(const MyString& name, int propertyPrice, int propertyRent);

    void onPlayerLanding(Player& player) override;

    int getPrice() const;
    int getRent() const;
    Player* getOwner() const;
    void setOwner(Player* newOwner);
    bool isOwned() const;
};