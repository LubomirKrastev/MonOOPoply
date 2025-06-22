#pragma once
#include "Field.h"
#include "Mortgage.h"

class Mortgage;

class Property : public Field {
private:
    int price;
    int baseRent;
    int currentRent;
    Player* owner;
    Mortgage* mortgage;

public:
    Property(const MyString& name, int propertyPrice, int propertyRent);
    ~Property();

    void onPlayerLanding(Player& player) override;

    int getPrice() const;
    int getRent() const;
    int getBaseRent() const;
    Player* getOwner() const;
    void setOwner(Player* newOwner);
    bool isOwned() const;

    void addMortgage(Mortgage* newMortgage);
    void removeMortgage();
    bool hasMortgage() const;
    void updateRent();
};
