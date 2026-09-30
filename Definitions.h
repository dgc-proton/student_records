#pragma once
#include <iostream>
#include <string>
using namespace std;

/**********************************************************************************************************
Since C++98, the meaning of inline keyword changed to mean "multiple definitions are permitted" rather than
to suggest inline expansion to the compiler.

In case the program runs out of stack/heap space, we don't want the stack consequences of a function call
and prefer to have an inline expanded function.
The following defines a macro FORCE_INLINE, which passes a compiler directive to inline substitute a
function call.

Used for functions like bailout().

For more info see:
https://en.cppreference.com/w/cpp/language/inline.html
https://meghprkh.github.io/blog/posts/c++-force-inline/
**********************************************************************************************************/
#if defined(__clang__)
#define FORCE_INLINE [[gnu::always_inline]] [[gnu::gnu_inline]] extern inline

#elif defined(__GNUC__)
#define FORCE_INLINE [[gnu::always_inline]] inline

#elif defined(_MSC_VER)
#pragma warning(error : 4714)
#define FORCE_INLINE __forceinline

#else
#error Unsupported compiler
#endif
/*********************************************************************************************************/

enum class ComparisonType
{
    NONE = 0,
    LOWER_THAN,
    EQUAL_TO,
    GREATER_THAN,
    NOT_COMPARABLE
};

inline string convertComparisonTypeToString(ComparisonType type)
{
    switch (type)
    {
    case ComparisonType::NONE:
        return "None";
    case ComparisonType::LOWER_THAN:
        return "Lower Than";
    case ComparisonType::EQUAL_TO:
        return "Equal To";
    case ComparisonType::GREATER_THAN:
        return "Greater Than";
    case ComparisonType::NOT_COMPARABLE:
        return "Not Comparable";
    default:
        return "Unknown";
    }
}

enum class DegreeProgramme
{
    NONE = 0,
    CHEM,
    CIVIL,
    EEE,
    MECH,
    PETR,
};

inline string convertDegreeProgrammeToString(DegreeProgramme programme)
{
    switch (programme)
    {
    case DegreeProgramme::NONE:
        return "None";
    case DegreeProgramme::CHEM:
        return "Chemical Engineering";
    case DegreeProgramme::CIVIL:
        return "Civil Engineering";
    case DegreeProgramme::EEE:
        return "Electrical and Electronic Engineering";
    case DegreeProgramme::MECH:
        return "Mechanical Engineering";
    case DegreeProgramme::PETR:
        return "Petroleum Engineering";
    default:
        return "Unknown";
    }
}

enum class DegreeType
{
    NONE = 0,
    BENG,
    MENG,
};

inline string convertDegreeTypeToString(DegreeType type)
{
    switch (type)
    {
    case DegreeType::NONE:
        return "None";
    case DegreeType::BENG:
        return "BEng";
    case DegreeType::MENG:
        return "MEng";
    default:
        return "Unknown";
    }
}

FORCE_INLINE void bailout()
{
    throw runtime_error("Unexpected Error. Bailing Out.");
}

enum class RecordFields
{
    UNSORTED = -1,
    START = 0,
    INT_W_LIMITS,
    RECORDvA_STOP,
    FIRST_NAME,
    FAMILY_NAME,
    STUDENT_ID,
    DEGREE,
    DEGREE_TYPE,
    RECORDvB_STOP,
    ENROLLMENT_YEAR,
    CGS,
    RECORDvC_STOP
};

inline string convertRecordFieldToString(RecordFields field)
{
    switch (field)
    {
    case RecordFields::UNSORTED:
        return "Unsorted";
    case RecordFields::INT_W_LIMITS:
        return "Integer With Limits";
    case RecordFields::FIRST_NAME:
        return "First Name";
    case RecordFields::FAMILY_NAME:
        return "Family Name";
    case RecordFields::STUDENT_ID:
        return "Student ID";
    case RecordFields::DEGREE:
        return "Degree Programme";
    case RecordFields::DEGREE_TYPE:
        return "Degree Type";
    case RecordFields::ENROLLMENT_YEAR:
        return "Enrollment Year";
        break;
    case RecordFields::CGS:
        return "Average CGS";
        break;
    default:
        return "Unknown Field";
    }
}

enum class RecordValidity
{
    INVALID = 0,
    PARTIALLY_VALID,
    VALID
};

struct SortCriterion
{
    RecordFields field; // which field
    bool ascending;     // true = ascending, false = descending
};

inline string convertSortCriterionToString(SortCriterion criterion)
{
    string msg = "";
    msg.append(convertRecordFieldToString(criterion.field));
    if (criterion.ascending && criterion.field != RecordFields::UNSORTED)
    {
        msg.append(" (ascending)");
    }
    else if (criterion.field != RecordFields::UNSORTED)
    {
        msg.append(" (descending)");
    }

    return msg;
}
