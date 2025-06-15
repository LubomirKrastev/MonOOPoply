#include "Mortgage.h"

Mortgage::Mortgage(double mult) : multiplier(mult) {}

int Mortgage::calculateNewRent(int baseRent) const {
    return static_cast<int>(baseRent * multiplier);
}

Cottage::Cottage() : Mortgage(1.15) {}

Castle::Castle() : Mortgage(1.50) {}