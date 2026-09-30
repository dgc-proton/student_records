#include "../../FieldR4.h"

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

// Test random generation and comparison of EnrollmentYearField
void testFunctions_part0()
{
    cout << endl
         << endl
         << "Starting testFunctions_part0" << endl
         << endl;

    EnrollmentYearField yearField1;
    yearField1.generateRandom();
    int year1, cgs1;
    yearField1.getEnrollmentYear(year1);
    yearField1.getCGS(cgs1);

    cout << "Generated Enrollment Year: " << year1 << ", CGS: " << cgs1 << endl;

    EnrollmentYearField yearField2;
    yearField2.generateRandom();
    int year2, cgs2;
    yearField2.getEnrollmentYear(year2);
    yearField2.getCGS(cgs2);
    cout << "Generated Enrollment Year: " << year2 << ", CGS: " << cgs2 << endl;

    ComparisonType type;
    yearField1.compareEnrollmentYears(yearField2, type);
    if (type == ComparisonType::LOWER_THAN)
    {
        cout << "EnrollmentYearField1 is LOWER_THAN EnrollmentYearField2" << endl;
    }
    else if (type == ComparisonType::GREATER_THAN)
    {
        cout << "EnrollmentYearField1 is GREATER_THAN EnrollmentYearField2" << endl;
    }
    else
    {
        cout << "EnrollmentYearField1 is EQUAL_TO EnrollmentYearField2" << endl;
    }

    yearField1.compareCGS(yearField2, type);
    if (type == ComparisonType::LOWER_THAN)
    {
        cout << "CGS for YearField1 is LOWER_THAN YearField2" << endl;
    }
    else if (type == ComparisonType::GREATER_THAN)
    {
        cout << "CGS for YearField1 is GREATER_THAN YearField2" << endl;
    }
    else
    {
        cout << "CGS for YearField1 is EQUAL_TO YearField2" << endl;
    }
}

// Test setting, getting and partial comparison of EnrollmentYearField
void testFunctions_part1()
{
    cout << endl
         << endl
         << "Starting testFunctions_part1" << endl
         << endl;

    EnrollmentYearField yearField1;
    string error_message;
    yearField1.setEnrollmentYear(2020, error_message);
    yearField1.setCGS(18, error_message);

    int year1, cgs1;
    yearField1.getEnrollmentYear(year1);
    yearField1.getCGS(cgs1);
    cout << "Set Enrollment Year: " << year1 << ", CGS: " << cgs1 << endl;

    EnrollmentYearField yearField2;
    yearField2.setEnrollmentYear(2026, error_message);
    yearField2.setCGS(20, error_message);

    int year2, cgs2;
    yearField2.getEnrollmentYear(year2);
    yearField2.getCGS(cgs2);
    cout << "Set Enrollment Year: " << year2 << ", CGS: " << cgs2 << endl;

    ComparisonType type;
    yearField1.compareEnrollmentYears(yearField2, type);
    if (type == ComparisonType::LOWER_THAN)
    {
        cout << "EnrollmentYearField1 is LOWER_THAN EnrollmentYearField2" << endl;
    }
    else if (type == ComparisonType::GREATER_THAN)
    {
        cout << "EnrollmentYearField1 is GREATER_THAN EnrollmentYearField2" << endl;
    }
    else
    {
        cout << "EnrollmentYearField1 is EQUAL_TO EnrollmentYearField2" << endl;
    }

    yearField1.compareCGS(yearField2, type);
    if (type == ComparisonType::LOWER_THAN)
    {
        cout << "CGS for EnrollmentYearField1 is LOWER_THAN EnrollmentYearField2" << endl;
    }
    else if (type == ComparisonType::GREATER_THAN)
    {
        cout << "CGS for EnrollmentYearField1 is GREATER_THAN EnrollmentYearField2" << endl;
    }
    else
    {
        cout << "CGS for EnrollmentYearField1 is EQUAL_TO EnrollmentYearField2" << endl;
    }
}

void testFunctions_part2()
{
    cout << endl
         << endl
         << "Starting testFunctions_part2" << endl
         << endl;
    // create an array of EnrollmentYearField objects and find partial matches
    EnrollmentYearField yearFields[5];
    for (int i = 0; i < 5; ++i)
    {
        yearFields[i].generateRandom();
    }

    // Find partial matches that have number 1 in their CGS
    int year, cgs;
    cout << "Finding partial matches with CGS containing '1':" << endl;
    for (int i = 0; i < 5; ++i)
    {
        if (yearFields[i].partialMatchCGS(1))
        {
            yearFields[i].getEnrollmentYear(year);
            yearFields[i].getCGS(cgs);
            cout << "EnrollmentYearField " << year << " has a partial match of 1 and CGS. " << cgs << endl;
        }
    }

    cout << endl;
    cout << "Finding partial matches with Enrollment Year containing '20':" << endl;
    // Find partial matches (e.g., same Enrollment 20)
    for (int i = 0; i < 5; ++i)
    {
        if (yearFields[i].partialMatchEnrollmentYear(20))
        {
            yearFields[i].getEnrollmentYear(year);
            yearFields[i].getCGS(cgs);
            cout << "EnrollmentYearField " << year << " has a partial match in Enrollment Year and the CGS is " << cgs << endl;
        }
    }
}