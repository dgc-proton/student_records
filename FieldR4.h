#pragma once
#include "Definitions.h"
#include "BasicField.h"
#include "FieldR2.h"
#include <algorithm>
#include <cctype>

#define MAX_ENROLLMENT_YEAR 2024

class EnrollmentYearField : public BasicField
{
protected:
    int enrollment_year; // Enrollment year value
    int CGS;             // Common Grade Scale value for the year
    bool EnrollmentYearFieldInitFlag;
    bool CGSInitFlag;

    void setEnrollmentYearFieldInitFlag()
    {
        EnrollmentYearFieldInitFlag = true;
    }

    void setCGSInitFlag()
    {
        CGSInitFlag = true;
    }

public:
    EnrollmentYearField() : EnrollmentYearFieldInitFlag(false), CGSInitFlag(false) {}

    EnrollmentYearField(const EnrollmentYearField &other) : BasicField(other)
    {
        if (other.getEnrollmentYearFieldInitFlag())
        {
            this->enrollment_year = other.enrollment_year;
            setEnrollmentYearFieldInitFlag();
        }
        if (other.getCGSInitFlag())
        {
            this->CGS = other.CGS;
            setCGSInitFlag();
        }
    }

    ~EnrollmentYearField() {}

    bool getEnrollmentYearFieldInitFlag() const
    {
        return EnrollmentYearFieldInitFlag;
    }

    bool getCGSInitFlag() const
    {
        return CGSInitFlag;
    }

    bool getEnrollmentYear(int &out) const
    {
        if (getEnrollmentYearFieldInitFlag())
        {
            out = this->enrollment_year;
            return true;
        }
        else
        {
            out = -1; // Meaningful value indicating uninitialized state
            return false;
        }
    }

    bool getCGS(int &out) const
    {
        if (getCGSInitFlag())
        {
            out = this->CGS;
            return true;
        }
        else
        {
            out = -1; // Meaningful value indicating uninitialized state
            return false;
        }
    }

    string printFieldType() const override
    {
        return convertRecordFieldToString(RecordFields::ENROLLMENT_YEAR);
    }

    bool setEnrollmentYear(int year, string &error_message)
    {
        if (year < 2000 || year > MAX_ENROLLMENT_YEAR)
        {
            error_message = "Invalid enrollment year. Year must be between 2000 and " + std::to_string(MAX_ENROLLMENT_YEAR) + ".";
            return false;
        }
        this->enrollment_year = year;
        setEnrollmentYearFieldInitFlag();
        return true;
    }

    bool setCGS(int cgs_value, string &error_message)
    {
        if (cgs_value < 0 || cgs_value > 22)
        {
            error_message = "Invalid CGS value. CGS must be between 0 and 22.";
            return false;
        }

        this->CGS = cgs_value;
        setCGSInitFlag();
        return true;
    }

    bool compare(__attribute__((unused)) const BasicField &field_1, __attribute__((unused)) ComparisonType &type) const override
    {
        cout << "Use compareEnrollmentYears() or compareCGS() Functions for Enrollment Year comparison." << endl;
        return true;
    }

    bool compareEnrollmentYears(const BasicField &field_1, ComparisonType &type) const
    {
        const EnrollmentYearField *enrollment_year_field1 = dynamic_cast<const EnrollmentYearField *>(&field_1);
        if (enrollment_year_field1 == nullptr || !enrollment_year_field1->getEnrollmentYearFieldInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!getEnrollmentYearFieldInitFlag())
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }

        else if (this->enrollment_year < enrollment_year_field1->enrollment_year)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (this->enrollment_year > enrollment_year_field1->enrollment_year)
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

    bool compareCGS(const BasicField &field_1, ComparisonType &type) const
    {
        const EnrollmentYearField *enrollment_year_field1 = dynamic_cast<const EnrollmentYearField *>(&field_1);
        if (enrollment_year_field1 == nullptr || !enrollment_year_field1->getCGSInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!getCGSInitFlag())
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }
        else if (this->CGS < enrollment_year_field1->CGS)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (this->CGS > enrollment_year_field1->CGS)
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
        bool success = generateRandomEnrollmentYear();
        success &= generateRandomCGS();
        return success;
    }

    bool generateRandomEnrollmentYear()
    {
        // Generate a random enrollment year between 2000 and before current year (2025)
        enrollment_year = 2000 + (rand() % (MAX_ENROLLMENT_YEAR - 2000 + 1)); // Random year between 2000 and 2024
        setEnrollmentYearFieldInitFlag();
        return true;
    }

    bool generateRandomCGS()
    {
        // Generate a random CGS value between 0 and 22
        CGS = rand() % 23; // Random CGS between 0 and 22
        setCGSInitFlag();
        return true;
    }

    bool partialMatchEnrollmentYear(const int &other) const
    {
        string a = to_string(this->enrollment_year);
        string b = to_string(other);

        if (a.empty() || b.empty())
            return false;

        // Check substring
        return substringMatch(a, b);
    }

    bool partialMatchCGS(const int &other) const
    {
        string a = to_string(this->CGS);
        string b = to_string(other);

        if (a.empty() || b.empty())
            return false;

        // Check substring
        return substringMatch(a, b);
    }
};
