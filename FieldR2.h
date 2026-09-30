#pragma once
#include "Definitions.h"
#include "BasicField.h"
#include <algorithm>
#include <cctype>

static bool substringMatch(const string &a, const string &b)
{
    // Check substring
    if (a.find(b) != std::string::npos)
        return true;
    if (b.find(a) != std::string::npos)
        return true;
    return false;
}

class NameField : public BasicField
{
protected:
    string first_name;
    string last_name;

    bool firstNameInitFlag;
    bool lastNameInitFlag;

    void setFirstNameInitFlag()
    {
        firstNameInitFlag = true;
    }

    void setLastNameInitFlag()
    {
        lastNameInitFlag = true;
    }

public:
    NameField() : firstNameInitFlag(false), lastNameInitFlag(false) {}

    NameField(const string &first, const string &last) : first_name(first), last_name(last)
    {
        setFirstNameInitFlag();
        setLastNameInitFlag();
    }

    NameField(const NameField &other) : BasicField(other)
    {
        if (other.getFirstNameInitFlag())
        {
            this->first_name = other.first_name;
            setFirstNameInitFlag();
        }
        if (other.getLastNameInitFlag())
        {
            this->last_name = other.last_name;
            setLastNameInitFlag();
        }
    }

    ~NameField() {}

    bool getFirstNameInitFlag() const
    {
        return firstNameInitFlag; // consider parent initFlag as first name initFlag
    }

    bool getLastNameInitFlag() const
    {
        return lastNameInitFlag;
    }

    bool getFirstName(string &out) const
    {
        if (getFirstNameInitFlag())
        {
            out = this->first_name;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool getLastName(string &out) const
    {
        if (getLastNameInitFlag())
        {
            out = this->last_name;
            return true;
        }
        else
        {
            return false;
        }
    }

    string printFieldType() const override
    {
        return convertRecordFieldToString(RecordFields::FIRST_NAME) + " " + convertRecordFieldToString(RecordFields::FAMILY_NAME);
    }

    bool setFirstName(const string &first)
    {
        this->first_name = first;
        setFirstNameInitFlag();
        return true;
    }

    bool setLastName(const string &last)
    {
        this->last_name = last;
        setLastNameInitFlag();
        return true;
    }

    // Not used for Name comparison since name is split into first and last names
    // but since BasicField requires it, we provide a dummy implementation
    bool compare(__attribute__((unused)) const BasicField &field_1, __attribute__((unused)) ComparisonType &type) const override
    {
        cout << "Use compareFirstNames() or compareLastNames() Functions for Name comparison." << endl;
        return true;
    }

    bool compareFirstNames(const BasicField &field_1, ComparisonType &type) const
    {
        // returns negative, zero, positive if *this->first_name* is
        // alphabetically before, equal to, or after field_1.first_name respectively
        const NameField *name_field1 = dynamic_cast<const NameField *>(&field_1);
        if (name_field1 == nullptr || !name_field1->getFirstNameInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }

        if (!getFirstNameInitFlag())
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }

        int comparison = this->first_name.compare(name_field1->first_name);

        if (comparison < 0)
            type = ComparisonType::LOWER_THAN;
        else if (comparison > 0)
            type = ComparisonType::GREATER_THAN;
        else
            type = ComparisonType::EQUAL_TO;

        return true;
    }

    bool compareLastNames(const BasicField &field_1, ComparisonType &type) const
    {
        // returns negative, zero, positive if *this->last_name* is
        // alphabetically before, equal to, or after field_1.last_name respectively
        const NameField *name_field1 = dynamic_cast<const NameField *>(&field_1);
        if (name_field1 == nullptr || !name_field1->getLastNameInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }

        if (!getLastNameInitFlag())
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }

        int comparison = this->last_name.compare(name_field1->last_name);

        if (comparison < 0)
            type = ComparisonType::LOWER_THAN;
        else if (comparison > 0)
            type = ComparisonType::GREATER_THAN;
        else
            type = ComparisonType::EQUAL_TO;

        return true;
    }

    bool partialMatchFirstName(const string &other) const
    {
        if (other.empty())
            return false;

        string a, b;
        this->getFirstName(a);
        b = other;

        // lowercase both
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        if (a.empty() || b.empty())
            return false;

        // Check substring
        return substringMatch(a, b);
    }

    bool partialMatchLastName(const string &other) const
    {
        if (other.empty())
            return false;

        string a, b;
        this->getLastName(a);
        b = other;

        // lowercase both
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        if (a.empty() || b.empty())
            return false;

        // Check substring
        return substringMatch(a, b);
    }

    bool generateRandom() override
    {
        bool success = generateRandomFirstName();
        success &= generateRandomLastName();
        return success;
    }

    bool generateRandomFirstName()
    {
        string firstNames[] = {
            "Alice", "Bob", "Charlie", "Diana", "Edward", "Fiona", "George", "Helen",
            "Ivan", "Julia", "Kevin", "Laura", "Michael", "Nina", "Oliver", "Paula",
            "Quinn", "Rachel", "Steven", "Tina", "Ulrich", "Victoria", "William", "Xara",
            "Yuki", "Zoe", "Adam", "Beth", "Carl", "Donna"};

        int first_index = rand() % static_cast<int>(sizeof(firstNames) / sizeof(firstNames[0]));
        first_name = firstNames[first_index];
        setFirstNameInitFlag();
        return true;
    }

    bool generateRandomLastName()
    {
        string lastNames[] = {
            "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
            "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson",
            "Thomas", "Taylor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson",
            "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson"};

        int last_index = rand() % static_cast<int>(sizeof(lastNames) / sizeof(lastNames[0]));
        last_name = lastNames[last_index];
        setLastNameInitFlag();
        return true;
    }
};

class StudentIdField : public BasicField
{
protected:
    long int student_id; // Student ID value
    bool initFlag;

    void setInitFlag()
    {
        initFlag = true;
    }

public:
    StudentIdField() : initFlag(false) {}
    StudentIdField(const StudentIdField &other) : BasicField(other), initFlag(other.initFlag)
    {
        if (other.getInitFlag())
        {
            this->student_id = other.student_id;
        }
    }

    ~StudentIdField() {}

    bool getStudentId(long int &out) const
    {
        if (getInitFlag())
        {
            out = this->student_id;
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
        return convertRecordFieldToString(RecordFields::STUDENT_ID);
    }

    bool setStudentId(long int id, string &error_message)
    {
        // Validate the student ID according to predefined rules
        bool isValid = checkIfStudentIdAdheresToRules(id, error_message);
        if (isValid)
        {
            this->student_id = id;
            setInitFlag();
        }

        return isValid;
    }

    bool checkIfStudentIdAdheresToRules(long int id, string &error_message)
    {
        string id_str = to_string(id); // Convert ID to string for easier digit access

        // Check length is 8 digits
        if (id_str.length() != 8)
        {
            error_message = "Invalid Student ID length.";
            return false;
        }

        int first_part = stoi(id_str.substr(0, 1));   // First digit
        int second_part = stoi(id_str.substr(1, 2));  // Second and third digits
        long int third_part = stol(id_str.substr(3)); // Last five digits

        // Validate according to rules
        if (first_part != 5)
        {
            error_message = "ID must start with 5";
            return false;
        }

        if (second_part < 10 || second_part > 20)
        {
            error_message = "2nd and 3rd digits must be between 10 and 20.";
            return false;
        }

        if (third_part < 0 || third_part > 99999)
        {
            error_message = "Last five digits must be between 0 and 99999.";
            return false;
        }

        error_message = "";
        return true;
    }

    bool compare(const BasicField &field_1, ComparisonType &type) const override
    {
        // Use dynamic_cast to safely check if both objects are StudentIdField
        const StudentIdField *id_field1 = dynamic_cast<const StudentIdField *>(&field_1);

        // If either cast failed, the objects are not of the same type (StudentIdField)
        if (id_field1 == nullptr || !id_field1->getInitFlag())
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!getInitFlag())
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }

        // Compare student IDs
        else if (this->student_id < id_field1->student_id)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (this->student_id > id_field1->student_id)
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

    bool partialMatchStudentId(const long int &other) const
    {
        string a = to_string(this->student_id);
        string b = to_string(other);

        if (a.empty() || b.empty())
            return false;

        // Check substring
        return substringMatch(a, b);
    }

    bool generateRandom() override
    {
        int second_part = (rand() % 11) + 10;  // Random number between 10 and 20
        long int third_part = rand() % 100000; // Random number between 0 and 99999

        long int random_id = 50000000 + second_part * 100000 + third_part;
        student_id = random_id;
        setInitFlag();
        return true;
    }

    bool getInitFlag() const
    {
        return initFlag;
    }
};

class DegreeProgrammeField : public BasicField
{
protected:
    DegreeProgramme programme; // Degree programme value (e.g., CHEM, CIVIL)
    DegreeType programme_type; // Degree type value (BEng, MEng)
    bool programmeInitFlag;
    bool programmeTypeInitFlag;

    void setProgrammeInitFlag()
    {
        programmeInitFlag = true;
    }

    void setProgrammeTypeInitFlag()
    {
        programmeTypeInitFlag = true;
    }

public:
    DegreeProgrammeField() : programmeInitFlag(false), programmeTypeInitFlag(false) {}

    DegreeProgrammeField(const DegreeProgrammeField &other) : BasicField(other),
                         programmeInitFlag(other.programmeInitFlag),
                         programmeTypeInitFlag(other.programmeTypeInitFlag)
    {
        if (other.getProgrammeInitFlag())
        {
            this->programme = other.programme;
        }
        if (other.getProgrammeTypeInitFlag())
        {
            this->programme_type = other.programme_type;
        }
    }
    ~DegreeProgrammeField() {}

    bool getProgrammeInitFlag() const
    {
        return programmeInitFlag;
    }

    bool getProgrammeTypeInitFlag() const
    {
        return programmeTypeInitFlag;
    }

    bool getProgramme(DegreeProgramme &out) const
    {
        if (getProgrammeInitFlag())
        {
            out = this->programme;
            return true;
        }
        else
        {
            return false;
        }
    }

    bool getProgrammeType(DegreeType &out) const
    {
        if (getProgrammeTypeInitFlag())
        {
            out = this->programme_type;
            return true;
        }
        else
        {
            return false;
        }
    }

    string printFieldType() const override
    {
        return convertRecordFieldToString(RecordFields::DEGREE);
    }

    bool setProgramme(DegreeProgramme prog)
    {
        this->programme = prog;
        setProgrammeInitFlag();
        return true;
    }

    bool setProgrammeType(DegreeType type)
    {
        this->programme_type = type;
        setProgrammeTypeInitFlag();
        return true;
    }

    bool compare(__attribute__((unused)) const BasicField &field_1, __attribute__((unused)) ComparisonType &type) const override
    {
        cout << "Use compareProgramme() or compareProgrammeType() Functions for Degree Programme comparison." << endl;
        return true;
    }

    bool compareProgramme(const BasicField &field_1, ComparisonType &type) const
    {
        // Use dynamic_cast to safely check if both objects are DegreeProgrammeField
        const DegreeProgrammeField *prog_field_1 = dynamic_cast<const DegreeProgrammeField *>(&field_1);

        // If either cast failed, the objects are not of the same type (DegreeProgrammeField) or if one has no field set
        if (prog_field_1 == nullptr)
        {
            type = ComparisonType::NOT_COMPARABLE;
            return true;
        }

        bool this_progInit = getProgrammeInitFlag();
        bool other_progInit = prog_field_1->getProgrammeInitFlag();

        // Not comparable if both are completely uninitialized
        if (!this_progInit && !other_progInit)
        {
            type = ComparisonType::NOT_COMPARABLE;
            return true;
        }
        else if (this_progInit && !other_progInit)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!this_progInit && other_progInit)
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }
        else if (this_progInit && other_progInit)
        {
            // Compare degree programmes first which are sorted because of the enum order
            if (this->programme < prog_field_1->programme)
            {
                type = ComparisonType::LOWER_THAN;
                return true;
            }
            else if (this->programme > prog_field_1->programme)
            {
                type = ComparisonType::GREATER_THAN;
                return true;
            }
            else
            {
                type = ComparisonType::EQUAL_TO;
                return true;
            }
        }

        return false;
    }

    bool compareProgrammeType(const BasicField &field_1, ComparisonType &type) const
    {
        // Use dynamic_cast to safely check if both objects are DegreeProgrammeField
        const DegreeProgrammeField *prog_field_1 = dynamic_cast<const DegreeProgrammeField *>(&field_1);

        // If either cast failed, the objects are not of the same type (DegreeProgrammeField) or if one has no field set
        if (prog_field_1 == nullptr)
        {
            type = ComparisonType::NOT_COMPARABLE;
            return true;
        }

        bool this_typeInit = getProgrammeTypeInitFlag();
        bool other_typeInit = prog_field_1->getProgrammeTypeInitFlag();

        // Not comparable if both are completely uninitialized
        if (!this_typeInit && !other_typeInit)
        {
            type = ComparisonType::NOT_COMPARABLE;
            return true;
        }
        else if (this_typeInit && !other_typeInit)
        {
            type = ComparisonType::LOWER_THAN;
            return true;
        }
        else if (!this_typeInit && other_typeInit)
        {
            type = ComparisonType::GREATER_THAN;
            return true;
        }
        else if (this_typeInit && other_typeInit)
        {
            if (this->programme_type < prog_field_1->programme_type)
            {
                type = ComparisonType::LOWER_THAN;
                return true;
            }
            else if (this->programme_type > prog_field_1->programme_type)
            {
                type = ComparisonType::GREATER_THAN;
                return true;
            }
            else
            {
                type = ComparisonType::EQUAL_TO;
                return true;
            }
        }
        return false;
    }

    bool partialMatchProgramme(const string &other) const
    {
        if (other.empty())
            return false;

        string a = convertDegreeProgrammeToString(this->programme);
        string b = other;

        if (a.empty() || b.empty())
            return false;

        // lowercase both
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        // Check substring
        return substringMatch(a, b);
    }

    bool partialMatchProgrammeType(const string &other) const
    {
        if (other.empty())
            return false;

        string a = convertDegreeTypeToString(this->programme_type);
        string b = other;

        if (a.empty() || b.empty())
            return false;

        // lowercase both
        std::transform(a.begin(), a.end(), a.begin(), ::tolower);
        std::transform(b.begin(), b.end(), b.begin(), ::tolower);

        // Check substring
        return substringMatch(a, b);
    }

    bool generateRandom() override
    {
        bool success = generateRandomProg();
        success &= generateRandomDegType();
        return success;
    }

    bool generateRandomProg()
    {
        int max_prog = static_cast<int>(DegreeProgramme::PETR);
        int random_prog = (rand() % max_prog) + 1; // Random DegreeProgramme between CHEM and PETR (excluding NONE)
        programme = static_cast<DegreeProgramme>(random_prog);

        setProgrammeInitFlag();
        return true;
    }

    bool generateRandomDegType()
    {
        int max_type = static_cast<int>(DegreeType::MENG);
        int random_type = (rand() % max_type) + 1; // Random DegreeType between BEng and MEng (excluding NONE)
        programme_type = static_cast<DegreeType>(random_type);

        setProgrammeTypeInitFlag();
        return true;
    }
};
