/*****************************
******************************
** RecordvB Class **
- Developed by Raghav Kejriwal

** Made for Release 2 **

** Assuming C++11
******************************
*****************************/

#ifndef RECORDvB_R2_H
#define RECORDvB_R2_H

#include <iostream>
using namespace std;

#include "Record.h"
#include "FieldR2.h"

#define MAX_FAIL_SET_FROM_KEYBOARD 5

// Implements a class containing a student record
// contains Student ID, Name, Degree
class RecordvB : public BasicRecord
{
protected:
    NameField student_name;
    StudentIdField student_id;
    DegreeProgrammeField degree;

public:
    // Constructor
    RecordvB() {}

    // Copy constructor
    RecordvB(RecordvB &other) : BasicRecord(other), student_name(other.student_name), student_id(other.student_id), degree(other.degree) {}

    // Writes record to file
    bool writeToStream(std::ostream &os) const
    {
        string first_name, last_name;
        long int id;
        DegreeProgramme prog;
        DegreeType type;

        student_name.getFirstName(first_name);
        student_name.getLastName(last_name);
        student_id.getStudentId(id);
        degree.getProgramme(prog);
        degree.getProgrammeType(type);

        string row = first_name + "," + last_name + "," + to_string(id) + "," + to_string((int)prog) + "," + to_string((int)type);

        os << row;

        return static_cast<bool>(os);
    }

    // Reads record from file
    bool readFromStream(std::istream &is)
    {
        std::string line;
        std::getline(is, line);
        if (line.empty())
            return false;

        // Split by commas
        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);
        size_t p4 = line.find(',', p3 + 1);

        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos || p4 == std::string::npos)
            return false;

        std::string firstname = line.substr(0, p1);
        std::string lastname = line.substr(p1 + 1, p2 - p1 - 1);
        long int idStr = std::stol(line.substr(p2 + 1, p3 - p2 - 1));
        DegreeProgramme degreeStr = static_cast<DegreeProgramme>(std::stoi(line.substr(p3 + 1, p4 - p3 - 1)));
        DegreeType typeStr = static_cast<DegreeType>(std::stoi(line.substr(p4 + 1)));

        // Assign to fields
        string error_msg;
        student_name.setFirstName(firstname);
        student_name.setLastName(lastname);
        student_id.setStudentId(idStr, error_msg);
        degree.setProgramme(degreeStr);
        degree.setProgrammeType(typeStr);

        setRecordValidity();
        return true;
    }

    // set record function to change all fields, option to generate random values
    // Should only return false in error cases (failures)
    bool setRecord(string &in_errorMessageHolder, const bool genRandom = false)
    {
        if (getLocked())
        {
            in_errorMessageHolder = "Record is Locked.";
            return true;
        }

        bool success = true;
        if (genRandom)
        {
            success = generateRandom();
            if (!success)
            {
                cout << "Unexpected Error: generateRandom() in RecordvB constructor returned false." << endl;
                bailout();
            }
        }
        else
        {
            success = setFromKeyboard();
            if (!success)
            {
                cout << "Unexpected Error: setFromKeyboard in RecordvB constructor returned false." << endl;
                bailout();
            }
        }

        return success;
    }

    // Used to edit single field of record.
    // Can edit a random field, or get user inputs
    // returns false if record is locked or failures.
    // Repeats certain number of times if inputs are invalid
    virtual bool editRecord(string &in_errorMessageHolder, const bool genRandom = false)
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
            int high = (int)RecordFields::RECORDvB_STOP - 1;

            int f = low + rand() % (high - low + 1);
            RecordFields field = (RecordFields)f;
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
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

    // Used to compare a record against another and determine
    // if they should be swapped based on SortCriterion critera.
    // SortCriterion tells the function the field to sort by and what order the
    // resultant output should be in (ascending or descending)
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
            if (f <= RecordFields::RECORDvA_STOP || f >= RecordFields::RECORDvB_STOP)
                return false;
            return true;
        };

        if (!checkField(criteria1.field))
        {
            throw std::runtime_error("SortCriterion field out of bounds in RecordvB::compareMultiField()");
        }

        // Error and guarantee no swap if items are not of same type
        const RecordvB *typecasted = typecastItem(other, this);
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
        if (!checkField(criteria2.field)) {
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
        if (!checkField(criteria3.field)) {
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

    bool compareSingleField(const RecordvB *other, const SortCriterion &criteria, ComparisonType &compareResult) const
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

        default:
            success = false;
            throw std::runtime_error("RecordvB::compareSingleField() invalid field");
        }

        return success;
    }

    virtual bool printInfo() const
    {
        if (getRecordInvalid())
        {
            cout << "Record is Invalid" << endl;
            return false;
        }

        string name;
        cout << "First Name: ";
        if (student_name.getFirstName(name))
        {
            cout << name << endl;
        }
        else
        {
            cout << endl;
        }
        cout << "Last Name: ";
        if (student_name.getLastName(name))
        {
            cout << name << endl;
        }
        else
        {
            cout << endl;
        }

        long int id;
        cout << "Student ID: ";
        if (student_id.getStudentId(id))
        {
            cout << id << endl;
        }
        else
        {
            cout << endl;
        }
        cout << "Degree Programme: ";
        DegreeProgramme prog;
        if (degree.getProgramme(prog))
        {
            cout << convertDegreeProgrammeToString(prog) << endl;
        }
        else
        {
            cout << endl;
        }
        cout << "Degree Type: ";
        DegreeType type;
        if (degree.getProgrammeType(type))
        {
            cout << convertDegreeTypeToString(type) << endl;
        }
        else
        {
            cout << endl;
        }
        return true;
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

    // Individual setter function to set first name by argument
    bool setFirstName(string name)
    {
        if (getLocked())
        {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        string in_errorMessageHolder = "";
        success &= stringClean(name, in_errorMessageHolder);

        if (success)
        {
            success &= student_name.setFirstName(name);
        }
        else
        {
            cout << in_errorMessageHolder << endl;
        }
        setRecordValidity();
        return success;
    }

    // Individual setter function to set last name by argument
    bool setLastName(string name)
    {
        if (getLocked())
        {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        string in_errorMessageHolder = "";
        success &= stringClean(name, in_errorMessageHolder);

        if (success)
        {
            success &= student_name.setLastName(name);
        }
        else
        {
            cout << in_errorMessageHolder << endl;
        }
        setRecordValidity();
        return success;
    }

    // Individual setter function to set studentID by argument
    bool setStudentID(const int id)
    {
        if (getLocked())
        {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        string in_errorMessageHolder = "";

        success &= student_id.setStudentId(id, in_errorMessageHolder);
        if (!success)
        {
            cout << in_errorMessageHolder << endl;
        }
        setRecordValidity();
        return success;
    }

    // individual setter function to set degree programme (subject) by argument
    bool setDegreeProgramme(const DegreeProgramme prog)
    {
        if (getLocked())
        {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        success &= degree.setProgramme(prog);
        setRecordValidity();
        return success;
    }

    // Individual setter function to set degree type (BEng, MEng) by argument
    bool setDegreeType(const DegreeType type)
    {
        if (getLocked())
        {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        success &= degree.setProgrammeType(type);
        setRecordValidity();
        return success;
    }

    // Gets inputs from user for various sort criteria. Sets any other argument
    // to unsorted in field. Returns true if atleast one field is specified,
    // false if no fields are valid
    static bool getSortCriteriaFromUser(SortCriterion &crit1, SortCriterion &crit2, SortCriterion &crit3) {
        bool exitSet = false;
        bool success = true;

        success = getSingleSortCriteriaFromUser(crit1, exitSet);

        if (!success) {
            crit1.field = RecordFields::UNSORTED;
            crit2.field = RecordFields::UNSORTED;
            crit3.field = RecordFields::UNSORTED;
            return success;
        }

        if (exitSet) {
            crit1.field = RecordFields::UNSORTED;
            crit2.field = RecordFields::UNSORTED;
            crit3.field = RecordFields::UNSORTED;
            return false;
        }

        success = getSingleSortCriteriaFromUser(crit2, exitSet);

        if (!success || exitSet) {
            crit2.field = RecordFields::UNSORTED;
            crit3.field = RecordFields::UNSORTED;
            return true;
        }

        success = getSingleSortCriteriaFromUser(crit3, exitSet);

        if (!success || exitSet) {
            crit3.field = RecordFields::UNSORTED;
            return true;
        }

        return true;
    }

    bool partialMatchCompare(const SortCriterion &criteria, const string match_str) {
        bool match = false;
        long int match_val = 0;
        string prompt, errorMessage;
        switch (criteria.field) {
        case RecordFields::FIRST_NAME:
            match = student_name.partialMatchFirstName(match_str);
            break;
        case RecordFields::FAMILY_NAME:
            match = student_name.partialMatchLastName(match_str);
            break;
        case RecordFields::STUDENT_ID:
            stol(match_str);
            match = student_id.partialMatchStudentId(match_val);
            break;
        case RecordFields::DEGREE:
            match = degree.partialMatchProgramme(match_str);
            break;
        case RecordFields::DEGREE_TYPE:
            match = degree.partialMatchProgrammeType(match_str);
            break;
        default:
            cout << "Invalid Choice" << endl;
            match = false;
        }

        return match;
    }

    // disable assignment operator
    void operator=(const RecordvB &record) = delete;

    // disable comparision operators
    bool operator==(const RecordvB &record) = delete;
    bool operator!=(const RecordvB &record) = delete;
    bool operator<(const RecordvB &record) = delete;
    bool operator>(const RecordvB &record) = delete;
    bool operator<=(const RecordvB &record) = delete;
    bool operator>=(const RecordvB &record) = delete;

protected:
    // Private function used to set a single Field
    virtual bool setRecordSingleField(const RecordFields field, string &in_errorMessageHolder, const bool genRandom = false)
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
            default:
                in_errorMessageHolder = "INVALID FIELD";
                return false;
            }
        }
        setRecordValidity();
        return success;
    }

    // Accept a user input from keyboard, carry out validation and clean string
    // and setFirstName
    bool getFirstNameFromKeyboard(string &in_errorMessageHolder)
    {
        in_errorMessageHolder = "";
        bool success = true;
        int failCount = 0;
        string firstNameHolder = "";
        // Accept first name input and clean string
        cout << "Type First Name: ";
        cin >> firstNameHolder;
        cout << endl;

        success = stringClean(firstNameHolder, in_errorMessageHolder);
        failCount++;

        while (!success)
        {
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error: stringClean() in getFirstNameFromKeyboard() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to user entering invalid characters. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";

                cout << "Type First Name: ";
                cin >> firstNameHolder;
                cout << endl;

                success = stringClean(firstNameHolder, in_errorMessageHolder);
                failCount++;
            }
        }

        if (success)
        {
            success &= student_name.setFirstName(firstNameHolder);
        }

        if (!success)
        {
            cout << "Unexpected Error: student_name.setFirstName() in getFirstNameFromKeyboard() returned false." << endl;
            bailout();
        }

        return success;
    }

    // Accept a user input from keyboard, carry out validation and clean string
    // and setLastName
    bool getLastNameFromKeyboard(string &in_errorMessageHolder)
    {
        in_errorMessageHolder = "";
        bool success = true;
        int failCount = 0;
        // Accept last name input and clean string
        string lastNameHolder = "";
        cout << "Type Last Name: ";
        cin >> lastNameHolder;
        cout << endl;

        success = stringClean(lastNameHolder, in_errorMessageHolder);
        failCount++;

        while (!success)
        {
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error. stringClean() in getLastNameFromKeyboard() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to user entering invalid characters. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";

                cout << "Type Last Name: ";
                cin >> lastNameHolder;
                cout << endl;

                success = stringClean(lastNameHolder, in_errorMessageHolder);
                failCount++;
            }
        }

        if (success)
        {
            success &= student_name.setLastName(lastNameHolder);
        }

        if (!success)
        {
            cout << "Unexpected Error: student_name.setLastName in getFirstNameFromKeyboard() returned false." << endl;
            bailout();
        }

        return success;
    }

    // Accept user input from keyboard, carry out validation and
    // set studentID
    bool getStudentIDFromKeyboard(string &in_errorMessageHolder, bool &exitSet)
    {
        exitSet = false;
        in_errorMessageHolder = "";
        // accept input for student ID, convert to long int
        bool success = true;
        string prompt;
        long int student_id_holder;
        int failCount = 0;

        if (success && !exitSet)
        {
            prompt = "Type Student ID (EXIT to stop) and hit ENTER: ";
            success = stringInputToLongInt(prompt, student_id_holder, exitSet, in_errorMessageHolder);
        }

        if (exitSet)
        {
            cout << "User exit from set from keyboard" << endl;
        }
        else if (!success)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        if (success && !exitSet)
        {
            success &= student_id.setStudentId(student_id_holder, in_errorMessageHolder);
            failCount++;
        }
        /*failCount is incremented because it is set to 0 after loop anyways. so it doesnt matter if it is incremented regardless of success
        Equivalent to:
        if (!success) {
            failCount++;
        }
        */

        // repeats student ID set until valid input or return false from member object more than set no of times
        while (!success && !exitSet)
        {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error: student_id.setStudentId() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This may be due to user setting invalid student ID. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";

                prompt = "Type Student ID (EXIT to stop) and hit ENTER: ";
                success = stringInputToLongInt(prompt, student_id_holder, exitSet, in_errorMessageHolder);

                if (exitSet)
                {
                    cout << "User exit from set from keyboard" << endl;
                    break;
                }
                else if (!success)
                {
                    cout << in_errorMessageHolder << endl;
                    in_errorMessageHolder = "";
                    continue;
                }

                success &= student_id.setStudentId(student_id_holder, in_errorMessageHolder);
                failCount++;
            }
        }

        return success;
    }

    // get user input from keyboard, carry out validation, and setProgramme (subject)
    bool getProgrammeChoiceFromKeyboard(string &in_errorMessageHolder, bool &exitSet)
    {
        exitSet = false;
        in_errorMessageHolder = "";
        bool success = true;
        int failCount = 0;
        // accept input for programme choice
        int programme_holder;
        DegreeProgramme enum_programme_holder;

        if (success && !exitSet)
        {
            cout << "Choose a Degree Programme. Type:" << endl
                 << "1 for Chemical Engineering" << endl
                 << "2 for Civil Engineering" << endl
                 << "3 for Electrical and Electronics Engineering" << endl
                 << "4 for Mechanical Engineering" << endl
                 << "5 for Petroleum Engineering" << endl
                 << "EXIT to stop" << endl;

            // get string input, convert to int
            success = stringInputToInt(programme_holder, exitSet, in_errorMessageHolder);
        }

        if (exitSet)
        {
            cout << "User exit from set from keyboard" << endl;
        }
        else if (!success)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        // convert int to enum DegreeProgramme
        if (success && !exitSet)
        {
            enum_programme_holder = intToDegreeProg(programme_holder);
            if (enum_programme_holder == DegreeProgramme::NONE)
            {
                success = false;
                cout << "Invalid Degree Programme choice. Try again." << endl;
            }
        }

        if (success && !exitSet)
        {
            success &= degree.setProgramme(enum_programme_holder);
            failCount++;
        }

        // repeats above until valid input or member object returns false more than set no of times
        while (!success && !exitSet)
        {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error: degree.setProgramme() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This MAY be due to user setting invalid inputs. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << "Choose a Degree Programme. Type:" << endl
                     << "1 for Chemical Engineering" << endl
                     << "2 for Civil Engineering" << endl
                     << "3 for Electrical and Electronics Engineering" << endl
                     << "4 for Mechanical Engineering" << endl
                     << "5 for Petroleum Engineering" << endl
                     << "EXIT to stop" << endl;

                success = stringInputToInt(programme_holder, exitSet, in_errorMessageHolder);

                if (exitSet)
                {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                }
                else if (!success)
                {
                    cout << in_errorMessageHolder << endl;
                    in_errorMessageHolder = "";
                    continue;
                }

                enum_programme_holder = intToDegreeProg(programme_holder);
                if (enum_programme_holder == DegreeProgramme::NONE)
                {
                    success = false;
                    cout << "Invalid Degree Programme choice. Try again." << endl;
                    continue;
                }

                failCount++;
                success &= degree.setProgramme(enum_programme_holder);
            }
        }

        return success;
    }

    // get user input from keyboard, carry out validation and set type of degree (BEng, MEng)
    bool getDegreeTypeFromKeyboard(string &in_errorMessageHolder, bool &exitSet)
    {
        exitSet = false;
        in_errorMessageHolder = "";
        int failCount = 0;
        bool success = true;
        string str_degree_type_holder = "";
        int degree_type_holder;
        DegreeType enum_degree_type_holder;

        // accept input for degree type
        if (success && !exitSet)
        {
            cout << "Choose a Degree Type. Type:" << endl
                 << "1 for Bachelor of Engineering (BEng)" << endl
                 << "2 for Master of Engineering (MEng)" << endl
                 << "EXIT to stop" << endl;

            // get string input, convert to int
            success = stringInputToInt(degree_type_holder, exitSet, in_errorMessageHolder);
        }

        if (exitSet)
        {
            cout << "User exit from set from keyboard" << endl;
        }
        else if (!success)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        // convert int to enum DegreeType
        if (success && !exitSet)
        {
            enum_degree_type_holder = intToDegreeType(degree_type_holder);
            if (enum_degree_type_holder == DegreeType::NONE)
            {
                success = false;
                cout << "Invalid Degree Type choice. Try again." << endl;
            }
        }

        if (success && !exitSet)
        {
            success &= degree.setProgrammeType(enum_degree_type_holder);
            failCount++;
        }

        // repeats above until valid input or member object returns false more than set no of times
        while (!success && !exitSet)
        {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD)
            {
                cout << "Unexpected Error. degree.setProgrammeType() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This MAY be due to user setting invalid inputs. If not, refer to devs." << endl;
                success = false;
                bailout();
            }
            else
            {
                cout << "Choose a Degree Type. Type:" << endl
                     << "1 for Bachelor of Engineering (BEng)" << endl
                     << "2 for Master of Engineering (MEng)" << endl
                     << "EXIT to stop" << endl;

                success = stringInputToInt(degree_type_holder, exitSet, in_errorMessageHolder);

                if (exitSet)
                {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                }
                else if (!success)
                {
                    cout << in_errorMessageHolder << endl;
                    in_errorMessageHolder = "";
                    continue;
                }

                enum_degree_type_holder = intToDegreeType(degree_type_holder);
                if (enum_degree_type_holder == DegreeType::NONE)
                {
                    success = false;
                    cout << "Invalid Degree Type choice. Try again." << endl;
                    continue;
                }

                failCount++;
                success &= degree.setProgrammeType(enum_degree_type_holder);
            }
        }

        return success;
    }

    virtual bool getFieldChoiceFromKeyboard(string &in_errorMessageHolder, RecordFields &chosenField, bool &exitSet) {
        bool success = true;
        int field_holder;
        int failCount = 0;

        cout << "Choose a field. Type:" << endl
             << "1 for First Name" << endl
             << "2 for Family Name" << endl
             << "3 for Student ID" << endl
             << "4 for Degree" << endl
             << "5 for Degree Type" << endl
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
            if (chosenField <= RecordFields::RECORDvA_STOP || chosenField >= RecordFields::RECORDvB_STOP)
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
                             << "5 for Degree Type" << endl;
                        cout << "EXIT to stop" << endl;
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

                    if (chosenField <= RecordFields::RECORDvA_STOP || chosenField >= RecordFields::RECORDvB_STOP)
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

    static bool getSingleSortCriteriaFromUser(SortCriterion &crit1, bool &exitStatus) {
        string in_errorMessageHolder = "";
        bool success = true;
        int order_holder;
        RecordvB temp;
        success = temp.getFieldChoiceFromKeyboard(in_errorMessageHolder, crit1.field, exitStatus);
        if (exitStatus) {
            crit1.field = RecordFields::UNSORTED;
            return true;
        } else if (!success) {
            crit1.field = RecordFields::UNSORTED;
            cout << in_errorMessageHolder << endl;
            return false;
        }

        int failCount = 0;
        
        cout << "Choose ascending or descending (Type and hit ENTER):" << endl
             << "1 for Ascending" << endl
             << "2 for descending" << endl
             << "EXIT to stop" << endl;
        
        success = stringInputToInt(order_holder, exitStatus, in_errorMessageHolder);

        if (exitStatus) {
            crit1.field = RecordFields::UNSORTED;
            cout << "User exit from set from keyboard" << endl;
            return true;
        } else if (!success) {
            failCount++;
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        } else if (order_holder != 1 && order_holder != 2) {
            success = false;
            cout << "Invalid Input." << endl;
            failCount++;
        }

        while (!success && !exitStatus) {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD) {
                cout << "Unexpected Error: stringInputToInt() in getSingleSortCriteriaFromUser() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This MAY be due to user setting invalid inputs. If not, refer to devs." << endl;
                success = false;
                bailout();
            } else {
                cout << "Choose ascending or descending (Type and hit ENTER):" << endl
                     << "1 for Ascending" << endl
                     << "2 for descending" << endl
                     << "EXIT to stop" << endl;
                
                success = stringInputToInt(order_holder, exitStatus, in_errorMessageHolder);

                if (exitStatus) {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                } else if (!success) {
                    cout << in_errorMessageHolder << endl;
                    in_errorMessageHolder = "";
                    continue;
                } else if (order_holder != 1 && order_holder != 2) {
                    success = false;
                    cout << "Invalid Input." << endl;
                    failCount++;
                }
            }
        }

        if (!success) {
            cout << "Setting criteria to unsorted" << endl;
            crit1.field = RecordFields::UNSORTED;
            return false;
        }

        switch (order_holder) {
        case 1:
            crit1.ascending = true;
            break;
        case 2:
            crit1.ascending = false;
            break;
        default:
            crit1.field = RecordFields::UNSORTED;
            cout << "Setting criteria to unsorted" << endl;
            return false;
        }

        return true;
    }

    // Used to accept user inputs for various member objects from keyboard
    virtual bool setFromKeyboard()
    {
        bool success = true;

        string in_errorMessageHolder = "";

        success &= getFirstNameFromKeyboard(in_errorMessageHolder);

        if (!success)
        {
            cout << in_errorMessageHolder;
            in_errorMessageHolder = "";
        }
        else
        {
            valid = RecordValidity::PARTIALLY_VALID; // set to partially valid
            success &= getLastNameFromKeyboard(in_errorMessageHolder);
        }

        bool exitSet = false; // set true if user types exit. Whenever this happens, validity is PARTIALLY_VALID
        if (!success)
        {
            cout << in_errorMessageHolder;
            in_errorMessageHolder = "";
        }
        else
        {
            success &= getStudentIDFromKeyboard(in_errorMessageHolder, exitSet);
        }

        if (!success && !exitSet)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }
        else if (!exitSet)
        {
            success &= getProgrammeChoiceFromKeyboard(in_errorMessageHolder, exitSet);
        }

        if (!success && !exitSet)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }
        else if (!exitSet)
        {
            success &= getDegreeTypeFromKeyboard(in_errorMessageHolder, exitSet);
        }

        if (!success && !exitSet)
        {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        setRecordValidity(); // set record to valid
        return success;
    }

    // Converts integer to DegreeProgramme object
    DegreeProgramme intToDegreeProg(const int val) const
    {
        switch (val)
        {
        case 1:
            return DegreeProgramme::CHEM;
        case 2:
            return DegreeProgramme::CIVIL;
        case 3:
            return DegreeProgramme::EEE;
        case 4:
            return DegreeProgramme::MECH;
        case 5:
            return DegreeProgramme::PETR;
        default:
            return DegreeProgramme::NONE;
        }
    }

    // Converts integer to DegreeType object
    DegreeType intToDegreeType(const int val) const
    {
        switch (val)
        {
        case 1:
            return DegreeType::BENG;
        case 2:
            return DegreeType::MENG;
        default:
            return DegreeType::NONE;
        }
    }

    // Converts integer to RecordFields object
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
        default:
            return RecordFields::RECORDvB_STOP;
        }
    }

    // Calls random value generator functions for all objects
    virtual bool generateRandom()
    {
        if (getLocked())
        {
            return false;
        }

        bool success = true;
        if (!student_name.generateRandom())
        {
            cout << "Unexpected Error: student_name.generateRandom() in RecordvB.generateRandom() returned false." << endl;
            success = false;
            bailout();
        }
        if (!student_id.generateRandom())
        {
            cout << "Unexpected Error: student_id.generateRandom() in RecordvB.generateRandom() returned false." << endl;
            success = false;
            bailout();
        }
        if (!degree.generateRandom())
        {
            cout << "Unexpected Error: degree.generateRandom() in RecordvB.generateRandom() returned false." << endl;
            success = false;
            bailout();
        }

        setRecordValidity();
        return success;
    }

    // sets record validity using various initFlags of the member objects.
    // To be used at end of any set functions
    // Sets to VALID if all initFlags are true, INVALID if none are true,
    // PARTIALLY_VALID if some are true
    virtual void setRecordValidity()
    {
        if (student_name.getFirstNameInitFlag() && student_name.getLastNameInitFlag() && student_id.getInitFlag() && degree.getProgrammeInitFlag() && degree.getProgrammeTypeInitFlag())
        {
            BasicRecord::setRecordValid();
        }
        else if (!student_name.getFirstNameInitFlag() && !student_name.getLastNameInitFlag() && !student_id.getInitFlag() && !degree.getProgrammeInitFlag() && !degree.getProgrammeTypeInitFlag())
        {
            setRecordInvalid();
        }
        else
        {
            setRecordPartValid();
        }
    }
};
#endif
