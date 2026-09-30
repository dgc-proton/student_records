#pragma once

#include <algorithm>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "Definitions.h"
#include "Record.h"
#include "RecordFileManager.h"
#include "RecordvA_R1.h"
#include "RecordvB_R2.h"

// This class holds an array which contains pointers to Records. It handles memory allocation and
// deallocation for the records and arrays. It is a template designed to work only with classes
// derived from the Record class. Example of use: RecordArray<RecordChildClass> name_of_object();
template<class RecordType>
class RecordArray
{
  protected:
    RecordType** record_array = nullptr; // points to an array of pointers which point to Records
    int max_size;                        // max size of record_array
    int current_size = 0;                // current size of record_array
    int current_free_index = 0;          // -1 means array full
    SortCriterion sorted_by = SortCriterion{ RecordFields::UNSORTED, true };
    bool array_locked = false;           // locking the array prevents editing
    static const int default_size = 512; // default array size
    RecordFileManager<RecordType> file_manager;

    // Find the next free index in the array. If array is full will set to -1. Returns false if it
    // fails (the array being full is *not* a failure, it will set current_free_index to -1 and
    // return true).
    bool findNextFree()
    {
        // lambda function to search for next free index in array
        auto arrSearch = [&] {
            for (int i = 0; i < max_size; i++) {
                if (record_array[i] == nullptr) {
                    return i;
                }
            }
            // should never reach the next line
            throw std::runtime_error("RecordArray::findNextFree lambda function arrSearch failed");
        };

        if (current_size >= max_size) {
            // the array is full
            current_free_index = -1;
            return true;
        }

        // the current index is at the end of the array, so search the array for the next free
        // location
        if (current_free_index >= (max_size - 1)) {
            current_free_index = arrSearch();
            return true;
        }

        // if the next index in the array is free, then use that
        if (record_array[current_free_index + 1] == nullptr) {
            current_free_index++;
            return true;
        } else {
            // need to search for a free index
            current_free_index = arrSearch();
            return true;
        }

        return false; // a problem has occured with the logic
    }

    // Create the array to hold records.
    RecordType** createArray(int array_size)
    {
        std::exception_ptr eptr = nullptr; // used for catching exceptions
        RecordType** new_ptr = nullptr;

        // check that the record_array is not already allocated
        if (record_array != nullptr) {
            std::cout << "RecordArray::createArray called when array already exists. ";
            std::cout << "New array not created." << std::endl;
            return record_array;
        }

        // allocate memory for the arrays
        try {
            new_ptr = new RecordType*[array_size];
        } catch (std::bad_alloc& ex) {
            // if memory allocation fails then catch the error, carry out essential tasks then
            // rethrow the error
            eptr = std::current_exception();
            std::cout << "Request failed for memory to create arrays to store " << array_size;
            std::cout << " records. Details for bad_alloc:" << ex.what() << std::endl;
            // TODO add code to save current state?
            throw; // rethrow the error to unwind stack and terminate program
        }

        std::fill(new_ptr, new_ptr + array_size, nullptr); // set all pointers to NULL

        return new_ptr;
    }

  public:
    // Default constructor.
    RecordArray(int size = default_size)
    {
        // check size is valid
        if (size < 1) {
            std::cout << "Array size of " << size << "is invalid. Array of size ";
            std::cout << default_size << " will be created instead." << std::endl;
            size = default_size;
        }

        max_size = size;
        record_array = createArray(size);
    }

    // Copy constructor.
    RecordArray(const RecordArray& original, bool shallow_copy = true, int increase_size_by = 0)
    {
        // check that not being asked to decrease size of new array
        if (increase_size_by < 0) {
            std::cout << "Error: it is not possible to copy an array and have the new array "
                      << "be smaller than the original. Will copy to same sized array.";
            increase_size_by = 0;
        }

        // copy attributes of original RecordArray
        max_size = original.max_size + increase_size_by;
        sorted_by = original.sorted_by;

        // create array
        record_array = createArray(max_size);

        // if shallow copy then copy pointers
        if (shallow_copy) {
            for (int i = 0; i < original.max_size; i++) {
                record_array[i] = original.record_array[i]; // copy the pointer
                RecordAttorney::incrementIncludeCounter(
                      *original.record_array[i]); // increment included in counter for the record
            }
            current_size = original.current_size; // only needed for shallow copy, addRecord()
                                                  // manages this for deep copy
        }

        // if deep copy then create all new records using record copy constructor
        if (!shallow_copy) {
            for (int i = 0; i < original.max_size; i++) {
                addRecord(i, true, original.record_array[i]);
            }
        }

        current_free_index = findNextFree();
    }

    // Destructor.
    ~RecordArray()
    {
        for (int i = 0; i < max_size; i++) {
            // decrement the Record includedIn counter because this array will no longer hold a
            // pointer to it
            if (record_array[i] != nullptr) {
                RecordAttorney::decrementIncludeCounter(*record_array[i]);
                if (RecordAttorney::getIncludeCounter(*record_array[i]) < 1) {
                    // nothing holds a pointer to the Record anymore, free the memory
                    delete record_array[i];
                }
            }
        }

        delete[] record_array; // deallocate memory for the array
    }

    // Disable copy operator so that only deep or shallow copy methods are used.
    void operator=(RecordArray const& RecordArray) = delete;
    // Disable comparison operators
    void operator==(const RecordArray& RecordArray) = delete;
    void operator!=(const RecordArray& RecordArray) = delete;
    void operator<(const RecordArray& RecordArray) = delete;
    void operator>(const RecordArray& RecordArray) = delete;
    void operator<=(const RecordArray& RecordArray) = delete;
    void operator>=(const RecordArray& RecordArray) = delete;

    // Adds a record to the array. Returns false if the record cannot be added. The record will
    // either be added as a deep copy of the record provided, or as a new record where only the
    // constructor has been called.
    bool addRecord(int new_record_index, bool deep_copy = false, RecordType* record_ptr = nullptr)
    {
        std::exception_ptr eptr = nullptr; // used for catching exceptions
        bool success = false;              // used for checking function return values
        RecordType* new_record_ptr = nullptr;

        // check if the array is locked
        if (array_locked) {
            std::cout << "Error adding record; the record array is locked." << std::endl;
            return false;
        }

        // check that there is room in the array
        if (current_size >= max_size || current_free_index < 0) {
            std::cout << "Error adding record; the record array is full." << std::endl;
            return false;
        }

        // check that the specified index is NULL
        if (record_array[new_record_index] != nullptr) {
            std::cout << "Error adding record; the specified record array index is "
                         "populated."
                      << std::endl;
            return false;
        }

        // allocate memory for the new record
        try {
            if (deep_copy) {
                // this record is to be a copy of an existing record
                new_record_ptr = new RecordType(*record_ptr);
            } else {
                // this record will be populated with new data
                new_record_ptr = new RecordType();
            }
        } catch (std::bad_alloc& ex) {
            // if memory allocation fails then catch the error, carry out essential
            // tasks then rethrow the error
            eptr = std::current_exception();
            std::cout << "Request failed for memory to create a new record. ";
            std::cout << "Details for bad_alloc:" << ex.what() << std::endl;
            // TODO add code to save current state?
            throw; // rethrow the error to unwind stack and terminate program
        }

        // memory for the new record has been sucessfully allocated
        RecordAttorney::incrementIncludeCounter(
              *new_record_ptr); // increment included in counter for the record
        record_array[new_record_index] = new_record_ptr;
        current_size++;
        sorted_by = { RecordFields::UNSORTED, true };
        success = findNextFree(); // increment the array current_free_index
        if (!success) {
            throw std::runtime_error("RecordArray::findNextFree() returned false to "
                                     "addRecord, indicating a logic failure");
        }

        return true;
    }

    // Edit an existing record.
    bool editRecord(int record_index, bool set_all = false, bool randomise = false)
    {
        std::string msg_holder = "";
        bool success;

        // check if the array is locked
        if (array_locked) {
            std::cout << "Error editing record; the record array is locked." << std::endl;
            return false;
        }

        // check that the specified index is not NULL
        if (record_array[record_index] == nullptr) {
            std::cout << "Error editing record; the specified record array index "
                         "does not hold an associated record."
                      << std::endl;
            return false;
        }

        // editing a record means the array will no longer be sorted
        sorted_by = { RecordFields::UNSORTED, true };

        if (set_all) {
            success = record_array[record_index]->setRecord(msg_holder, randomise);
        } else {
            success = record_array[record_index]->editRecord(msg_holder, randomise);
        }

        if (!success) {
            std::cout << "Error returned by Record when attempting to edit / set: " << msg_holder
                      << std::endl;
        }

        return success;
    }

    // Add manually entered records to the array, either until it is full or
    // until the user types "exit".
    bool enterArrayFromKeyboard()
    {
        bool success = false; // used for checking function return values
        std::string option;
        int new_index;
        std::string error_msg;

        while (true) {
            // check that there is space in the array
            if (current_free_index < 0) {
                std::cout << "Array full, cannot enter another record from keyboard" << std::endl;
                break;
            }
            // add the record
            new_index = current_free_index;
            success = addRecord(new_index);
            if (!success) {
                std::cout << "An error occured adding that last record. The record was not added."
                          << std::endl;
                return false;
            }
            // populate the record
            error_msg = "";
            record_array[new_index]->setRecord(error_msg, false);
            if (!error_msg.empty()) {
                // an error occured (should only happen if record is locked, which will not be the
                // case here)
                std::cout << error_msg << std::endl;
            }
            // check if user wants to add the next record
            while (true) {
                std::cout << "Enter another record? [y|n]";
                std::cin >> option;
                std::transform(option.begin(),
                               option.end(),
                               option.begin(),
                               ::tolower); // lowercase the input
                if (option == "y" || option == "n") {
                    break;
                }
            }
            if (option == "n") {
                break;
            }
        }

        return true;
    }

    // Fill all empty elements of the array with random records.
    bool fillRandomValueArray()
    {
        bool success = false; // used for checking function return values
        int new_index;
        std::string error_msg;

        // check that there is space in the array for at least one additional record
        if (current_free_index < 0) {
            std::cout << "Array is full, cannot enter any random records" << std::endl;
            return false;
        }

        // fill the array
        while (true) {
            // check that there is still space in the array
            if (current_free_index < 0) {
                break;
            }
            // add the record
            new_index = current_free_index;
            success = addRecord(new_index);
            if (!success) {
                std::cout << "An error occured adding a random record." << std::endl;
                return false;
            }
            // randomly populate the new record
            error_msg = "";
            record_array[new_index]->setRecord(error_msg, true);
            if (!error_msg.empty()) {
                // an error occured (should only happen if record is locked, which will not be the
                // case here)
                std::cout << error_msg << std::endl;
            }
        }

        return true;
    }

    bool deleteRecord(int rec_index)
    {
        // check if the array is locked
        if (array_locked) {
            std::cout << "Error deleting record; the record array is locked." << std::endl;
            return false;
        }
        // check that the index is valid
        if (!isValidRecord(rec_index)) {
            std::cout << "index supplied to RecordArray::deleteRecord (" << rec_index;
            std::cout << ") is invalid. Record not deleted" << std::endl;
            return false;
        }

        // decrement the Record includedIn counter because this array will no longer hold a pointer
        // to it
        RecordAttorney::decrementIncludeCounter(*record_array[rec_index]);
        if (RecordAttorney::getIncludeCounter(*record_array[rec_index]) < 1) {
            // nothing holds a pointer to the Record anymore, free the memory
            delete record_array[rec_index];
        }

        record_array[rec_index] = nullptr; // remove the pointer to the record
        current_size--;

        // deleting a record introduces a nullptr in the middle of the array, so although the array
        // elements will still be in the same order they were, if they were sorted they will not be
        // in the same state where they are sorted with no nullptr inbetween, so set the array to
        // unsorted status
        sorted_by = { RecordFields::UNSORTED, true };

        if (current_free_index < 0) {
            // the current_free_index was set when array was full; it is no longer full
            current_free_index = rec_index;
        }
        return true;
    }

    // Delete all entries in the array. Returns true if successful.
    bool clearArray()
    {
        // check if the array is locked
        if (array_locked) {
            std::cout << "Error adding record; the record array is locked." << std::endl;
            return false;
        }

        // if guard clause passes then clear the array
        for (int i = 0; i < max_size; i++) {
            if (record_array[i] != nullptr) {
                deleteRecord(i);
            }
        }

        // reset relevant fields
        current_size = 0;
        current_free_index = 0;

        return true;
    }

    // Sort the array in-place.
    bool bubblesort(const SortCriterion& criteria1,
                    const SortCriterion& criteria2 = { RecordFields::UNSORTED, true },
                    const SortCriterion& criteria3 = { RecordFields::UNSORTED, true })
    {
        bool swap = false;
        RecordType* curr_item;
        RecordType* next_item;
        int to_swap = -1; // for preprocessing array; -1 indicates that haven't hit a NULL yet

        // preprocess the array to move all NULL to right hand side
        for (int i = 0; i < max_size; i++) {
            if (record_array[i] == nullptr && to_swap < 0) {
                to_swap = i;
            }
            if (record_array[i] != nullptr && to_swap >= 0) {
                // swap the pointer element at this index with the NULL which is furthest left
                record_array[to_swap] = record_array[i];
                // swap the NULL to where the element was
                record_array[i] = nullptr;
                // the furthest left NULL is now the next index along
                to_swap++;
            }
        }

        // use bubblesort algorithm to sort only the elements in the array
        for (int loop_index = 0; loop_index < current_size - 1; loop_index++) {
            int numSwaps = 0;
            for (int curr_index = 0; curr_index < current_size - 1; curr_index++) {
                curr_item = record_array[curr_index];
                next_item = record_array[curr_index + 1];

                // in case there are "empty (non allocated) items"
                if ((curr_item != nullptr) && (next_item != nullptr)) {
                    swap = curr_item->compare(next_item, criteria1, criteria2, criteria3);
                }

                if (swap) {
                    swapRecords(curr_index, curr_index + 1);
                    numSwaps++;
                }
            }

            if (numSwaps == 0) {
                break;
            }
        }

        sorted_by = criteria1;
        return true;
    }

    // Check if a record is valid; returns true if it is.
    bool isValidRecord(int rec_index)
    {
        if (rec_index < 0 || rec_index > (max_size - 1)) {
            return false; // the index is out of bounds
        } else if (record_array[rec_index] == nullptr) {
            return false; // the index is not a valid record
        } else {
            return true; // the index is within bounds and points to a valid item
        }
        throw std::invalid_argument(
              "RecordArray::isValidRecord exhaused logic"); // function should never reach here
    }

    // Swap two records. Returns false if it fails.
    bool swapRecords(int rec1_index, int rec2_index)
    {
        // check that indexes are valid
        if (!isValidRecord(rec1_index) || !isValidRecord(rec2_index)) {
            return false; // at least one of the indexes were invalid
        }

        // swap the records
        RecordType* temp_record = record_array[rec1_index];
        record_array[rec1_index] = record_array[rec2_index];
        record_array[rec2_index] = temp_record;

        sorted_by = { RecordFields::UNSORTED,
                      true }; // swapping records means the array is not sorted
        return true;
    }

    // Lock the array so it cannot be altered. Note that this does *not* lock the individual
    // Records.
    bool setArrayLocked(bool set_locked)
    {
        array_locked = set_locked;
        return true;
    }

    // Print information about the array, but not the contents of the array.
    bool printArrayInfoOnScreen() const
    {
        std::cout << std::boolalpha; // enable output of bool formatted as True/False
        std::cout << "Array Information:" << std::endl;
        printArrayType();
        std::cout << "Max size: " << max_size << std::endl;
        std::cout << "Current size: " << current_size << std::endl;
        std::cout << "Sorted by: " << convertSortCriterionToString(sorted_by) << std::endl;
        std::cout << "Locked: " << array_locked << std::endl << std::endl;
        return true;
    }

    // Print the contents of the array. If debug=true then will also print the reference count of
    // each Record.
    bool printArrayOnScreen(bool debug = false) const
    {
        std::cout << "START of printing RecordArray:" << std::endl;
        for (int i = 0; i < max_size; i++) {
            if (record_array[i] != nullptr) {
                std::cout << "** Record index " << i << std::endl;
                record_array[i]->printInfo();
                if (debug) {
                    std::cout << "record included in: ";
                    std::cout << RecordAttorney::getIncludeCounter(*record_array[i]) << std::endl;
                }
            }
        }
        std::cout << "END of printing RecordArray" << std::endl << std::endl;
        return true;
    }

    // Print the type of Record that this array holds. Template will be specialised for each type.
    bool printArrayType() const
    {
        std::cout << "This array has been created with a class that has no specialist template";
        std::cout << std::endl;
        return true;
    }

    // General getter functions:

    bool getIsFull() const { return (current_size >= max_size); }

    SortCriterion getSortedBy() const { return sorted_by; }

    int getMaxSize() const { return max_size; }

    int getCurrentSize() const { return current_size; }

    // Returns the pointer to a record.
    RecordType* getRecord(int index) const
    {
        if (index >= 0 && index < max_size && record_array[index] != nullptr) {
            return record_array[index];
        }
        return nullptr;
    }

    // File handling functions:

    bool writeAllToFile(const std::string& filename)
    {
        return file_manager.writeAllToFile(*this, filename);
    }

    bool loadFromFile(const std::string& filename)
    {
        return file_manager.loadFromFile(*this, filename);
    }

    bool appendToFile(const std::string& filename)
    {
        return file_manager.appendToFile(*this, filename);
    }

    // bool updateFile(const std::string &filename, int record_index) {
    //     return file_manager.updateFile(*this, filename, record_index);
    // }
};

// Specialisations:

// This specialisation is to safeguard against the array being used for the base class.
template<>
bool
RecordArray<BasicRecord>::addRecord(__attribute__((unused)) int a,
                                    __attribute__((unused)) bool b,
                                    __attribute__((unused)) BasicRecord* c)
{
    throw std::invalid_argument("RecordArray should not be specialised with BasicRecord");
}

// This specialisation is for use in basic testing.
// template<>
// bool RecordArray<DummyRecord>::printArrayType() const
//     {
//         std::cout << "Array record type: DummyRecord, to be used for testing
//         only" << std::endl; return true;
//     }

// These specialisations are for Record version A.
template<>
bool
RecordArray<RecordvA>::printArrayType() const
{
    std::cout << "Array record type: Record version A" << std::endl;
    return true;
}

// These specialisations are for Record version B.
template<>
bool
RecordArray<RecordvB>::printArrayType() const
{
    std::cout << "Array record type: Record version B" << std::endl;
    return true;
}

// Class used for basic testing. This contains simple code to carry out basic
// tests before testing is done using the other classes. class DummyRecord {
// protected:
//     friend class RecordAttorney;

//     int getIncludeCounter() {return included_in;}

//     bool decrementIncludeCounter() {included_in--; return (included_in <=
//     0);}

//     bool incrementIncludeCounter() {included_in++; return false;}

// public:
//     int included_in = 0;
//     int value = 0;

//     DummyRecord() {}

//     DummyRecord(const DummyRecord &original) { value = original.value; }

//     bool set(bool random=false)
//     {
//         if (random) {
//             value = std::rand() + 10;
//         } else {
//             value = 1;
//         }
//         return true;
//     }

//     bool compare(const DummyRecord *other, __attribute__((unused)) const
//     RecordFields field, ComparisonType &compareResult)
//     {

//         if (value > other->value) { compareResult =
//         ComparisonType::GREATER_THAN; } if (value < other->value) {
//         compareResult = ComparisonType::LOWER_THAN; } if (value ==
//         other->value) { compareResult = ComparisonType::EQUAL_TO; }

//         return true;
//     }

//     void printInfo()
//     {
//         std::cout << "Value: " << value << std::endl << "included_in: " <<
//         included_in << std::endl;
//     }
// };
