#include "Field.h"

Field::Field(const MyString& fieldName) : name(fieldName) {}

const MyString& Field::getName() const {
    return name;
}