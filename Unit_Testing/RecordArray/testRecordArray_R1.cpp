#include "../../RecordArray.h"
#include "../../RecordvA_R1.h"

void testArrayFunctions_part0();
void testArrayFunctions_part1();
void testArrayFunctions_part2();

using namespace std;


int main()
{
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
	cout << endl << endl << "Starting testArrayFunctions_part0" << endl << endl;
	cout << "Random Fill Records" << endl;
	RecordArray<RecordvA> testArray(3);
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen(true);

	cout << "Tests checking that the Record included_in is adjusted correctly" << endl;
	{
		cout << endl << "Create two shallow copies of the array" << endl;
		RecordArray<RecordvA> shallow_cp1(testArray, true);
		RecordArray<RecordvA> shallow_cp2(shallow_cp1, true);
		cout << "Print one of the shallow copies; records should have value 3 for 'included in'" << endl;
		testArray.printArrayOnScreen(true);
	
		cout << endl << "Create a deep copy of the array then reprint original array; included_in should still be 3" << endl;
		RecordArray<RecordvA> deep_cp1(testArray, false);
		cout << "Original: Included in should be 3" << endl;
		testArray.printArrayOnScreen(true);
		cout << "Deep Copy: Included in should be 1" << endl;
		deep_cp1.printArrayOnScreen(true);
	}

	cout << "The copies have now gone out of scope; reprint the original array. Included in should be 1" << endl;
	testArray.printArrayOnScreen(true);
}


// Test RecordArray random input function, clearing function, enter from keyboard function lock function and delete
// function.
void testArrayFunctions_part1()
{
	cout << endl << endl << "Starting testArrayFunctions_part1" << endl << endl;
	int arraysize;

	cout << "Enter arraysize and hit enter: " << endl;
	cin >> arraysize;
	cout << endl;

	RecordArray<RecordvA> testArray(arraysize);

	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << endl;
	// 
	cout << "Test random input funciton:" << endl;
	testArray.fillRandomValueArray();
	testArray.printArrayInfoOnScreen();
	testArray.printArrayOnScreen();
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
}


// Test sort function.
void testArrayFunctions_part2()
{
	cout << endl << endl << "Starting testArrayFunctions_part2" << endl << endl;
	int arraysize;

	cout << "Enter arraysize and hit enter: " << endl;
	cin >> arraysize;
	cout << endl;

	RecordArray<RecordvA> testArray(arraysize);
	SortCriterion sortCrit;
	sortCrit.ascending = true;
	sortCrit.field = RecordFields::INT_W_LIMITS;

	cout << " Fill array randomly: " << endl;
	testArray.fillRandomValueArray();
	testArray.printArrayOnScreen();
	cout << "Done." << endl;

	cout << " Sort array and print the result: " << endl;
	testArray.bubblesort(sortCrit);
	testArray.printArrayOnScreen();
	testArray.printArrayInfoOnScreen();
	cout << "Done." << endl;
}

