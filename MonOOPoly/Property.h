#pragma once
#include "Field.h"

class Mortgage;
class ColorGroup;

class Property : public Field {
private:
    int price;
    int baseRent;
    int currentRent;
    Player* owner;
    Mortgage* mortgage;
    ColorGroup* colorGroup;
    int houseCount;
    int hotelCount;
    bool isMortgaged;

    int rentWith1House;
    int rentWith2Houses;
    int rentWith3Houses;
    int rentWith4Houses;
    int rentWithHotel;

public:
    Property(const MyString& name, int propertyPrice, int propertyRent);
    Property(const MyString& name, int propertyPrice, int propertyRent,
        int rent1H, int rent2H, int rent3H, int rent4H, int rentHotel);
    ~Property();

    void onPlayerLanding(Player& player) override;

    int getPrice() const;
    int getRent() const;
    int getBaseRent() const;
    Player* getOwner() const;
    void setOwner(Player* newOwner);
    bool isOwned() const;

    void setColorGroup(ColorGroup* group);
    ColorGroup* getColorGroup() const;

    bool canBuildHouse() const;
    bool canBuildHotel() const;
    void buildHouse();
    void buildHotel();
    int getHouseCount() const;
    int getHotelCount() const;

    void sellHouse();
    void sellHotel();
    void sellAllBuildings();

    void mortgageProperty();
    void unmortgageProperty();
    bool getIsMortgaged() const;

    void addMortgage(Mortgage* newMortgage);
    void removeMortgage();
    bool hasMortgage() const;
    Mortgage* getMortgage() const;

    void updateRent();
    bool hasMonopoly() const;

    void setRent(int newRent);  
    
    void removeAllBuildings();
};