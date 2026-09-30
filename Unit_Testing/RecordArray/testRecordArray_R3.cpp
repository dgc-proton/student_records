#include "../../RecordArray.h"
#include "../../RecordFileManager.h"

void testArrayFunctions_part0();
void testArrayFunctions_part1();

using namespace std;

int main()
{
    //srand(static_cast<unsigned int>(time(NULL)));

    testArrayFunctions_part0();
    testArrayFunctions_part1();
    return 0;
}

void testArrayFunctions_part0()
{
    RecordArray<RecordvB> testArray(20);
    testArray.fillRandomValueArray();
    testArray.writeAllToFile("test_records_part3_unsorted.txt");

    testArray.bubblesort(
        SortCriterion{RecordFields::FAMILY_NAME, true},
        SortCriterion{RecordFields::FIRST_NAME, true},
        SortCriterion{RecordFields::STUDENT_ID, true});

    testArray.writeAllToFile("test_records_part3_sorted_1.txt");

    SortCriterion criteria1, criteria2, criteria3;
    cout << "Sort by user choice" << endl;
    if (RecordvB::getSortCriteriaFromUser(criteria1, criteria2, criteria3)) {
        testArray.bubblesort(criteria1, criteria2, criteria3);
    } else {
        cout << "No valid input from user" << endl;
    }

    testArray.writeAllToFile("test_records_part3_sorted_2.txt");

    RecordArray<RecordvB> testArray2(20);
    testArray2.loadFromFile("test_records_part3_sorted_2.txt");

    testArray2.bubblesort(
        SortCriterion{RecordFields::FIRST_NAME, true},
        SortCriterion{RecordFields::DEGREE_TYPE, true},
        SortCriterion{RecordFields::STUDENT_ID, false});

    testArray2.writeAllToFile("test_records_part3_sorted_3.txt");

    testArray2.bubblesort(
        SortCriterion{RecordFields::DEGREE, true});

    testArray2.writeAllToFile("test_records_part3_sorted_4.txt");

    testArray2.bubblesort(
        SortCriterion{RecordFields::DEGREE, true}, 
        SortCriterion{RecordFields::DEGREE_TYPE, false}
    );

    testArray2.writeAllToFile("test_records_part3_sorted_5.txt");
}

void testArrayFunctions_part1() {
    RecordArray<RecordvA> testArray(20);
    testArray.fillRandomValueArray();

    cout << "Sorting by integer value ascending" << endl;
    testArray.bubblesort(
        SortCriterion{RecordFields::INT_W_LIMITS, true},
        SortCriterion{RecordFields::FIRST_NAME, true},
        SortCriterion{RecordFields::STUDENT_ID, true});

    testArray.printArrayOnScreen();
    testArray.printArrayInfoOnScreen();

    cout << "Sorting by integer value descending" << endl;
    testArray.bubblesort(
        SortCriterion{RecordFields::INT_W_LIMITS, false});

    testArray.printArrayOnScreen();
    testArray.printArrayInfoOnScreen();

    RecordArray<RecordvA> testArray2(testArray);

    cout << "Sorting by integer value ascending" << endl;
    testArray2.bubblesort(
        SortCriterion{RecordFields::INT_W_LIMITS, true},
        SortCriterion{RecordFields::DEGREE_TYPE, true},
        SortCriterion{RecordFields::STUDENT_ID, false});

    testArray2.printArrayOnScreen();
    testArray2.printArrayInfoOnScreen();
}
