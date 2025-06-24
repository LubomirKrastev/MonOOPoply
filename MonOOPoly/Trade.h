#pragma once
#include "Vector.hpp"

class Player;
class Property;

class Trade {
private:
    Player* player1;
    Player* player2;
    Vector<Property*> propertiesFrom1;
    Vector<Property*> propertiesFrom2;
    int moneyFrom1;
    int moneyFrom2;
    
    bool validateTrade();
    void transferProperties();
    void transferMoney();

public:
    Trade(Player* p1, Player* p2);

    void addPropertyFromPlayer1(Property* property);
    void addPropertyFromPlayer2(Property* property);
    void setMoneyFromPlayer1(int amount);
    void setMoneyFromPlayer2(int amount);

    bool executeTrade();
    void displayTradeDetails() const;
};
