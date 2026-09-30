#pragma once
#include "Definitions.h"
#include "BasicField.h"

class IntegerWithLimits : public BasicField
{
protected:
    int min_value; // Minimum allowable value
    int max_value; // Maximum allowable value
    int value;     // Current value
    bool initFlag;

    void setInitFlag() {
        initFlag = true;
    }

public:
    IntegerWithLimits()
    {
        min_value = -50; // Default minimum value
        max_value = 50;  // Default maximum value
        initFlag = false;
        /*
        generateRandom(); // Initialize with a random value within the default range
        */
    }

    IntegerWithLimits(const IntegerWithLimits &other) : BasicField(other), initFlag(other.initFlag)
    {
        this->min_value = other.min_value;
        this->max_value = other.max_value;
        if (other.getInitFlag())
        {
            this->value = other.value;
        }
    }

    ~IntegerWithLimits() {}

    bool getMinValue(int &out) const
    {
        if (getInitFlag())
        {
            out = this->min_value;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool getMaxValue(int &out) const
    {
        if (getInitFlag())
        {
            out = this->max_value;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool getValue(int &out) const
    {
        if (getInitFlag())
        {
            out = this->value;
            return true;
        }
        else
        {
            return false;
        }
    }

    string printFieldType() const override
    {
        return convertRecordFieldToString(RecordFields::INT_W_LIMITS);
    }

    // Set both minimum and maximum values with validation and return error message if invalid
    bool setMinAndMaxValues(int min_val, int max_val, string &errorMessage)
    {
        if (min_val > max_val)
        {
            errorMessage = "Invalid range. Minimum value cannot be greater than maximum value.";
            return false;
        }
        this->min_value = min_val;
        this->max_value = max_val;
        return true;
    }

    // Set the current value with validation and return error message if invalid
    bool setValue(int val, string &error_message)
    {
        if (val < min_value || val > max_value)
        {
            error_message = "Value out of bounds.";
            return false;
        }
        value = val;
        setInitFlag();
        return true;
    }

    bool compare(const BasicField &field_1, ComparisonType &type) const override
    {
        // Use dynamic_cast to safely check if both objects are IntegerWithLimits
        const IntegerWithLimits *int_field1 = dynamic_cast<const IntegerWithLimits *>(&field_1);

        // If either cast failed, the objects are not of the same type (IntegerWithLimits)
        if (int_field1 == nullptr || !int_field1->getInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        } 
        else if (!getInitFlag()) 
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }
        else if (value < int_field1->value)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (value > int_field1->value)
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }
        else
        {
            type = ComparisonType::EQUAL_TO;
            return true;
        }

        return false;
    }

    bool generateRandom() override
    {
        int random_value = rand() % (max_value - min_value + 1) + min_value; // Generate random value within limits
        string error_message;
        setValue(random_value, error_message);
        setInitFlag();
        return true;
    }

    bool getInitFlag() const {
        return initFlag;
    }
};