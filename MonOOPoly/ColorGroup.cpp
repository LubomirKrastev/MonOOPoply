#include "ColorGroup.h"

ColorGroup::ColorGroup(Color c, const MyString& n, int house, int hotel)
    : color(c), name(n), houseCost(house), hotelCost(hotel) {}

void ColorGroup::addProperty(Property* prop) {
    properties.pushBack(prop);
}

bool ColorGroup::isMonopoly(Player* owner) const {
    if (owner == nullptr || properties.empty()) {
        return false;
    }

    for (size_t i = 0; i < properties.size(); i++) {
        if (properties[i]->getOwner() != owner) {
            return false;
        }
    }
    return true;
}

Color ColorGroup::getColor() const {
    return color;
}

const MyString& ColorGroup::getName() const {
    return name;
}

int ColorGroup::getHouseCost() const {
    return houseCost;
}

int ColorGroup::getHotelCost() const {
    return hotelCost;
}

const Vector<Property*>& ColorGroup::getProperties() const {
    return properties;
}