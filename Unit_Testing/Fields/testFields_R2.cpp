#include "../../FieldR2.h"

void testFunctions_part0();
void testFunctions_part1();
void testFunctions_part2();

int main()
{
    srand(time(NULL));
    testFunctions_part0();
    testFunctions_part1();
    testFunctions_part2();
    return 0;
}

void testFunctions_part0()
{
    NameField nameField1("Alice", "Smith");
    NameField nameField2;
    NameField nameField3;

    nameField2.setFirstName("Bob");
    nameField2.setLastName("Johnson");

    nameField3.generateRandomFirstName();
    nameField3.generateRandomLastName();

    string firstName, lastName;

    nameField1.getFirstName(firstName);
    nameField1.getLastName(lastName);
    cout << "Name Field 1: " << firstName << " " << lastName << endl;

    nameField2.getFirstName(firstName);
    nameField2.getLastName(lastName);
    cout << "Name Field 2: " << firstName << " " << lastName << endl;

    nameField3.getFirstName(firstName);
    nameField3.getLastName(lastName);
    cout << "Name Field 3 (Random): " << firstName << " " << lastName << endl;

    ComparisonType comparison;
    nameField1.compareFirstNames(nameField2, comparison);
    if (comparison == ComparisonType::EQUAL_TO)
    {
        cout << "First names are equal." << endl;
    }
    else if (comparison == ComparisonType::LOWER_THAN)
    {
        cout << "First name 1 is less than first name 2." << endl;
    }
    else
    {
        cout << "First name 1 is greater than first name 2." << endl;
    }
}

void testFunctions_part1()
{
    StudentIdField idField1;
    StudentIdField idField2;
    StudentIdField idField3;

    string errorMsg;

    idField1.setStudentId(512345, errorMsg);
    if (!errorMsg.empty())
    {
        cout << "Error setting ID Field 1: " << errorMsg << endl;
    }

    idField2.setStudentId(520000, errorMsg);
    if (!errorMsg.empty())
    {
        cout << "Error setting ID Field 2: " << errorMsg << endl;
    }

    idField3.generateRandom();

    long int idValue;

    idField1.getStudentId(idValue);
    cout << "ID Field 1: " << idValue << endl;

    idField2.getStudentId(idValue);
    cout << "ID Field 2: " << idValue << endl;

    idField3.getStudentId(idValue);
    cout << "ID Field 3 (Random): " << idValue << endl;

    ComparisonType comparison;
    idField1.compare(idField2, comparison);
    if (comparison == ComparisonType::EQUAL_TO)
    {
        cout << "ID fields are equal." << endl;
    }
    else if (comparison == ComparisonType::LOWER_THAN)
    {
        cout << "ID Field 1 is less than ID Field 2." << endl;
    }
    else
    {
        cout << "ID Field 1 is greater than ID Field 2." << endl;
    }
}

void testFunctions_part2()
{
    DegreeProgrammeField progField1;
    DegreeProgrammeField progField2;
    DegreeProgrammeField progField3;

    progField1.setProgramme(DegreeProgramme::CHEM);
    progField1.setProgrammeType(DegreeType::BENG);

    progField2.setProgramme(DegreeProgramme::CIVIL);
    progField2.setProgrammeType(DegreeType::MENG);

    progField3.generateRandom();

    DegreeProgramme degreeProg;
    DegreeType degreeType;

    progField1.getProgramme(degreeProg);
    progField1.getProgrammeType(degreeType);
    cout << "Programme Field 1: " << convertDegreeProgrammeToString(degreeProg)
         << ", Type: " << convertDegreeTypeToString(degreeType) << endl;

    progField2.getProgramme(degreeProg);
    progField2.getProgrammeType(degreeType);
    cout << "Programme Field 2: " << convertDegreeProgrammeToString(degreeProg)
         << ", Type: " << convertDegreeTypeToString(degreeType) << endl;

    progField3.getProgramme(degreeProg);
    progField3.getProgrammeType(degreeType);
    cout << "Programme Field 3 (Random): " << convertDegreeProgrammeToString(degreeProg)
         << ", Type: " << convertDegreeTypeToString(degreeType) << endl;

    ComparisonType comparison;
    progField1.compareProgramme(progField2, comparison);
    if (comparison == ComparisonType::EQUAL_TO)
    {
        cout << "Degree programmes are equal." << endl;
    }
    else if (comparison == ComparisonType::LOWER_THAN)
    {
        cout << "Programme Field 1 is less than Programme Field 2." << endl;
    }
    else
    {
        cout << "Programme Field 1 is greater than Programme Field 2." << endl;
    }
}