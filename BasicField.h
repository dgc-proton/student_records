#pragma once
#include "Definitions.h"

class BasicField
{
protected:
public:
    BasicField() {}

    // Copy constructor only needed if member objects are added to basicField and require copying
    //BasicField(const BasicField &other) {}

    virtual ~BasicField() {}

    virtual string printFieldType() const = 0;

    // Pure virtual function for comparing two BasicField objects
    virtual bool compare(const BasicField &field_1, ComparisonType &type) const = 0;

    // Pure virtual function for generating random value for the field
    virtual bool generateRandom() = 0;
};
