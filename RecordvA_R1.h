/*****************************
******************************
** RecordvA Class **
- Developed by Raghav Kejriwal

** Made for Release 1 **

** Assuming C++11
******************************
*****************************/

#ifndef RECORDvA_R1_H
#define RECORDvA_R1_H

#include <iostream>
#include <string>
using namespace std;

#include "FieldR1.h"
#include "Record.h"

#define MAX_FAIL_SET_FROM_KEYBOARD 5

// This implements a class containing an object of type IntegerWithLimits.
// Derived from abstract class BasicRecord in Record.h
class RecordvA : public BasicRecord {
protected:
    IntegerWithLimits my_int;

public:
    // Constructor
    RecordvA() {}

    // Copy constructor
    RecordvA(const RecordvA &other) : BasicRecord(other), my_int(other.my_int) {}

    // Used to set a record, with an option to generate random values
    // Should only return false in error cases (failure of helper functions)
    bool setRecord(string &in_errorMessageHolder, const bool genRandom = false) {
        if (getLocked()) {
            in_errorMessageHolder = "Record is Locked.";
            return true;
        }

        bool success = true;
        if (genRandom) {
            success = generateRandom();
            if (!success) {
                cout << "Unexpected Error: generateRandom() in RecordvA constructor returned false." << endl;
                bailout();
            }
        } else {
            success = setFromKeyboard();
            if (!success) {
                cout << "Unexpected Error: setFromKeyboard() in RecordvA constructor returned false." << endl;
                bailout();
            }
        }

        setRecordValidity();
        return success;
    }

    // Used to edit a record after creation. In this case, it only calls setRecord,
    // however for further implementations of similar classes, it allows editing
    // single fields at a time if the class is a composite class
    bool editRecord(string &in_errorMessageHolder, const bool genRandom = false) {
        if (getLocked()) {
            in_errorMessageHolder = "Record is Locked.";
            return true;
        }
        cout << "Editing Field IntegerWithLimits" << endl;
        if (!setRecord(in_errorMessageHolder, genRandom)) {
            cout << "Unexpected Error. setRecord in editRecord returned false." << endl;
            bailout();
        }

        setRecordValidity();
        return true;
    }

    // Takes struct SortCriterion. Returns boolean specifying if items should be swapped.
    // Assumes that any nulls or invalid records are kept to the RHS.
    bool compare(const BasicRecord* other, const SortCriterion &criteria1, __attribute__((unused)) const SortCriterion &criteria2, __attribute__((unused)) const SortCriterion &criteria3) const {
        bool success = true;

        // Guarantee no swap if other record is null or invalid
        if (other == NULL || other->getRecordInvalid()) {
            return false;
        }

        // Guarantee Swap if other record is valid or part valid, and this record is invalid
        if (!other->getRecordInvalid() && getRecordInvalid()) {
            return true;
        }

        // Error and guarantee no swap if criteria out of bounds.
        if (criteria1.field <= RecordFields::START || criteria1.field >= RecordFields::RECORDvA_STOP) {
            throw runtime_error("SortCriterion field out of bounds in RecordvA::compare()");
            return false;
        }

        // Error and guarantee no swap if items are not of same type
        const RecordvA* typecasted = typecastItem(other, this);
        if (typecasted == NULL) {
            throw runtime_error("Items are not of same type");
            return false;
        }
        
        ComparisonType compareResult;

        if (criteria1.field == RecordFields::INT_W_LIMITS) {
            success &= my_int.compare(typecasted->my_int, compareResult);
        }

        if (!success) {
            throw runtime_error("my_int.compare() in RecordvA::compare() returned false");
            return false;
        }

        // holds which comparision result the items should be swapped for.
        // (i.e. if swap_if is GREATER_THAN, the items should be swapped
        //  if compareResult is GREATER_THAN)
        ComparisonType swap_if;
        if (criteria1.ascending) {
            swap_if = ComparisonType::GREATER_THAN;
        } else {
            swap_if = ComparisonType::LOWER_THAN;
        }

        if (compareResult == swap_if) {
            return true;
        } else {
            return false;
        }
    }

    // Print member function values
    bool printInfo() const {
        int minVal, maxVal, val;
        if (getRecordInvalid()) {
            cout << "Record is Invalid" << endl;
            return false;
        }
        my_int.getMinValue(minVal);
        my_int.getMaxValue(maxVal);
        my_int.getValue(val);
        cout << "Minimum: " << minVal << endl;
        cout << "Maximum: " << maxVal << endl;
        cout << "Value: " << val << endl;
        return true;
    }

    // Check compatibility of other record with this using typecast
    bool compatibilityCheck(const BasicRecord* other) const {
        if (other == NULL) {
            return false;
        } else {
            const RecordvA* typecasted = typecastItem(other, this);
            if (typecasted == NULL) {
                return false;
            } else {
                return true;
            }
        }
    }

    // Setter function if values need to be specified by argument
    bool setIntValues(const int minVal, const int maxVal, const int val) {
        if (getLocked()) {
            cout << "Record is Locked." << endl;
            return true;
        }
        bool success = true;
        string in_errorMessageHolder = "";
        success &= my_int.setMinAndMaxValues(minVal, maxVal, in_errorMessageHolder);
        if (success) {
            success &= my_int.setValue(val, in_errorMessageHolder);
        }
        if (!success) {
            cout << in_errorMessageHolder << endl;
        }
        setRecordValidity();
        return success;
    }
protected:
    // Set values from keyboard
    bool setFromKeyboard() {
        bool success = true;

        int minVal = 0;
        int maxVal = 0;

        int failCount = 0;

        string in_errorMessageHolder = "";
        string minValHolder = "", maxValHolder = "";
        string prompt;

        bool exitSet = false; // set true if user types exit. Whenever this happens, validity is set to PARTIALLY_VALID

        if (success && !exitSet) {
            prompt = "Type Integer Minimum Value (EXIT to stop) and hit ENTER: ";
            success = stringInputToInt(prompt, minVal, exitSet, in_errorMessageHolder);
        }

        if (success && !exitSet) {
            prompt = "Type Integer Maximum Value (EXIT to stop) and hit ENTER: ";
            success = stringInputToInt(prompt, maxVal, exitSet, in_errorMessageHolder);
        }

        if (exitSet) {
            cout << "User exit from set from keyboard" << endl;
        } else if (!success) {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        if (success && !exitSet) {
            success &= my_int.setMinAndMaxValues(minVal, maxVal, in_errorMessageHolder);
            failCount++;
        }
        /*failCount is incremented because it is set to 0 after loop anyways. so it doesnt matter if it is incremented regardless of success
        Equivalent to:
        if (!success) {
            failCount++;
        }
        */

        // repeats min and max set until valid input or return false from member object more than set number of times
        while (!success && !exitSet) {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD) {
                cout << "Unexpected Error: my_int.setMinAndMaxValues() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This MAY be due to user setting maximum value larger than minimum. If not, refer to devs." << endl;
                success = false;
                bailout();
            } else {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";

                prompt = "Type Integer Minimum Value (EXIT to stop) and hit ENTER: ";
                success = stringInputToInt(prompt, minVal, exitSet, in_errorMessageHolder);

                if (exitSet) {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                } else if (!success) {
                    cout << in_errorMessageHolder;
                    in_errorMessageHolder = "";
                    continue;
                }

                prompt = "Type Integer Maximum Value (EXIT to stop) and hit ENTER: ";
                success = stringInputToInt(prompt, maxVal, exitSet, in_errorMessageHolder);

                if (exitSet) {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                } else if (!success) {
                    cout << in_errorMessageHolder;
                    in_errorMessageHolder = "";
                    continue;
                }

                success &= my_int.setMinAndMaxValues(minVal, maxVal, in_errorMessageHolder);
                failCount++;
            }
        }

        failCount = 0; // reset counter
        int val = 0;
        in_errorMessageHolder = "";
        string valHolder;

        if (success && !exitSet) {
            prompt = "Type Integer Value (EXIT to stop) and hit ENTER: ";
            success = stringInputToInt(prompt, val, exitSet, in_errorMessageHolder);
        }

        if (exitSet) {
            cout << "User exit from set from keyboard" << endl;
        } else if (!success) {
            cout << in_errorMessageHolder << endl;
            in_errorMessageHolder = "";
        }

        if(success && !exitSet) {
            success &= my_int.setValue(val, in_errorMessageHolder);
            failCount++;
        }
        /*failCount is incremented because it is set to 0 after loop anyways. so it doesnt matter if it is incremented regardless of success
        Equivalent to:
        if (!success) {
            failCount++;
        }
        */

        //repeats integer set until valid input or member object returns false more than set no of times
        while (!success && !exitSet) {
            success = true;
            if (failCount > MAX_FAIL_SET_FROM_KEYBOARD) {
                cout << "Unexpected Error: my_int.setVal() returned false more than " << MAX_FAIL_SET_FROM_KEYBOARD << " times. This MAY be due to user setting value outside of limits. If not, refer to devs." << endl;
                success = false;
                bailout();
            } else {
                cout << in_errorMessageHolder << endl;
                in_errorMessageHolder = "";

                prompt = "Type Integer Value (EXIT to stop) and hit ENTER: ";
                success = stringInputToInt(prompt, val, exitSet, in_errorMessageHolder);

                if (exitSet) {
                    cout << "User exit from set from keyboard" << endl;
                    continue;
                } else if (!success) {
                    cout << in_errorMessageHolder << endl;
                    in_errorMessageHolder = "";
                    continue;
                }

                failCount++;
                success &= my_int.setValue(val, in_errorMessageHolder);
            }
        }
        
        //failCount = 0; Needed if further inputs are added.
        setRecordValidity();

        return success;
    }

    // Calls generateRandom() for member objects
    bool generateRandom() {
        if (getLocked()) {
            return false;
        }
        
        bool success = true;
        success = my_int.generateRandom();
        if (!success) {
            cout << "Unexpected Error: my_int.generateRandom() in RecordvA.setRandom returned false." << endl;
            bailout();
        } else {
            setRecordValidity();
        }

        return success;
    }

    // To be used after any settter functions. Changes non-static member valid
    // (inherited from abstract class BasicRecord) based on initFlags of member
    // objects
    void setRecordValidity() {
        if (my_int.getInitFlag()) {
            BasicRecord::setRecordValid();
        } else {
            BasicRecord::setRecordInvalid();
        }
    }

    // disable assignment operator
    void operator=(const RecordvA &record) = delete;

    // disable comparision operators
    bool operator==(const RecordvA &record) = delete;
    bool operator!=(const RecordvA &record) = delete;
    bool operator<(const RecordvA &record) = delete;
    bool operator>(const RecordvA &record) = delete;
    bool operator<=(const RecordvA &record) = delete;
    bool operator>=(const RecordvA &record) = delete;
};
#endif