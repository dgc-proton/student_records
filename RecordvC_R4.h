#ifndef RECORD_VC_H
#define RECORD_VC_H

#include "Record.h"
#include "RecordvB_R2.h"
#include "FieldR4.h"
#include "limits"

#define NUM_YEARS 10

class RecordvC : public RecordvB
{
private:
    EnrollmentYearField years[NUM_YEARS];
    int firstYear;

public:
    // default constructor
    RecordvC()
    {
        firstYear = -1;
    }

    // copy constructor
    RecordvC(RecordvC &other) : RecordvB(other), firstYear(other.firstYear)
    {
        if (other.getAtleastOneYearInit())
        {
            for (int i = 0; i < NUM_YEARS; i++)
            {
                int temp;
                if (other.years[i].getEnrollmentYear(temp))
                {
                    string temp_message;
                    years[i].setEnrollmentYear(temp, temp_message);

                    if (other.years[i].getCGS(temp))
                    {
                        years[i].setCGS(temp, temp_message);
                    }
                }
            }
        }
    }

    // override inherited editRecord function
    bool editRecord(string &in_errorMessageHolder, const bool genRandom = false)
    {
        if (getLocked())
        {
            in_errorMessageHolder = "Record is Locked. Could not edit.";
            return false;
        }
        in_errorMessageHolder = "";

        if (genRandom)
        {
            int low = (int)RecordFields::RECORDvA_STOP + 1;
            int high = (int)RecordFields::RECORDvB_STOP + 1;

            int f = low + rand() % (high - low + 1);
            RecordFields field = (RecordFields)f;
            if (field >= RecordFields::RECORDvB_STOP)
            {
                field = (RecordFields)((int)field + 1);
            }
            return setRecordSingleField(field, in_errorMessageHolder, genRandom);
        }
        else
        {
            bool success = true;
            bool exitSet = false;
            RecordFields enum_field_holder;

            success = getFieldChoiceFromKeyboard(in_errorMessageHolder, enum_field_holder, exitSet);

            if (!success)
            {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";
                setRecordValidity();
                return false;
            }
            else if (exitSet)
            {
                return true;
            }
            else
            {
                return setRecordSingleField(enum_field_holder, in_errorMessageHolder, false);
            }
        }
    }

    bool matchesField(const RecordvC &other, const SortCriterion &criteria)
    {
        // get value of field and call partialMatchCompare with that value
        string match_str;
        switch (criteria.field)
        {
        case RecordFields::FIRST_NAME:
            other.student_name.getFirstName(match_str);
            break;
        case RecordFields::FAMILY_NAME:
            other.student_name.getLastName(match_str);
            break;
        case RecordFields::STUDENT_ID:
        {
            long int id;
            other.student_id.getStudentId(id);
            match_str = to_string(id);
            break;
        }
        case RecordFields::DEGREE:
        {
            DegreeProgramme prog;
            string error_msg;
            other.degree.getProgramme(prog);
            match_str = convertDegreeProgrammeToString(prog);
            break;
        }
        case RecordFields::DEGREE_TYPE:
        {
            DegreeType type;
            other.degree.getProgrammeType(type);
            match_str = convertDegreeTypeToString(type);
            break;
        }
        case RecordFields::ENROLLMENT_YEAR:
        {
            if (!other.getAtleastOneYearInit() || !this->getAtleastOneYearInit())
            {
                return false; // Can't match if no enrollment year initialized
            }
            return checkEnrollmentYearsExists(other);
        }
        case RecordFields::CGS:
        {
            if (!other.getAtleastOneYearInit() || !this->getAtleastOneYearInit())
            {
                return false; // Can't match if no CGS initialized
            }
            return checkCGSExists(other);
        }
        default:
            cout << "Invalid Choice" << endl;
            return false;
        }
        return partialMatchCompare(criteria, match_str);
    }

    bool matchesFieldRange(const RecordvC *min_record, const RecordvC *max_record, const SortCriterion &criteria)
    {
        switch (criteria.field)
        {
        case RecordFields::FIRST_NAME:
        {
            string min_str, max_str, compare_str;
            min_record->student_name.getFirstName(min_str);
            max_record->student_name.getFirstName(max_str);
            student_name.getFirstName(compare_str);

            // Convert to lowercase for case-insensitive comparison
            transform(min_str.begin(), min_str.end(), min_str.begin(), ::tolower);
            transform(max_str.begin(), max_str.end(), max_str.begin(), ::tolower);
            transform(compare_str.begin(), compare_str.end(), compare_str.begin(), ::tolower);

            return compare_str >= min_str && compare_str <= max_str;
        }
        case RecordFields::FAMILY_NAME:
        {
            string min_str, max_str, compare_str;
            min_record->student_name.getLastName(min_str);
            max_record->student_name.getLastName(max_str);
            student_name.getLastName(compare_str);

            // Convert to lowercase for case-insensitive comparison
            transform(min_str.begin(), min_str.end(), min_str.begin(), ::tolower);
            transform(max_str.begin(), max_str.end(), max_str.begin(), ::tolower);
            transform(compare_str.begin(), compare_str.end(), compare_str.begin(), ::tolower);

            return compare_str >= min_str && compare_str <= max_str;
        }
        case RecordFields::STUDENT_ID:
        {
            long int min_id, max_id, compare_id;
            min_record->student_id.getStudentId(min_id);
            max_record->student_id.getStudentId(max_id);
            student_id.getStudentId(compare_id);

            // Use numeric comparison, not string comparison
            return compare_id >= min_id && compare_id <= max_id;
        }
        case RecordFields::DEGREE:
        {
            DegreeProgramme min_prog, max_prog, compare_prog;
            min_record->degree.getProgramme(min_prog);
            max_record->degree.getProgramme(max_prog);
            degree.getProgramme(compare_prog);

            // Use enum comparison
            return compare_prog >= min_prog && compare_prog <= max_prog;
        }
        case RecordFields::DEGREE_TYPE:
        {
            DegreeType min_type, max_type, compare_type;
            min_record->degree.getProgrammeType(min_type);
            max_record->degree.getProgrammeType(max_type);
            degree.getProgrammeType(compare_type);

            // Use enum comparison
            return compare_type >= min_type && compare_type <= max_type;
        }
        case RecordFields::ENROLLMENT_YEAR:
        {
            // Check if all enrollment years in this record fall within the range [min_record, max_record]
            return checkEnrollmentYearsInRange(min_record, max_record);
        }
        case RecordFields::CGS:
        {
            float min_cgs = min_record->getAvgCGS();
            float max_cgs = max_record->getAvgCGS();
            float compare_cgs = getAvgCGS();

            // Handle case where CGS might be -1 (not available)
            if (compare_cgs == -1 || min_cgs == -1 || max_cgs == -1)
            {
                return false;
            }

            return compare_cgs >= min_cgs && compare_cgs <= max_cgs;
        }
        default:
            cout << "Invalid Choice" << endl;
            return false;
        }
    }

    // Prints CGS for each year
    bool printInfoDetailed() const
    {
        bool success = true;
        if (getRecordInvalid())
        {
            cout << "Record is Invalid" << endl;
            return false;
        }

        int firstYearIndex = findFirstYearIndex();
        RecordvB::printInfo();
        cout << "Years Enrolled: ";
        if (getAtleastOneYearInit())
        {
            cout << firstYear;
            for (int i = 0; i < NUM_YEARS; i++)
            {
                int year;
                if (i == firstYearIndex)
                {
                    continue;
                }
                else if (years[i].getEnrollmentYear(year))
                {
                    cout << ", " << year;
                }
            }
            cout << endl;
        }
        else
        {
            cout << "Not found" << endl;
        }

        int CGS;
        if (firstYearIndex == -1)
        {
            return true;
        }
        if (years[firstYearIndex].getCGS(CGS))
        {
            cout << "CGS for " << firstYear << ": " << CGS << endl;
        }

        for (int i = 0; i < NUM_YEARS; i++)
        {
            if (i == firstYearIndex)
            {
                continue;
            }

            int year;
            if (years[i].getEnrollmentYear(year))
            {
                if (years[i].getCGS(CGS))
                {
                    cout << "CGS for " << year << ": " << CGS << endl;
                }
            }
        }

        cout << "Average CGS: " << getAvgCGS() << endl;
        return success;
    }

    // prints average CGS
    bool printInfo() const
    {
        bool success = true;
        if (getRecordInvalid())
        {
            cout << "Record is Invalid" << endl;
            return false;
        }

        int firstYearIndex = findFirstYearIndex();
        RecordvB::printInfo();
        cout << "Years Enrolled: ";
        if (getAtleastOneYearInit())
        {
            cout << firstYear;
            for (int i = 0; i < NUM_YEARS; i++)
            {
                int year;
                if (i == firstYearIndex)
                {
                    continue;
                }
                else if (years[i].getEnrollmentYear(year))
                {
                    cout << ", " << year;
                }
            }
            cout << endl;
        }
        else
        {
            cout << "Not found" << endl;
        }

        float avgCGS = getAvgCGS();
        if (avgCGS != -1)
        {
            cout << "Average CGS: " << getAvgCGS() << endl;
        }
        return success;
    }

    // overriding function for user to choose a field
    bool getFieldChoiceFromKeyboard(string &in_errorMessageHolder, RecordFields &chosenField, bool &exitSet)
    {
        bool success = true;
        int field_holder;
        int failCount = 0;

        cout << "Choose a field. Type:" << endl
             << "1 for First Name" << endl
             << "2 for Family Name" << endl
             << "3 for Student ID" << endl
             << "4 for Degree" << endl
             << "5 for Degree Type" << endl
             << "6 for Enrollment Years" << endl
             << "7 for CGS" << endl
             << "EXIT to stop" << endl;

        success = stringInputToInt(field_holder, exitSet, in_errorMessageHolder);

        if (exitSet)
        {
            cout << "User Exit from keyboard" << endl;
        }
        else if (!success)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        while (!success && !exitSet)
        {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error: editRecord() failed more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to invalid inputs. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << "Choose a field. Type:" << endl
                     << "1 for First Name" << endl
                     << "2 for Family Name" << endl
                     << "3 for Student ID" << endl
                     << "4 for Degree" << endl
                     << "5 for Degree Type" << endl
                     << "6 for Enrollment Years" << endl
                     << "7 for CGS" << endl
                     << "EXIT to stop" << endl;
            }

            success = stringInputToInt(field_holder, exitSet, in_errorMessageHolder);

            if (exitSet)
            {
                cout << "User Exit from keyboard" << endl;
                break;
            }
            else if (!success)
            {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";
                continue;
            }
            failCount++;
        }

        if (success && !exitSet)
        {
            chosenField = intToField(field_holder);
            if (chosenField <= RecordFields::RECORDvA_STOP || chosenField >= RecordFields::RECORDvC_STOP || chosenField == RecordFields::RECORDvB_STOP)
            {
                cout << "Invalid Field choice. Try again" << endl;
                success = false;
                failCount = 0;
                while (!success)
                {
                    success = true;
                    if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
                    {
                        cout << "Unexpected Error: editRecord() failed more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to invalid inputs. If not, refer to devs." << endl;
                        success = false;
                        bailout();
                    }
                    else
                    {
                        cout << "Choose a field. Type:" << endl
                             << "1 for First Name" << endl
                             << "2 for Family Name" << endl
                             << "3 for Student ID" << endl
                             << "4 for Degree" << endl
                             << "5 for Degree Type" << endl
                             << "6 for Enrollment Years" << endl
                             << "7 for CGS" << endl
                             << "EXIT to stop" << endl;
                    }
                    success = stringInputToInt(field_holder, exitSet, in_errorMessageHolder);

                    if (exitSet)
                    {
                        cout << "User exit from keyboard" << endl;
                        break;
                    }
                    else if (!success)
                    {
                        cout << in_errorMessageHolder << endl;
                        in_errorMessageHolder = "";
                        continue;
                    }
                    else
                    {
                        chosenField = intToField(field_holder);
                    }

                    if (chosenField <= RecordFields::RECORDvA_STOP || chosenField >= RecordFields::RECORDvC_STOP || chosenField == RecordFields::RECORDvB_STOP)
                    {
                        cout << "Invalid Field choice. Try again" << endl;
                        success = false;
                        failCount++;
                    }
                }
            }
        }
        return success;
    }

    // override inherited function
    bool compareSingleField(const RecordvC *other, const SortCriterion &criteria, ComparisonType &compareResult) const
    {
        bool success = true;

        switch (criteria.field)
        {
        case RecordFields::FIRST_NAME:
            success &= student_name.compareFirstNames(other->student_name, compareResult);
            break;

        case RecordFields::FAMILY_NAME:
            success &= student_name.compareLastNames(other->student_name, compareResult);
            break;

        case RecordFields::STUDENT_ID:
            success &= student_id.compare(other->student_id, compareResult);
            break;

        case RecordFields::DEGREE:
            success &= degree.compareProgramme(other->degree, compareResult);
            break;

        case RecordFields::DEGREE_TYPE:
            success &= degree.compareProgrammeType(other->degree, compareResult);
            break;

        case RecordFields::ENROLLMENT_YEAR:
            success &= compareFirstYear(*other, compareResult);
            break;

        case RecordFields::CGS:
            success &= compareAvgCGS(*other, compareResult);
            break;

        default:
            success = false;
            throw std::runtime_error("RecordvB::compareSingleField() invalid field");
        }

        return success;
    }

    // override inherited function
    virtual bool compare(const BasicRecord *other,
                         const SortCriterion &criteria1,
                         const SortCriterion &criteria2,
                         const SortCriterion &criteria3) const
    {
        // Guarantee no swap if other record is null or invalid
        if (other == nullptr || other->getRecordInvalid())
        {
            return false;
        }

        // Guarantee swap if this record is invalid but other is valid
        if (!other->getRecordInvalid() && getRecordInvalid())
        {
            return true;
        }

        // Check each criterion field is in valid range
        auto checkField = [](RecordFields f) -> bool
        {
            if (f <= RecordFields::RECORDvA_STOP || f >= RecordFields::RECORDvC_STOP || f == RecordFields::RECORDvB_STOP)
                return false;
            return true;
        };

        if (!checkField(criteria1.field))
        {
            throw std::runtime_error("SortCriterion field out of bounds in RecordvB::compareMultiField()");
        }

        // Error and guarantee no swap if items are not of same type
        const RecordvC *typecasted = typecastItem(other, this);
        if (typecasted == nullptr)
        {
            throw std::runtime_error("Items are not of same type in RecordvB::compareMultiField()");
            return false;
        }

        bool success;
        ComparisonType cmp;

        // ---------- Criterion 1 ----------
        success = compareSingleField(typecasted, criteria1, cmp);
        if (!success)
        {
            throw std::runtime_error("Error in compareMultiField() for criteria1");
        }

        if (cmp != ComparisonType::EQUAL_TO)
        {
            // Decide using criterion 1
            if (criteria1.ascending)
            {
                // ascending: swap if this > other
                return (cmp == ComparisonType::GREATER_THAN);
            }
            else
            {
                // descending: swap if this < other
                return (cmp == ComparisonType::LOWER_THAN);
            }
        }

        // check if criteria 2 is specified. If not, return no swap because comparision 1 returned EQUAL_TO
        if (!checkField(criteria2.field))
        {
            return false;
        }

        // ---------- Criterion 2 ----------
        success = compareSingleField(typecasted, criteria2, cmp);
        if (!success)
        {
            throw std::runtime_error("Error in compareMultiField() for criteria2");
        }

        if (cmp != ComparisonType::EQUAL_TO)
        {
            if (criteria2.ascending)
            {
                return (cmp == ComparisonType::GREATER_THAN);
            }
            else
            {
                return (cmp == ComparisonType::LOWER_THAN);
            }
        }

        // check if criteria 3 is specified. If not, return no swap because comparision 1 & 2 returned EQUAL_TO
        if (!checkField(criteria3.field))
        {
            return false;
        }

        // ---------- Criterion 3 ----------
        success = compareSingleField(typecasted, criteria3, cmp);
        if (!success)
        {
            throw std::runtime_error("Error in compareMultiField() for criteria3");
        }

        if (cmp != ComparisonType::EQUAL_TO)
        {
            if (criteria3.ascending)
            {
                return (cmp == ComparisonType::GREATER_THAN);
            }
            else
            {
                return (cmp == ComparisonType::LOWER_THAN);
            }
        }

        // If we get here, all three criteria said "equal" → no swap
        return false;
    }

    // check compaitibility of other record with this using typecast
    bool compatibilityCheck(const BasicRecord *other) const
    {
        if (other == NULL)
        {
            return false;
        }
        else
        {
            const RecordvB *typecasted = typecastItem(other, this);
            if (typecasted == NULL)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

protected:
    // finds array index of the first year
    int findFirstYearIndex() const
    {
        for (int i = 0; i < NUM_YEARS; i++)
        {
            if (years[i].getEnrollmentYearFieldInitFlag())
            {
                int temp;
                if (years[i].getEnrollmentYear(temp))
                {
                    if (temp == firstYear)
                    {
                        return i;
                    }
                }
            }
        }

        return -1;
    }

    // compares first enrollment year
    bool compareFirstYear(const RecordvC &other, ComparisonType &compareResult) const
    {
        if (!getAtleastOneYearInit() && !other.getAtleastOneYearInit())
        {
            compareResult = ComparisonType::EQUAL_TO;
            return true;
        }
        else if (!other.getAtleastOneYearInit())
        {
            compareResult = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!getAtleastOneYearInit())
        {
            compareResult = ComparisonType::GREATER_THAN;
            return true;
        }

        if (firstYear < other.firstYear)
        {
            compareResult = ComparisonType::LOWER_THAN;
        }
        else if (firstYear > other.firstYear)
        {
            compareResult = ComparisonType::GREATER_THAN;
        }
        else
        {
            compareResult = ComparisonType::EQUAL_TO;
        }

        return true;
    }

    // compares average CGS
    bool compareAvgCGS(const RecordvC &other, ComparisonType &compareResult) const
    {
        float this_avg, other_avg;
        this_avg = -1;
        other_avg = -1;
        this_avg = getAvgCGS();
        other_avg = other.getAvgCGS();
        if (this_avg == -1 && other_avg == -1)
        {
            compareResult = ComparisonType::EQUAL_TO;
            return true;
        }
        else if (other_avg == -1)
        {
            compareResult = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (this_avg == -1)
        {
            compareResult = ComparisonType::GREATER_THAN;
            return true;
        }

        if (this_avg < other_avg)
        {
            compareResult = ComparisonType::LOWER_THAN;
        }
        else if (this_avg > other_avg)
        {
            compareResult = ComparisonType::GREATER_THAN;
        }
        else
        {
            compareResult = ComparisonType::EQUAL_TO;
        }

        return true;
    }

    // checks if the enrollment years of other fully exists in this record
    bool checkEnrollmentYearsExists(const RecordvC &other) const
    {
        if (!other.getAtleastOneYearInit())
        {
            return false;
        }
        else if (!this->getAtleastOneYearInit())
        {
            return false;
        }

        for (int i = 0; i < NUM_YEARS; i++)
        {
            int temp;
            if (other.years[i].getEnrollmentYear(temp))
            { // Count years being checked
                bool found = false;
                for (int j = 0; j < NUM_YEARS; j++)
                {
                    int temp2;
                    if (this->years[j].getEnrollmentYear(temp2))
                    {
                        if (temp == temp2)
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                {
                    return false;
                }
            }
        }

        return true;
    }

    // checks if at least one enrollment year in this record falls within the range [min_record, max_record]
    bool checkEnrollmentYearsInRange(const RecordvC *min_record, const RecordvC *max_record) const
    {
        if (!this->getAtleastOneYearInit())
        {
            return false; // Can't check range if this record has no enrollment years
        }
        if (!min_record->getAtleastOneYearInit() || !max_record->getAtleastOneYearInit())
        {
            return false; // Can't check range if min or max records don't have enrollment years
        }

        // Check each enrollment year in this record
        int min_year, max_year;
        min_record->years[0].getEnrollmentYear(min_year);
        max_record->years[0].getEnrollmentYear(max_year);
        for (int i = 0; i < NUM_YEARS; i++)
        {
            int this_year;

            if (this->years[i].getEnrollmentYear(this_year))
            {
                // Check if this year falls within the range defined by min and max records
                if (this_year >= min_year && this_year <= max_year)
                {
                    return true; // Found at least one year within the range
                }
            }
        }

        return false; // No enrollment years in this record are within the range
    }

    // checks if the cgs of other fully exists in this record
    bool checkCGSExists(const RecordvC &other) const
    {
        if (!other.getAtleastOneYearInit())
        {
            return false;
        }
        else if (!this->getAtleastOneYearInit())
        {
            return false;
        }

        for (int i = 0; i < NUM_YEARS; i++)
        {
            int temp;
            if (other.years[i].getCGS(temp))
            { // Count CGS being checked
                bool found = false;
                for (int j = 0; j < NUM_YEARS; j++)
                {
                    int temp2;
                    if (this->years[j].getCGS(temp2))
                    {
                        if (temp == temp2)
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (!found)
                {
                    return false;
                }
            }
        }
        return true;
    }

    // calculates the average CGS
    float getAvgCGS() const
    {
        float out;
        int average = 0;
        int num_years = 0;
        for (int i = 0; i < NUM_YEARS; i++)
        {
            int temp;
            if (years[i].getCGS(temp))
            {
                average += temp;
                num_years++;
            }
        }

        if (num_years == 0)
        {
            return -1;
        }
        else
        {
            out = ((float)average / ((float)num_years));
            return out;
        }
    }

    // override inherited function
    bool setFromKeyboard()
    {
        bool success = true;
        bool exitSet = false;
        string in_errorMessageHolder = "";

        success &= RecordvB::setFromKeyboard();

        if (success)
        {
            success &= getEnrollmentYearFromKeyboard(in_errorMessageHolder, exitSet);
            if (getAtleastOneYearInit())
            {
                success &= getCGSFromKeyboard(in_errorMessageHolder, exitSet);
            }
            else if (!success)
            {
                cout << "Unexpected Error: getEnrollmentYearFromKeyboard() returned false." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << "User exit from setting from keyboard" << endl;
            }
        }

        setRecordValidity();
        setFirstYear();
        return success;
    }

    // override inherited function
    bool setRecordSingleField(const RecordFields field, string &in_errorMessageHolder, const bool genRandom = false)
    {
        in_errorMessageHolder = "";
        bool temp;
        bool success = true;
        if (genRandom)
        {
            switch (field)
            {
            case RecordFields::FIRST_NAME:
                success = student_name.generateRandomFirstName();
                break;
            case RecordFields::FAMILY_NAME:
                success = student_name.generateRandomLastName();
                break;
            case RecordFields::STUDENT_ID:
                success = student_id.generateRandom();
                break;
            case RecordFields::DEGREE:
                success = degree.generateRandomProg();
                break;
            case RecordFields::DEGREE_TYPE:
                success = degree.generateRandomDegType();
                break;
            case RecordFields::ENROLLMENT_YEAR:
                success = genRandomEnrollmentYears();
                break;
            case RecordFields::CGS:
                success = genRandomCGS();
                break;
            default:
                in_errorMessageHolder = "INVALID FIELD";
                return false;
            }
        }
        else
        {
            switch (field)
            {
            case RecordFields::FIRST_NAME:
                success = getFirstNameFromKeyboard(in_errorMessageHolder);
                break;
            case RecordFields::FAMILY_NAME:
                success = getLastNameFromKeyboard(in_errorMessageHolder);
                break;
            case RecordFields::STUDENT_ID:
                success = getStudentIDFromKeyboard(in_errorMessageHolder, temp);
                break;
            case RecordFields::DEGREE:
                success = getProgrammeChoiceFromKeyboard(in_errorMessageHolder, temp);
                break;
            case RecordFields::DEGREE_TYPE:
                success = getDegreeTypeFromKeyboard(in_errorMessageHolder, temp);
                break;
            case RecordFields::ENROLLMENT_YEAR:
                success = getEnrollmentYearFromKeyboard(in_errorMessageHolder, temp);
                break;
            case RecordFields::CGS:
                success = getCGSFromKeyboard(in_errorMessageHolder, temp);
                break;
            default:
                in_errorMessageHolder = "INVALID FIELD";
                return false;
            }
        }
        setRecordValidity();
        return success;
    }
    // individual setter function for enrollment years
    bool setEnrollmentYear(int year, int index)
    {
        if (getLocked())
        {
            cout << "Record is locked." << endl;
            return true;
        }

        bool success = true;
        string in_errorMessageHolder = "";
        success &= years[index].setEnrollmentYear(year, in_errorMessageHolder);
        if (!success)
        {
            cout << in_errorMessageHolder << endl;
        }
        setFirstYear();
        return success;
    }

    // individual setter function for CGS
    bool setCGS(int CGS, int index)
    {
        if (getLocked())
        {
            cout << "Record is locked." << endl;
            return true;
        }

        bool success = true;
        string in_errorMessageHolder = "";
        if (!years[index].getEnrollmentYearFieldInitFlag())
        {
            cout << "Year for index " << index << " not initialised. Could not set CGS" << endl;
            return false;
        }
        success &= years[index].setCGS(CGS, in_errorMessageHolder);
        if (!success)
        {
            cout << in_errorMessageHolder << endl;
        }

        return success;
    }

    // gets user input for CGS value from keyboard
    bool getCGSFromKeyboard(string &in_errorMessageHolder, bool &exitSet)
    {
        if (!getAtleastOneYearInit())
        {
            cout << "To set CGS, atleast one year should be initialized." << endl;
            return false;
        }

        exitSet = false;
        in_errorMessageHolder = "";

        bool success = true;
        string prompt;
        int CGS;
        int failCount = 0;

        for (int i = 0; i < NUM_YEARS; i++)
        {
            int temp;
            if (years[i].getCGSInitFlag())
            {
                continue;
            }
            if (years[i].getEnrollmentYear(temp))
            {
                prompt = "Type CGS (EXIT to stop) for " + to_string(temp) + " and hit ENTER";
                success = stringInputToInt(prompt, CGS, exitSet, in_errorMessageHolder);
                if (exitSet)
                {
                    cout << "User exit from set from keyboard" << endl;
                    return true;
                }
                else
                {
                    if (success)
                    {
                        success = years[i].setCGS(CGS, in_errorMessageHolder);
                    }
                    while (!success && !exitSet)
                    {
                        if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
                        {
                            cout << "Unexpected Error: RecordvC::getCGSFromKeybpard() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to invalid user inputs. If not, refer to devs." << endl;
                            success = false;
                            bailout();
                        }
                        else
                        {
                            cout << in_errorMessageHolder << endl;
                            in_errorMessageHolder = "";
                            success = stringInputToInt(prompt, CGS, exitSet, in_errorMessageHolder);
                            if (exitSet)
                            {
                                cout << "User exit from set from keyboard" << endl;
                                return true;
                            }
                            else if (!success)
                            {
                                cout << in_errorMessageHolder << endl;
                                in_errorMessageHolder = "";
                                continue;
                            }

                            success &= years[i].setCGS(CGS, in_errorMessageHolder);
                            failCount++;
                        }
                    }
                }
            }
        }
        return success;
    }

    // gets user input for enrollment year from keyboard
    bool getEnrollmentYearFromKeyboard(string &in_errorMessageHolder, bool &exitSet)
    {
        exitSet = false;
        in_errorMessageHolder = "";

        bool success = true;
        string prompt;
        int year;
        int failCount = 0;

        for (int i = 0; i < NUM_YEARS; i++)
        {
            if (years[i].getEnrollmentYearFieldInitFlag())
            {
                continue;
            }
            string response;
            cout << "Add a year? [y/n]" << endl;
            cin >> response;
            cout << endl;
            if (response == "y")
            {
                prompt = "Type Enrollment Year [" + to_string(i + 1) + "] (EXIT to stop) and hit ENTER: ";
                success = stringInputToInt(prompt, year, exitSet, in_errorMessageHolder);
                if (exitSet)
                {
                    cout << "User exit from set from keyboard" << endl;
                    setFirstYear();
                    setRecordValidity();
                    return true;
                }
                else
                {
                    if (success)
                    {
                        success = years[i].setEnrollmentYear(year, in_errorMessageHolder);
                    }
                    while (!success && !exitSet)
                    {
                        if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
                        {
                            cout << "Unexpected Error: RecordvC::getEnrollmentYearFromKeyboard() failed more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to invalid user inputs. If not, refer to devs." << endl;
                            success = false;
                            bailout();
                        }
                        else
                        {
                            cout << in_errorMessageHolder << endl;
                            in_errorMessageHolder = "";

                            success = stringInputToInt(prompt, year, exitSet, in_errorMessageHolder);
                            if (exitSet)
                            {
                                cout << "User exit from set from keyboard" << endl;
                                setFirstYear();
                                setRecordValidity();
                                return true;
                            }
                            else if (!success)
                            {
                                cout << in_errorMessageHolder << endl;
                                in_errorMessageHolder = "";
                                continue;
                            }

                            success &= years[i].setEnrollmentYear(year, in_errorMessageHolder);
                            failCount++;
                        }
                    }
                }
            }
            else if (response == "n")
            {
                break;
            }
            else
            {
                i--;
                cout << "Invalid Input. Try again" << endl;
                continue;
            }
        }

        setFirstYear();
        setRecordValidity();
        return success;
    }

    // returns true if atleast one year has been initialised
    bool getAtleastOneYearInit() const
    {
        for (int i = 0; i < NUM_YEARS; i++)
        {
            if (years[i].getEnrollmentYearFieldInitFlag())
            {
                return true;
            }
        }
        return false;
    }

    // goes through array of years, and sets object firstYear to the lowest year found
    void setFirstYear()
    {
        if (!getAtleastOneYearInit())
        {
            return;
        }

        int minYear = std::numeric_limits<int>::max();
        for (int i = 0; i < NUM_YEARS; i++)
        {
            int temp;
            if (years[i].getEnrollmentYear(temp))
            {
                if (temp < minYear)
                {
                    minYear = temp;
                }
            }
        }
        firstYear = minYear;
    }

    // override inherited function
    bool generateRandom()
    {
        if (getLocked())
        {
            return false;
        }

        bool success = genRandomEnrollmentYears();
        success &= genRandomCGS();
        success &= RecordvB::generateRandom();
        setRecordValidity();
        setFirstYear();
        return success;
    }

    // override inherited function
    void setRecordValidity()
    {
        RecordvB::setRecordValidity();
        if (getAtleastOneYearInit() && valid == RecordValidity::VALID)
        {
            valid = RecordValidity::VALID;
        }
        else if (getAtleastOneYearInit())
        {
            valid = RecordValidity::PARTIALLY_VALID;
        }
    }

    // generates random enrollment years. Takes random first year at years[0]. Then sets the next three indices with firstYear+1, 2, 3
    bool genRandomEnrollmentYears()
    {
        bool success = true;
        if (!years[0].generateRandomEnrollmentYear())
        {
            cout << "Unexpected Error: years[0].generateRandomEnrollmentYear() in RecordvC.genRandomEnrollmentYears() returned false." << endl;
            success = false;
            bailout();
        }

        int temp = 0;
        string temp_message;
        years[0].getEnrollmentYear(temp);
        for (int i = 1; i < 4; i++)
        {
            if (temp + i > MAX_ENROLLMENT_YEAR)
            {
                break;
            }
            success &= years[i].setEnrollmentYear(temp + i, temp_message);
            if (!success)
            {
                cout << "Unexpected Error: years[i].setEnrollmentYear() in RecordvC.genRandomEnrollmentYears() returned false" << endl;
                cout << temp << endl;
                bailout();
            }
        }

        setFirstYear();
        setRecordValidity();
        return success;
    }

    // generates random CGS for initialised years.
    bool genRandomCGS()
    {
        bool success = true;
        for (int i = 0; i < NUM_YEARS; i++)
        {
            if (years[i].getEnrollmentYearFieldInitFlag())
            {
                years[i].generateRandomCGS();
            }
            else
            {
                if (i == 0)
                {
                    cout << "No years are initialised. Cannot generate CGS." << endl;
                    success = false;
                    break;
                }
            }
        }
        return success;
    }

    // overrides inherited function
    RecordFields intToField(const int val)
    {
        switch (val)
        {
        case 1:
            return RecordFields::FIRST_NAME;
        case 2:
            return RecordFields::FAMILY_NAME;
        case 3:
            return RecordFields::STUDENT_ID;
        case 4:
            return RecordFields::DEGREE;
        case 5:
            return RecordFields::DEGREE_TYPE;
        case 6:
            return RecordFields::ENROLLMENT_YEAR;
        case 7:
            return RecordFields::CGS;
        default:
            return RecordFields::RECORDvB_STOP;
        }
    }
};
#endif