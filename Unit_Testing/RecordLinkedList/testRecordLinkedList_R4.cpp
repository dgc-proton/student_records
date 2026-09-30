#include "../../RecordLinkedList.h"
// #include "../../RecordvB_R2.h"
#include <stdexcept>

void testListFunctions_part0();
void testListFunctions_part1();
void testListFunctions_part2();
void testListFunctions_part3();
void testListFunctions_part4();
void testListFunctions_part5();
void testListFunctions_part6();
void testListFunctions_part7();

using namespace std;

int main()
{
     srand(static_cast<unsigned int>(time(NULL)));
     // testListFunctions_part0();
     // testListFunctions_part1();
     // testListFunctions_part2();
     // testListFunctions_part3();
     testListFunctions_part4();
     // testListFunctions_part5();
     // testListFunctions_part6();
     // testListFunctions_part7();
     return 0;
}

// Test the interaction between Record and RecordLinkedList, where each Record tracks how many
// pointers exist to it and RecordLinkedList will delete a record once the number of pointers
// to it reaches zero.
void testListFunctions_part0()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part0" << endl
          << endl;
     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(5);
     testList.printListOnScreen(true);

     cout << "Tests checking that the Record included_in is adjusted correctly" << endl;
     {
          cout << endl
               << "Create two shallow copies of the List" << endl;
          RecordLinkedList<RecordvC> shallow_cp1(testList, true);
          RecordLinkedList<RecordvC> shallow_cp2(shallow_cp1, true);
          cout << "Print one of the shallow copies; records should have value 3 for 'included in'"
               << endl;
          testList.printListOnScreen(true);

          cout << endl
               << "Create a deep copy of the List then reprint original List; included_in should "
                  "still be 3"
               << endl;
          RecordLinkedList<RecordvC> deep_cp1(testList, false);
          cout << "Original: Included in should be 3" << endl;
          testList.printListOnScreen(true);
          cout << "Deep Copy: Included in should be 1" << endl;
          deep_cp1.printListOnScreen(true);
     }

     cout << "The copies have now gone out of scope; reprint the original List. Included in should "
             "be 1"
          << endl;
     testList.printListOnScreen(true);
}

// Test RecordList functionality: add record, edit record, random input, clearing, enter from
// keyboard, lock and delete.
void testListFunctions_part1()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part1" << endl
          << endl;

     RecordLinkedList<RecordvC> testList;

     cout << "Add a record using add and edit functionality, then randomly populate some records."
          << endl;
     testList.addRecord(0);
     testList.editRecord(0);
     testList.fillRandomValueList(4);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;
     //
     cout << "Clearing List" << endl;
     testList.clearList();
     testList.printListInfoOnScreen();
     testList.printListOnScreen();
     cout << endl;
     //
     cout << "Test input funciton:" << endl;
     testList.enterListFromKeyboard();
     testList.printListInfoOnScreen();
     testList.printListOnScreen();
     cout << endl;
     //
     cout << "Now delete record at index 0" << endl;
     testList.deleteRecord(0);
     testList.printListInfoOnScreen();
     testList.printListOnScreen();
     cout << "Now randomly fill the List" << endl;
     testList.fillRandomValueList(5);
     testList.printListInfoOnScreen();
     testList.printListOnScreen();
     cout << endl;
     cout << "Done with testListFunctions_part1." << endl;
}

// Testing editing a record via argument, sort function.
void testListFunctions_part2()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part2" << endl
          << endl;

     RecordLinkedList<RecordvC> testList;

     cout << "Adding Records containing different missing information, then fill the rest of the "
          << " List randomly: " << endl;
     testList.addRecord(0);
     testList.getRecord(0)->setFirstName("Dannytest");
     testList.getRecord(0)->setLastName("Tudor");
     testList.getRecord(0)->setStudentID(51199999);
     testList.getRecord(0)->setDegreeProgramme(DegreeProgramme::CHEM);
     testList.addRecord(0);
     testList.getRecord(0)->setFirstName("Dannytest");
     cout << "Should have an error saying record is locked." << endl;
     testList.getRecord(0)->lock();
     testList.getRecord(0)->setLastName("Tudor");
     testList.getRecord(0)->unlock();
     testList.getRecord(0)->setStudentID(51199999);
     testList.getRecord(0)->setDegreeType(DegreeType::MENG);
     testList.addRecord(0);
     testList.getRecord(0)->setFirstName("Dannytest");
     testList.getRecord(0)->setLastName("Tudor");
     testList.getRecord(0)->setDegreeType(DegreeType::MENG);
     testList.getRecord(0)->setLastName("Tudor");
     testList.getRecord(0)->setStudentID(51000000);
     testList.getRecord(0)->setDegreeType(DegreeType::BENG);
     testList.addRecord(0);
     cout << "Should have an error saying invalid student ID." << endl;
     testList.getRecord(0)->setStudentID(9100000); // this is not a valid value
     testList.getRecord(0)->setDegreeProgramme(DegreeProgramme::EEE);
     testList.getRecord(0)->setDegreeType(DegreeType::BENG);
     testList.fillRandomValueList(3);
     testList.printListOnScreen(true);
     cout << "Done." << endl;

     SortCriterion criteria;

     criteria.ascending = true;
     criteria.field = RecordFields::DEGREE;
     cout << " Sort List by degree (Ascending) and print the result: " << endl;
     testList.mergeSort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = false;
     criteria.field = RecordFields::DEGREE_TYPE;
     cout << " Sort List by degree type (Descending) and print the result: " << endl;
     testList.mergeSort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = true;
     criteria.field = RecordFields::FAMILY_NAME;
     cout << " Sort List by family name (Ascending) and print the result: " << endl;
     testList.mergeSort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = false;
     criteria.field = RecordFields::FIRST_NAME;
     cout << " Sort List by first name (Descending) and print the result: " << endl;
     testList.mergeSort(criteria);
     testList.printListOnScreen();
     cout << endl;

     criteria.ascending = true;
     criteria.field = RecordFields::STUDENT_ID;
     cout << " Sort List by student ID (Ascending) and print the result: " << endl;
     testList.mergeSort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     cout << "Creating an List of size 8, populating randomly then deleting elements 0 and 2 to "
             "test handling of paritally empty List"
          << endl;
     RecordLinkedList<RecordvC> testList2;
     cout << "Filling list with random values" << endl;
     cout << "Printing List" << endl;
     testList2.printListOnScreen();
     testList2.fillRandomValueList(8);

     cout << "Deleting Record 0:" << endl;
     testList2.getRecord(0)->printInfo();
     cout << endl;

     testList2.deleteRecord(0);

     cout << "Deleting Record 2:" << endl;
     testList2.getRecord(2)->printInfo();
     cout << endl;

     testList2.deleteRecord(2);

     cout << "Sorting List by average CGS (Descending)" << endl;
     criteria.field = RecordFields::CGS;
     criteria.ascending = false;
     testList2.mergeSort(criteria);
     testList2.printListOnScreen();
     testList2.printListInfoOnScreen();

     cout << endl
          << "***** Now repeating sorting tests using bubblesort *****" << endl
          << endl;

     criteria.ascending = true;
     criteria.field = RecordFields::DEGREE;
     cout << " Sort List by degree (Ascending) and print the result: " << endl;
     testList.bubblesort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = false;
     criteria.field = RecordFields::DEGREE_TYPE;
     cout << " Sort List by degree type (Descending) and print the result: " << endl;
     testList.bubblesort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = true;
     criteria.field = RecordFields::FAMILY_NAME;
     cout << " Sort List by family name (Ascending) and print the result: " << endl;
     testList.bubblesort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     criteria.ascending = false;
     criteria.field = RecordFields::FIRST_NAME;
     cout << " Sort List by first name (Descending) and print the result: " << endl;
     testList.bubblesort(criteria);
     testList.printListOnScreen();
     cout << endl;

     criteria.ascending = true;
     criteria.field = RecordFields::STUDENT_ID;
     cout << " Sort List by student ID (Ascending) and print the result: " << endl;
     testList.bubblesort(criteria);
     testList.printListOnScreen();
     testList.printListInfoOnScreen();
     cout << endl;

     cout << "Creating an List of size 8, populating randomly then deleting elements 0 and 2 to "
             "test handling of paritally empty List"
          << endl;
     RecordLinkedList<RecordvC> testList3;
     cout << "Filling list with random values" << endl;
     cout << "Printing List" << endl;
     testList3.printListOnScreen();
     testList3.fillRandomValueList(8);

     cout << "Deleting Record 0:" << endl;
     testList3.getRecord(0)->printInfo();
     cout << endl;

     testList3.deleteRecord(0);

     cout << "Deleting Record 2:" << endl;
     testList3.getRecord(2)->printInfo();
     cout << endl;

     testList3.deleteRecord(2);

     cout << "Sorting List by average CGS (Descending)" << endl;
     criteria.field = RecordFields::CGS;
     criteria.ascending = false;
     testList3.bubblesort(criteria);
     testList3.printListOnScreen();
     testList3.printListInfoOnScreen();

     cout << "Done with testListFunctions_part2." << endl;
}

// Test simple search for specific Family Name
void testListFunctions_part3()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part3" << endl
          << endl;

     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(20);
     testList.mergeSort(SortCriterion{RecordFields::FAMILY_NAME, true});
     testList.printListOnScreen();

     cout << "Testing simple search on list for specific family name:" << endl;

     string family_name_to_find = "Smith";
     SortCriterion criteria;
     criteria.field = RecordFields::FAMILY_NAME;
     RecordvC record;
     string error_msg;
     record.setLastName(family_name_to_find);
     RecordLinkedList<RecordvC> found_record = testList.simpleSearchList(record, criteria);
     found_record.printListOnScreen();

     cout << "Done with testListFunctions_part3." << endl;
}

// Test advanced search for range of Family Names
void testListFunctions_part4()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part4" << endl
          << endl;

     // Your test code here
     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(20);
     testList.mergeSort(SortCriterion{RecordFields::FAMILY_NAME, true});
     testList.printListOnScreen();

     cout << "Testing advanced search on list for range of student family names:" << endl;

     string family_name_min = "J";
     string family_name_max = "N";
     SortCriterion criteria;
     criteria.field = RecordFields::FAMILY_NAME;
     criteria.ascending = true;
     RecordvC *record_min = new RecordvC();
     RecordvC *record_max = new RecordvC();
     string error_msg;
     record_min->setLastName(family_name_min);
     record_max->setLastName(family_name_max);
     RecordLinkedList<RecordvC> found_records = testList.complexSearchList(record_min, record_max, criteria);
     found_records.printListOnScreen();

     cout << "Done with testListFunctions_part4." << endl;
}

// Test advanced search for range of Degree Programmes
void testListFunctions_part5(){
     cout << endl
          << endl
          << "Starting testListFunctions_part5" << endl
          << endl;

     // Your test code here
     RecordvC *record_min = new RecordvC();
     RecordvC *record_max = new RecordvC();
     string error_msg;
     record_min->setDegreeProgramme(DegreeProgramme::CHEM);
     record_max->setDegreeProgramme(DegreeProgramme::MECH);

     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(20);
     testList.mergeSort(SortCriterion{RecordFields::DEGREE, true});
     testList.printListOnScreen();

     cout << "Testing advanced search on list for range of student Degree Programmes:" << endl;

     SortCriterion criteria;
     criteria.field = RecordFields::DEGREE;
     criteria.ascending = true;
     RecordLinkedList<RecordvC> found_records = testList.complexSearchList(record_min, record_max, criteria);
     found_records.printListOnScreen();

     cout << "Done with testListFunctions_part5." << endl;
}

// Test advanced search for range of enrollment years
void testListFunctions_part6()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part6" << endl
          << endl;

     RecordvC *record_min = new RecordvC();
     RecordvC *record_max = new RecordvC();
     string error_msg;
     record_min->setRecord(error_msg);
     record_max->setRecord(error_msg);

     // Your test code here
     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(20);
     testList.mergeSort(SortCriterion{RecordFields::FAMILY_NAME, true});
     testList.printListOnScreen();

     cout << "Testing advanced search on list for range of student Enrollment Years:" << endl;

     SortCriterion criteria;
     criteria.field = RecordFields::ENROLLMENT_YEAR;
     criteria.ascending = true;
     RecordLinkedList<RecordvC> found_records = testList.complexSearchList(record_min, record_max, criteria);
     found_records.printListOnScreen();

     cout << "Done with testListFunctions_part6." << endl;
}

// Test advanced search for range of CGS values
void testListFunctions_part7()
{
     cout << endl
          << endl
          << "Starting testListFunctions_part7" << endl
          << endl;

     // Your test code here
     RecordvC *record_min = new RecordvC();
     RecordvC *record_max = new RecordvC();
     string error_msg;
     record_min->setRecord(error_msg);
     record_max->setRecord(error_msg);

     RecordLinkedList<RecordvC> testList;
     testList.fillRandomValueList(20);
     testList.mergeSort(SortCriterion{RecordFields::CGS, true});
     testList.printListOnScreen();

     cout << "Testing advanced search on list for range of student CGS values:" << endl;
     SortCriterion criteria;
     criteria.field = RecordFields::CGS;
     criteria.ascending = true;
     RecordLinkedList<RecordvC> found_records = testList.complexSearchList(record_min, record_max, criteria);
     found_records.printListOnScreen();
     cout << "Done with testListFunctions_part7." << endl;
}
