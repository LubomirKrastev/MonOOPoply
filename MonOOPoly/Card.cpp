#include "Card.h"

Card::Card(const MyString& desc) : description(desc) {}

const MyString& Card::getDescription() const {
    return description;
}