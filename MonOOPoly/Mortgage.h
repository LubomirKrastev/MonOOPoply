#pragma once
#include "Property.h"

class Mortgage {
protected:
    double multiplier;

public:
    Mortgage(double mult);
    virtual ~Mortgage() = default;

    virtual int calculateNewRent(int baseRent) const;
    double getMultiplier() const;
};

class Cottage : public Mortgage {
public:
    Cottage();
};

class Castle : public Mortgage {
public:
    Castle();
};