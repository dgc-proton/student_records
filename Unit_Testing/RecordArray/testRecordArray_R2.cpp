#include "../../RecordArray.h"
#include "../../RecordvB_R2.h"
#include "../../RecordFileManager.h"

void testArrayFunctions_part0();
void testArrayFunctions_part1();
void testArrayFunctions_part2();

using namespace std;

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	testArrayFunctions_part0();
	testArrayFunctions_part1();
	testArrayFunctions_part2();
	return 0;
}

// Test the interaction between Record and RecordArray, where each Record tracks how many
// pointers exist to it and RecordArray will delete a record once the number of pointers
// to it reaches zero.
void testArrayFunctions_part0()
{
	cout << endl
		 << endl
		 << "Starting testArrayFunctions_part0" << endl
		 << endl;
	RecordArray<RecordvB> testArray(3);
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen(true);

	cout << "Tests checking that the Record included_in is adjusted correctly" << endl;
	{
		cout << endl
			 << "Create two shallow copies of the array" << endl;
		RecordArray<RecordvB> shallow_cp1(testArray, true);
		RecordArray<RecordvB> shallow_cp2(shallow_cp1, true);
		cout << "Print one of the shallow copies; records should have value 3 for 'included in'" << endl;
		testArray.printArrayOnScreen(true);

		cout << endl
			 << "Create a deep copy of the array then reprint original array; included_in should still be 3" << endl;
		RecordArray<RecordvB> deep_cp1(testArray, false);
		cout << "Original: Included in should be 3" << endl;
		testArray.printArrayOnScreen(true);
		cout << "Deep Copy: Included in should be 1" << endl;
		deep_cp1.printArrayOnScreen(true);
	}

	cout << "The copies have now gone out of scope; reprint the original array. Included in should be 1" << endl;
	testArray.printArrayOnScreen(true);
}


// Test RecordArray functionality: add record, edit record, random input, clearing, enter from keyboard, lock and
// delete.
void testArrayFunctions_part1()
{
	cout << endl
		 << endl
		 << "Starting testArrayFunctions_part1" << endl
		 << endl;
	int arraysize;

	cout << "Enter arraysize and hit enter: " << endl;
	cin >> arraysize;
	cout << endl;

	RecordArray<RecordvB> testArray(arraysize);

	cout << "Add a record using add and edit functionality, then randomly populate rest of array." << endl;
	testArray.addRecord(0);
	testArray.editRecord(0);
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;
	//
	cout << "Clearing array" << endl;
	testArray.clearArray();
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
	cout << endl;
	//
	cout << "Test input funciton:" << endl;
	testArray.enterArrayFromKeyboard();
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
	cout << endl;
	//
	cout << "Now try to clear and delete records with locked Array:" << endl;
	testArray.setArrayLocked(true);
	testArray.clearArray();
	testArray.deleteRecord(0);
	testArray.setArrayLocked(false);
	cout << endl;
	//
	cout << "Now delete record at index 0" << endl;
	testArray.deleteRecord(0);
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
	cout << "Now randomly fill the array" << endl;
	testArray.fillRandomValueArray();
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
	cout << endl;
	cout << "Done with testArrayFunctions_part1." << endl;
}


// Testing editing a record via argument, sort function and file writing.
void testArrayFunctions_part2()
{
	cout << endl
		 << endl
		 << "Starting testArrayFunctions_part2" << endl
		 << endl;
	int arraysize = 8;

	RecordArray<RecordvB> testArray(arraysize);

	cout << "Add containing different missing information, then fill the rest of the ";
	cout << " array randomly: " << endl;
	testArray.addRecord(0);
	testArray.getRecord(0)->setFirstName("Dannytest");
	testArray.getRecord(0)->setLastName("Tudor");
	testArray.getRecord(0)->setStudentID(51199999);
	testArray.getRecord(0)->setDegreeProgramme(DegreeProgramme::CHEM);
	testArray.addRecord(1);
	testArray.getRecord(1)->setFirstName("Dannytest");
	cout << "Should have an error saying record is locked." << endl;
	testArray.getRecord(1)->lock();
	testArray.getRecord(1)->setLastName("Tudor");
	testArray.getRecord(1)->unlock();
	testArray.getRecord(1)->setStudentID(51199999);
	testArray.getRecord(1)->setDegreeType(DegreeType::MENG);
	testArray.addRecord(2);
	testArray.getRecord(2)->setFirstName("Dannytest");
	testArray.getRecord(2)->setLastName("Tudor");
	testArray.getRecord(2)->setDegreeType(DegreeType::MENG);
	//testArray.addRecord(2);
	testArray.getRecord(2)->setLastName("Tudor");
	testArray.getRecord(2)->setStudentID(51000000);
	testArray.getRecord(2)->setDegreeType(DegreeType::BENG);
	testArray.addRecord(3);
	testArray.getRecord(3)->setStudentID(9100000);  // this is not a valid value
	testArray.getRecord(3)->setDegreeProgramme(DegreeProgramme::EEE);
	testArray.getRecord(3)->setDegreeType(DegreeType::BENG);
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	cout << "Done." << endl;

	SortCriterion criteria;

	criteria.ascending = true;
	criteria.field = RecordFields::DEGREE;
	cout << " Sort array by degree (Ascending) and print the result: " << endl;
	testArray.bubblesort(criteria);
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;

	criteria.ascending = false;
	criteria.field = RecordFields::DEGREE_TYPE;
	cout << " Sort array by degree type (Descending) and print the result: " << endl;
	testArray.bubblesort(criteria);
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;

	criteria.ascending = true;
	criteria.field = RecordFields::FAMILY_NAME;
	cout << " Sort array by family name (Ascending) and print the result: " << endl;
	testArray.bubblesort(criteria);
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;

	criteria.ascending = false;
	criteria.field = RecordFields::FIRST_NAME;
	cout << " Sort array by first name (Descending) and print the result: " << endl;
	testArray.bubblesort(criteria);
	testArray.printArrayOnScreen();
	cout << endl;

	criteria.ascending = true;
	criteria.field = RecordFields::STUDENT_ID;
	cout << " Sort array by student ID (Ascending) and print the result: " << endl;
	testArray.bubblesort(criteria);
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;

	cout << "Creating an array of size 8, populating randomly then deleting elements 0 and 2 to test handling of paritally empty array" << endl;
	RecordArray<RecordvB> testArray2(8);
	testArray2.fillRandomValueArray();
	testArray2.deleteRecord(0);
	testArray2.deleteRecord(2);
	cout << "Sorting array by family name (Descending)" << endl;
	criteria.field = RecordFields::FAMILY_NAME;
	criteria.ascending = false;
	testArray2.bubblesort(criteria);
	testArray2.printArrayOnScreen();
	testArray2.printArrayInfoOnScreen();


	// Test file writing
	cout << endl
		 << "Testing file writing..." << endl;

	bool success = testArray.writeAllToFile("test_records.txt");

	RecordArray<RecordvB> testArray1;
	testArray1.loadFromFile("test_records.txt");
	testArray1.printArrayOnScreen();

	testArray1.swapRecords(0, 1);
	testArray1.printArrayOnScreen();

	testArray1.writeAllToFile("test_records.txt");
	// testArray1.updateFile("test_records.txt", 0);

	testArray1.swapRecords(2, 3);

	// testArray1.updateFile("test_records.txt", 2);
	testArray1.printArrayOnScreen();

	testArray1.appendToFile("test_records.txt");
	testArray1.printArrayOnScreen();

	RecordArray<RecordvB> testArray3;
	success = testArray3.loadFromFile("test_records.txt");
	testArray3.printArrayOnScreen();
	testArray3.writeAllToFile("test_records_updated.txt");

	if (success)
	{
		cout << "SUCCESS: Records written to test_records.txt" << endl;
	}
	else
	{
		cout << "FAILED to write records to file" << endl;
	}

	cout << "Done with testArrayFunctions_part2." << endl;
}
