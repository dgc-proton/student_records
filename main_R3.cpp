#include <iostream>
#include <stdexcept>
#include <chrono>
#include "RecordArray.h"

// Description from the project specification for "main.cpp" for release 3:
// 
// Implement a main program that:
// • Prompt user to input record size from keyboard; allocate array. Start with a small
//   array (say 5 entries) and test the input from keyboard functionality.
// • Prompt sorting criteria input (Main + Secondary) from keyboard; sorts the array and
//   print to screen the sorted array. Repeat for different sorting options.
// • Now assume a larger array size (say 50), generates random entries in the array;
//   prints to screen the input array.
// • Prompt sorting criteria from keyboard; sort the array and print to screen the sorted
//   array; repeat for all supported sorting options.
// Sorting criteria: any supported field can be used for sorting, either as primary or secondary, tertiary etc


void createAndSortKeyboard();
void createAndSortRandom();


int main()
{
    createAndSortKeyboard();
    createAndSortRandom();
    return 0;
}


void createAndSortKeyboard()
{
    long int array_size = -1;
    std::string response;
    SortCriterion sort_crit1, sort_crit2, sort_crit3;

    std::cout << "Enter a small array size (e.g. 5 entries) for testing manual Record data input: " << std::endl;
    while((!(std::cin >> array_size)) || (array_size < 0)) {
        std::cout << "Invalid input. Please enter a positive integer." << std::endl;
        std::cin.clear();
    }

    RecordArray<RecordvB> data_array(array_size);

    std::cout << std::endl << "Array created. Now enter the Record data." << std::endl;
    data_array.enterArrayFromKeyboard();
    
    std::cout << std::endl << "The array you entered was:" << std::endl;
    data_array.printArrayOnScreen();

    while(response != "exit") {
        response = "";
        std::cout << "Enter sorting criteria. The array will then be sorted and printed to screen." << std::endl;
        bool success = RecordvB::getSortCriteriaFromUser(sort_crit1, sort_crit2, sort_crit3);
        if (!success) {
            cout << "No valid inputs from user. Exiting." << endl;
            break;
        }
        data_array.bubblesort(sort_crit1, sort_crit2, sort_crit3);
        std::cout << "The sorted array is: " << std::endl;
        data_array.printArrayOnScreen();
        data_array.printArrayInfoOnScreen();
        std::cout << std::endl << "Type 'exit' to stop, or any key to re-sort the array: ";
        std::cin >> response;
    }

    return;
}


void createAndSortRandom()
{
    long int array_size = -1;
    std::string response;
    SortCriterion sort_crit1, sort_crit2, sort_crit3;

    std::cout << "Enter a large array size (e.g. 50 entries, or 50,000 or more if stress testing) for testing"
              <<  "random Record data generation and sorting of the resulting array: " << std::endl;
    while((!(std::cin >> array_size)) || (array_size < 0)) {
        std::cout << "Invalid input. Please enter a positive integer." << std::endl;
        std::cin.clear();
    }

    RecordArray<RecordvB> data_array(array_size);

    data_array.fillRandomValueArray();
    
    std::cout << std::endl << "The randomly generated Record array is:" << std::endl;
    data_array.printArrayOnScreen();

    while(response != "exit") {
        response = "";
        std::cout << "Enter sorting criteria. The array will then be sorted and printed to screen. "
                  << " The sorting operation will be timed." << std::endl;
        // throw std::runtime_error("TODO: implement entering sort criteria");
        bool success = RecordvB::getSortCriteriaFromUser(sort_crit1, sort_crit2, sort_crit3);
        if (!success) {
            cout << "No valid inputs from user. Exiting." << endl;
            break;
        }
        auto start = std::chrono::high_resolution_clock::now();  // start time
        data_array.bubblesort(sort_crit1, sort_crit2, sort_crit3);
        auto stop = std::chrono::high_resolution_clock::now();  // stop time
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "The sorted array is: " << std::endl;
        data_array.printArrayOnScreen();
        data_array.printArrayInfoOnScreen();
        std::cout << "The time taken to sort the array was: " << duration.count() << " milliseconds (ms)" << std::endl;
        std::cout << std::endl << "Type 'exit' to stop, or any key to re-sort the array: ";
        std::cin >> response;
    }

    return;
}
