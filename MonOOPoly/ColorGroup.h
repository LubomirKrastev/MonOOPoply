#pragma once
#include "MyString.h"
#include "Vector.hpp"
#include "Player.h"
#include "Property.h"

class Property;

enum class Color {
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE,
    RAILROAD,
    UTILITY,
    NONE
};

class ColorGroup {
private:
    Color color;
    MyString name;
    Vector<Property*> properties;
    int houseCost;
    int hotelCost;

public:
    ColorGroup(Color c, const MyString& n, int house, int hotel);

    void addProperty(Property* prop);
    bool isMonopoly(Player* owner) const;
    Color getColor() const;
    const MyString& getName() const;
    int getHouseCost() const;
    int getHotelCost() const;
    const Vector<Property*>& getProperties() const;
};