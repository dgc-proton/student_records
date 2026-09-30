#pragma once

#include <algorithm>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <iterator>
#include <list>
#include <optional>
#include <stdexcept>
#include <string>

#include "Definitions.h"
#include "Record.h"
#include "RecordFileManager.h"
#include "RecordvA_R1.h"
#include "RecordvB_R2.h"
#include "RecordvC_R4.h"

template <class RecordType>
class RecordLinkedList
{
protected:
    std::list<RecordType *> record_list;
    SortCriterion sorted_by = SortCriterion{RecordFields::UNSORTED, true};

    // merge sort helper function. Takes a list as input, splits into two halves.
    // recursively calls itself on each half. Merges using std::list::merge().
    void mergeSortPriv(std::list<RecordType*>& my_list,
                       const SortCriterion& crit1,
                       const SortCriterion& crit2,
                       const SortCriterion& crit3)
    {
        if (my_list.size() <= 1) {
            return;
        }

        std::list<RecordType*> left, right;
        auto mid = my_list.begin();
        std::advance(mid, my_list.size() / 2);

        left.splice(left.begin(), my_list, my_list.begin(), mid);
        right.splice(right.begin(), my_list, mid, my_list.end());

        mergeSortPriv(left, crit1, crit2, crit3);
        mergeSortPriv(right, crit1, crit2, crit3);

        // merge using lambda function that returns negated value of compare()
        // (compare() returns true if a > b (i.e., b should appear before a).
        // mergeSort requires it to return true if a < b (a should appear before b))
        my_list.merge(left, [&](RecordType* a, RecordType* b) {
            return !a->compare(b, crit1, crit2, crit3);
        });

        my_list.merge(right, [&](RecordType* a, RecordType* b) {
            return !a->compare(b, crit1, crit2, crit3);
        });
    }

public:
    // Default constructor.
    RecordLinkedList() {}

    // Copy constructor.
    RecordLinkedList(const RecordLinkedList &original, bool shallow_copy = true)
        : record_list(original.record_list)
    {
        typename std::list<RecordType *>::iterator it1 = record_list.begin(); // create list iterator

        // the pointers have already been copied (shallow copy of list) by the initialiser list copy
        // constructor; increment the Record included in counters
        for (int i = 0; i < ((int)record_list.size()); i++)
        {
            RecordAttorney::incrementIncludeCounter(**it1);
            std::advance(it1, 1);
        }

        if (!shallow_copy)
        {
            clearList();
            typename std::list<RecordType *>::const_iterator it2 =
                original.record_list.begin(); // create list iterator
            while (record_list.size() < original.record_list.size())
            {
                addRecord(-1, true, *it2);
                std::advance(it2, 1);
            }
        }
    }

    // Destructor.
    ~RecordLinkedList() { clearList(); }

    // Adds a record to the specified index in the list. Defaults to index -1 (back). Use index 0
    // to add to the front. Adding to front or back is much more efficient than adding at other
    // index values.
    bool addRecord(int new_record_index = -1,
                   bool deep_copy = false,
                   RecordType *record_ptr = nullptr)
    {
        std::exception_ptr eptr = nullptr; // used for catching exceptions
        RecordType *new_record_ptr = nullptr;

        // check that the index is valid
        if (new_record_index < -1 || new_record_index > (int)(record_list.size()))
        {
            std::string msg =
                "RecordLinkedList::addRecord() invalid argument provided: new_record_index=";
            msg.append(std::to_string(new_record_index));
            throw std::invalid_argument(msg);
        }

        // allocate memory for the new record
        try
        {
            if (deep_copy)
            {
                // this record is to be a copy of an existing record
                new_record_ptr = new RecordType(*record_ptr);
            }
            else
            {
                // this record will be populated with new data
                new_record_ptr = new RecordType();
            }
        }
        catch (std::bad_alloc &ex)
        {
            // if memory allocation fails then catch the error, carry out essential
            // tasks then rethrow the error
            eptr = std::current_exception();
            std::cout << "Request failed for memory to create a new record. ";
            std::cout << "Details for bad_alloc:" << ex.what() << std::endl;
            // TODO add code to save current state?
            throw; // rethrow the error to unwind stack and terminate program
        }

        // memory has been sucessfully allocated, increment record include counter
        RecordAttorney::incrementIncludeCounter(
            *new_record_ptr); // increment included in counter for the record

        // add pointer to the new record to the list
        switch (new_record_index)
        {
        case -1:
            record_list.push_back(new_record_ptr);
            break;
        case 0:
            record_list.push_front(new_record_ptr);
            break;
        default:
            // create a list iterator, advance it to the position for insert then insert ptr
            typename std::list<RecordType *>::iterator it = record_list.begin();
            std::advance(it, new_record_index);
            record_list.insert(it, new_record_ptr);
        }

        sorted_by = {RecordFields::UNSORTED, true};

        return true;
    }

    // Edit an existing record.
    bool editRecord(int record_index, bool set_all = false, bool randomise = false)
    {
        std::string msg_holder = "";
        bool success;

        // check that the index is valid
        if (record_index < 0 || record_index > ((int)record_list.size() - 1))
        {
            std::string msg =
                "RecordLinkedList::editRecord() invalid argument provided: record_index=";
            msg.append(std::to_string(record_index));
            throw std::invalid_argument(msg);
        }

        // editing a record means the array will no longer be sorted
        sorted_by = {RecordFields::UNSORTED, true};

        // create a list iterator, advance it to the position for the list Record ptr
        typename std::list<RecordType *>::iterator it = record_list.begin();
        std::advance(it, record_index);

        // edit the record
        if (set_all)
        {
            success = (*it)->setRecord(msg_holder, randomise);
        }
        else
        {
            success = (*it)->editRecord(msg_holder, randomise);
        }

        if (!success)
        {
            std::cout << "Error returned by Record when attempting to edit / set: " << msg_holder
                      << std::endl;
        }

        return success;
    }

    // Add manually entered records to the list until user types "exit".
    bool enterListFromKeyboard()
    {
        bool success = false; // used for checking function return values
        std::string error_msg;
        std::string option;

        while (true)
        {

            // add the record
            success = addRecord();
            if (!success)
            {
                std::cout << "An error occured adding that last record. The record was not added."
                          << std::endl;
                return false;
            }

            // populate the record
            error_msg = "";
            (*record_list.back()).setRecord(error_msg, false);
            if (!error_msg.empty())
            {
                // an error occured (should only happen if record is locked which will not be the
                // case here)
                std::cout << error_msg << std::endl;
            }

            // check if user wants to add the next record
            while (true)
            {
                std::cout << "Enter another record? [y|n]";
                std::cin >> option;
                std::transform(option.begin(),
                               option.end(),
                               option.begin(),
                               ::tolower); // lowercase the input
                if (option == "y" || option == "n")
                {
                    break;
                }
            }

            if (option == "n")
            {
                break;
            }
        }

        return true;
    }

    // Fill the list with random records.
    bool fillRandomValueList(int num_records_toadd)
    {
        bool success = false; // used for checking function return values
        std::string error_msg;
        std::string option;

        for (int i = 0; i < num_records_toadd; i++)
        {
            // add the record
            success = addRecord();
            if (!success)
            {
                std::cout << "An error occured adding a random record." << std::endl;
                return false;
            }

            error_msg = "";
            (*record_list.back()).setRecord(error_msg, true);
            if (!error_msg.empty())
            {
                // an error occured (should only happen if record is locked which will not be the
                // case here)
                std::cout << error_msg << std::endl;
            }
        }

        return true;
    }

    // Delete record at specified index. If index=-1 delete the last record.
    bool deleteRecord(int record_index)
    {
        // check that the index is valid
        if (record_index < -1 || record_index > ((int)record_list.size() - 1))
        {
            std::string msg =
                "RecordLinkedList::deleteRecord() invalid argument provided: record_index=";
            msg.append(std::to_string(record_index));
            throw std::invalid_argument(msg);
        }

        // decrement the record include counter, free memory if required then remove from list
        switch (record_index)
        {
        case -1:
            RecordAttorney::decrementIncludeCounter(*record_list.back());
            if (RecordAttorney::getIncludeCounter(*record_list.back()) < 1)
            {
                delete record_list
                    .back(); // free Record memory if this was the last pointer to it
            }
            record_list.pop_back(); // remove the pointer from the list
            break;
        case 0:
            RecordAttorney::decrementIncludeCounter(*record_list.front());
            if (RecordAttorney::getIncludeCounter(*record_list.front()) < 1)
            {
                delete record_list
                    .front(); // free Record memory if this was the last pointer to it
            }
            record_list.pop_front(); // remove the pointer from the list
            break;
        default:
            // create a list iterator, advance it to the position for the record
            typename std::list<RecordType *>::iterator it = record_list.begin();
            std::advance(it, record_index);
            RecordAttorney::decrementIncludeCounter(**it);
            if (RecordAttorney::getIncludeCounter(**it) < 1)
            {
                delete *it; // free Record memory if this was the last pointer to it
            }
            record_list.erase(it);
            break;
        }

        sorted_by = {RecordFields::UNSORTED, true};

        return true;
    }

    // Clear all contents from the list.
    bool clearList()
    {
        while (record_list.size())
        {
            deleteRecord(0);
        }

        return true;
    }

    // Swap two records. Returns false if it fails.
    bool swapRecords(int rec1_index, int rec2_index)
    {
        int final_index = static_cast<int>(record_list.size()) - 1;

        // check that the index is valid
        if (rec1_index < 0 || rec2_index < 0 || rec1_index > final_index ||
            rec2_index > final_index)
        {
            std::string msg =
                "RecordLinkedList::swapRecords() invalid argument provided, either: rec1_index=";
            msg.append(std::to_string(rec1_index));
            msg.append(" or rec2_index=");
            msg.append(std::to_string(rec2_index));
            throw std::invalid_argument(msg);
        }

        // create list iterators and advance them to the positions for the pointers
        typename std::list<RecordType *>::iterator it1 = record_list.begin();
        typename std::list<RecordType *>::iterator it2 = record_list.begin();
        std::advance(it1, rec1_index);
        std::advance(it2, rec2_index);

        // swap the records
        std::swap(*it1, *it2);
        sorted_by = {RecordFields::UNSORTED, true};
        return true;
    }

    // public function to call private merge sort implementation.
    // done with a private function as it requires the list to be passed as a parameter
    void mergeSort(const SortCriterion& criteria1,
                   const SortCriterion& criteria2 = { RecordFields::UNSORTED, true },
                   const SortCriterion& criteria3 = { RecordFields::UNSORTED, true })
    {
        mergeSortPriv(record_list, criteria1, criteria2, criteria3);
        sorted_by = criteria1;
    }

    // Search the list for records matching the target record's field specified by criteria.
    // Returns a new list with shallow copies of matching records.
    RecordLinkedList<RecordType> simpleSearchList(const RecordType &target_record,
                                                  const SortCriterion &criteria)
    {
        RecordLinkedList<RecordType> results_list;

        for (RecordType *record_ptr : record_list)
        {
            if (record_ptr->matchesField(target_record, criteria))
            {
                RecordAttorney::incrementIncludeCounter(*record_ptr);
                results_list.record_list.push_back(record_ptr);
            }
        }

        return results_list;
    }

    // Complex search the list for records that fall within specified ranges of target record fields.
    // Returns a new list with shallow copies of matching records.
    RecordLinkedList<RecordType> complexSearchList(const RecordType *min_record,
                                                   const RecordType *max_record,
                                                   const SortCriterion &criteria)
    {
        RecordLinkedList<RecordType> results_list;

        for (RecordType *record_ptr : record_list)
        {
            if (record_ptr->matchesFieldRange(min_record, max_record, criteria))
            {
                RecordAttorney::incrementIncludeCounter(*record_ptr);
                results_list.record_list.push_back(record_ptr);
            }
        }

        return results_list;
    }

    // Sort the list in-place.
    bool bubblesort(const SortCriterion &criteria1,
                    const SortCriterion &criteria2 = {RecordFields::UNSORTED, true},
                    const SortCriterion &criteria3 = {RecordFields::UNSORTED, true})
    {
        bool swap = false;
        RecordType *curr_item;
        RecordType *next_item;
        typename std::list<RecordType *>::const_iterator it1;
        typename std::list<RecordType *>::const_iterator it2;
        int numSwaps = 0;
        int curr_index = 0;

        for (it1 = record_list.begin(); *it1 != record_list.back(); std::advance(it1, 1))
        {
            numSwaps = 0;
            curr_index = 0;
            for (it2 = record_list.begin();;)
            {
                curr_item = *it2;
                std::advance(it2, 1);
                next_item = *it2;

                swap = curr_item->compare(next_item, criteria1, criteria2, criteria3);
                if (swap)
                {
                    swapRecords(curr_index, curr_index + 1);
                    numSwaps++;
                }
                curr_index++;
                if (*it2 == record_list.back())
                {
                    break; // do not repeat the loop, there is not item beyond this one to compare
                           // it with
                }
            }

            if (numSwaps == 0)
            {
                break;
            }
        }

        sorted_by = criteria1;
        return true;
    }

    // Print information about the list, but not the contents of the list.
    bool printListInfoOnScreen() const
    {
        std::cout << std::boolalpha; // enable output of bool formatted as True/False
        std::cout << "Array Information:" << std::endl;
        printListType();
        std::cout << "Current size: " << record_list.size() << std::endl;
        std::cout << "Sorted by: " << convertSortCriterionToString(sorted_by) << std::endl;
        return true;
    }

    // Print the contents of the list. If debug=true then will also print the reference count of
    // each Record.
    bool printListOnScreen(bool debug = false) const
    {
        typename std::list<RecordType *>::const_iterator it = record_list.begin();
        std::cout << "START of printing RecordLinkedList:" << std::endl;
        for (int i = 0; i < (int)record_list.size(); i++)
        {
            std::cout << "** Record index " << i << std::endl;
            (*it)->printInfo();
            if (debug)
            {
                std::cout << "record included in: ";
                std::cout << RecordAttorney::getIncludeCounter(**it) << std::endl;
            }
            std::advance(it, 1); // increment the iterator
        }
        std::cout << "END of printing RecordLinkedList" << std::endl
                  << std::endl;
        return true;
    }

    // Print the type of Record that this list holds. Template will be specialised for each type.
    bool printListType() const
    {
        std::cout << "This list has been created with a class that has no specialist template";
        std::cout << std::endl;
        return true;
    }

    // General getter functions:

    SortCriterion getSortedBy() const { return sorted_by; }

    int getCurrentSize() const { return record_list.size(); }

    // Returns the pointer to a record.
    RecordType *getRecord(int record_index) const
    {
        RecordType *rec_ptr = nullptr;

        // check that the index is valid
        if (record_index < 0 || record_index > ((int)record_list.size() - 1))
        {
            std::string msg =
                "RecordLinkedList::deleteRecord() invalid argument provided: record_index=";
            msg.append(std::to_string(record_index));
            throw std::invalid_argument(msg);
        }
        else
        {
            // create a list iterator, advance it to the record position
            typename std::list<RecordType *>::const_iterator it = record_list.begin();
            std::advance(it, record_index);
            rec_ptr = *it;
        }

        return rec_ptr;
    }

    // Disable copy operator so that only deep or shallow copy methods are used.
    void operator=(RecordLinkedList const &RecordLinkedList) = delete;
    // Disable comparison operators
    void operator==(const RecordLinkedList &RecordLinkedList) = delete;
    void operator!=(const RecordLinkedList &RecordLinkedList) = delete;
    void operator<(const RecordLinkedList &RecordLinkedList) = delete;
    void operator>(const RecordLinkedList &RecordLinkedList) = delete;
    void operator<=(const RecordLinkedList &RecordLinkedList) = delete;
    void operator>=(const RecordLinkedList &RecordLinkedList) = delete;
};

// Specialisations:

// This specialisation is to safeguard against the array being used for the
// base class.
template <>
bool RecordLinkedList<BasicRecord>::addRecord(__attribute__((unused)) int a,
                                              __attribute__((unused)) bool b,
                                              __attribute__((unused)) BasicRecord *c)
{
    throw std::invalid_argument("RecordLinkedList should not be specialised with BasicRecord");
}

// This specialisation is for use in basic testing.
// template<>
// bool RecordLinkedList<DummyRecord>::printArrayType() const
//     {
//         std::cout << "Array record type: DummyRecord, to be used for testing
//         only" << std::endl; return true;
//     }

// These specialisations are for Record version A.
template <>
bool RecordLinkedList<RecordvA>::printListType() const
{
    std::cout << "List record type: Record version A" << std::endl;
    return true;
}

// These specialisations are for Record version B.
template <>
bool RecordLinkedList<RecordvB>::printListType() const
{
    std::cout << "List record type: Record version B" << std::endl;
    return true;
}

// These specialisations are for Record version C.
template <>
bool RecordLinkedList<RecordvC>::printListType() const
{
    std::cout << "List record type: Record version C" << std::endl;
    return true;
}
