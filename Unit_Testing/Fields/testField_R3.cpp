#include "../../FieldR2.h"

void testFunctions_part0();
void testFunctions_part1();
void testFunctions_part2();

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));
    testFunctions_part0();
    testFunctions_part1();
    testFunctions_part2();
    return 0;
}

void testFunctions_part0()
{
    NameField nameField1("Clarkson", "Smith");
    NameField nameField2("Alicia", "Smythe");

    if (nameField1.partialMatchFirstName("Clark"))
    {
        cout << "First name partial match found for 'Clark' in 'Clarkson'" << endl;
    }
    else
    {
        cout << "No first name partial match found for 'Clark' in 'Clarkson'" << endl;
    }

    if (nameField2.partialMatchLastName("Smi"))
    {
        cout << "Last name partial match found for 'Smi' in 'Smythe'" << endl;
    }
    else
    {
        cout << "No last name partial match found for 'Smi' in 'Smythe'" << endl;
    }
}

void testFunctions_part1()
{
    StudentIdField idField1;
    string error_message;
    idField1.setStudentId(51234567, error_message);

    if (idField1.partialMatchStudentId(1234))
    {
        cout << "Partial match found for '1234' in Student ID '51234567'" << endl;
    }
    else
    {
        cout << "No partial match found for '1234' in Student ID '51234567'" << endl;
    }

    if (idField1.partialMatchStudentId(55))
    {
        cout << "Partial match found for '55' in Student ID '51234567'" << endl;
    }
    else
    {
        cout << "No partial match found for '55' in Student ID '51234567'" << endl;
    }
}

void testFunctions_part2()
{
    DegreeProgrammeField degreeField;
    degreeField.setProgramme(DegreeProgramme::EEE);
    degreeField.setProgrammeType(DegreeType::BENG);

    DegreeProgramme prog;
    degreeField.getProgramme(prog);
    if (degreeField.partialMatchProgramme("Chemical"))
    {
        cout << "Partial match found for 'Chemical' in Degree Programme '" << convertDegreeProgrammeToString(prog) << "'" << endl;
    }
    else
    {
        cout << "No partial match found for 'Chemical' in Degree Programme '" << convertDegreeProgrammeToString(prog) << "'" << endl;
    }

    DegreeType type;
    degreeField.getProgrammeType(type);
    if (degreeField.partialMatchProgrammeType("be"))
    {
        cout << "Partial match found for 'BEng' in Degree Type '" << convertDegreeTypeToString(type) << "'" << endl;
    }
    else
    {
        cout << "No partial match found for 'Master' in Degree Type " << convertDegreeTypeToString(type) << "'" << endl;
    }
}