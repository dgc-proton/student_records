/*****************************
******************************
** BasicRecord Class (Abstract) **
- Developed by Raghav Kejriwal

** Also RecordAttorney Class **

** Dependancies:
(None but all inherited classes require Definitions.h)

** Assuming C++11
******************************
*****************************/

#ifndef RECORD_H
#define RECORD_H

#define ATTORNEY_ERR_STR "BasicRecord.includedIn < 0"
#define USER_EXIT "User Exit"

#include "Definitions.h"

// Used to typecast items to child classes if possible
template <class Base, class Derived> const Derived* typecastItem(const Base* basic_ptr, const __attribute__((unused)) Derived* derivedItem_ptr)
{
	const Derived* typecasted_ptr = dynamic_cast <const Derived*>(basic_ptr);

	/*if (typecasted_ptr == NULL) {
		printf("\n Error typecasting item: type does not match the expected derived item\n");
    }*/

	return typecasted_ptr;
}

// This implements a purely abstract class BasicRecord used later.
class BasicRecord {
protected:
    RecordValidity valid; // holds whether record is valid, part valid, or invalid
    int includedIn; // holds number of arrays, lists, etc record is included in. to be used with RecordAttorney
    friend class RecordAttorney; // Access to includeCounter functions
    bool locked; // holds whether record has been locked

    // Used to increment include counter. Protected function so only accessible
    // within the class or through friend class RecordAttorney
    // returns true if includedIn = 0, false if +ve.
    // throws error if negative
    bool incrementIncludeCounter() {
        includedIn++;
        if (includedIn > 0) {
            return false;
        } else if (includedIn == 0) {
            return true;
        } else {
            throw(runtime_error(ATTORNEY_ERR_STR)); // catch using if(string(e.what()) == ATTORNEY_ERR_STRING)
        }
    }

    // Used to decrement include counter. Protected function so only accessible
    // within the class or through friend class RecordAttorney
    // returns true if includedIn = 0, false if +ve.
    // throws error if negative
    bool decrementIncludeCounter() {
        // Errors still allow decrement to help debugging if needed
        includedIn--;
        if (includedIn > 0) {
            return false;
        } else if (includedIn == 0) {
            return true;
        } else {
            throw(runtime_error(ATTORNEY_ERR_STR)); // catch using if(string(e.what()) == ATTORNEY_ERR_STRING)
        }
    }

    // Returns includedIn counter
    int getIncludeCounter() const {
        return includedIn;
    }

    // Converts string inputs to long integer. Accepts a display prompt
    // and a long int by reference that stores value. returns false if
    // invalid user input
    bool stringInputToLongInt(const string &prompt, long int &val, bool &exitStatus, string &errorMessageHolder) const {
        if (exitStatus) {
            return false;
        }

        string valHolder;

        cout << prompt;
        cin >> valHolder;
        cout << endl;

        // sets exit status to true if user types EXIT.
        if (valHolder == "EXIT") {
            exitStatus = true;
            return true;
        } else {
            try {
                val = stol(valHolder);
                return true;
            } catch (invalid_argument&) {
                // Returns false if letter is entered.
                errorMessageHolder = "Letter entered. Try again";
                return false;
            } catch (out_of_range&) {
                // Returns false on out of range
                errorMessageHolder = "Out of range. Try again";
                return false;
            }
        }
    }

    // Converts string inputs to integer. Accepts an int by reference that stores value
    // returns false if invalid user input
    static bool stringInputToInt(int &val, bool &exitStatus, string &errorMessageHolder) {
        if (exitStatus) {
            return false;
        }

        string valHolder;

        cin >> valHolder;
        cout << endl;

        // sets exit status to true if user types EXIT
        if (valHolder == "EXIT") {
            exitStatus = true;
            return true;
        } else {
            try {
                val = stoi(valHolder);
                return true;
            } catch (invalid_argument&) {
                // returns false if letter is entered
                errorMessageHolder = "Letter entered. Try again";
                return false;
            } catch (out_of_range&) {
                // returns false on out of range
                errorMessageHolder = "Out of range. Try again";
                return false;
            }
        }
    }

    bool stringInputToInt(const string &prompt, int &val, bool &exitStatus, string &errorMessageHolder) const {
        if (exitStatus) {
            return false;
        }

        string valHolder;

        cout << prompt << endl;
        cin >> valHolder;
        cout << endl;

        // sets exit status to true if user types EXIT.
        if (valHolder == "EXIT") {
            exitStatus = true;
            return true;
        } else {
            try {
                val = stoi(valHolder);
                return true;
            } catch (invalid_argument&) {
                // Returns false if letter is entered.
                errorMessageHolder = "Letter entered. Try again";
                return false;
            } catch (out_of_range&) {
                // Returns false on out of range
                errorMessageHolder = "Out of range. Try again";
                return false;
            }
        }
    }
    
    // Cleans string input (for Name). Removes leading and trailing spaces.
    // Capitalizes first letter. Only accepts A-Z or a-z or spaces.
    // returns false on invalid inputs
    bool stringClean(string &str, string &errorMessage) const {
        str = str.erase(0, str.find_first_not_of(' '));
        if (str.empty()) {
            errorMessage = "Name cannot contain only spaces. Try again";
            return false;
        }

        string out = "";
        //bool prevCharSpace = true; // To be used with commented out code

        for (char c : str) {
            if ((c < 'A' || (c > 'Z' && c < 'a') || (c > 'z')) && c != ' ') {
                errorMessage = "Name must contain only a-z or A-Z. Try again";
                return false;
            }

            /*
            This code can clean strings to remove spaces between words. Can be used for middle name in first name field, but requires use of a different function than cin
            if (c == ' ') {
                if (!prevCharSpace) {
                    prevCharSpace = true;
                    out = out + ' ';
                }
            } else {
                if (prevCharSpace) {
                    if (c >= 'a' && c <= 'z') {
                        out = out + (char) (c - 'a' + 'A');
                    } else {
                        out = out + c;
                    }
                    prevCharSpace = false;
                } else {
                    if (c >= 'A' && c <= 'Z') {
                        out = out + (char) (c - 'A' + 'a');
                    } else {
                        out = out + c;
                    }
                }
            }
            */

            if (c == str[0]) {
                if (c >= 'a' && c <= 'z') {
                    out = out + (char) (c - 'a' + 'A');
                } else {
                    out = out + c;
                }
            } else {
                if (c >= 'A' && c <= 'Z') {
                    out = out + (char) (c - 'A' + 'a');
                } else {
                    out = out + c;
                }
            }
        }

        str = out;
        str.erase(str.find_last_not_of(' ') + 1);
        return true;
    }

    // returns BasicRecord::valid
    RecordValidity getValid() const {
        return valid;
    }

    // sets BasicRecord::valid = RecordValidity::VALID;
    void setRecordValid() {
        valid = RecordValidity::VALID;
    }

    // sets BasicRecord::valid = RecordValidity::PARTIALLY_VALID;
    void setRecordPartValid() {
        valid = RecordValidity::PARTIALLY_VALID;
    }

    // sets BasicRecord::valid = RecordValidity::INVALID;
    void setRecordInvalid() {
        valid = RecordValidity::INVALID;
    }

    // purely virtual (abstract) functions to be defined on inheritance
    virtual bool setFromKeyboard() =0;
    virtual bool generateRandom() =0;
    virtual void setRecordValidity() =0;
public:
    // Basic constructor
    BasicRecord() {
        setRecordInvalid();
        includedIn = 0;
        locked = false;
    }

    // Basic copy constructor
    BasicRecord(const BasicRecord& other) : valid(other.valid), includedIn(0), locked(other.locked) {}

    // virtual destructor for base to prevent warning flags from compiler
    // regarding potentially undefined behaviour or memory leaks
    // For more info:
    // https://learn.microsoft.com/en-us/cpp/code-quality/c26436?view=msvc-170
    // https://www.geeksforgeeks.org/cpp/virtual-destructor/
    virtual ~BasicRecord() {}

    // returns locked status
    bool getLocked() const {
        return locked;
    }

    // unlocks record
    void unlock() {
        locked = false;
    }

    // locks record
    void lock() {
        locked = true;
    }

    // returns true if valid = RecordValidity::PARTIALLY_VALID
    bool getRecordPartValid() const {
        if (getValid() == RecordValidity::PARTIALLY_VALID) {
            return true;
        } else {
            return false;
        }
    }

    // returns true if valid = RecordValidity::VALID
    bool getRecordValid() const {
        if (getValid() == RecordValidity::VALID) {
            return true;
        } else {
            return false;
        }
    }

    // returns true if valid = RecordValidity::INVALID
    bool getRecordInvalid() const {
        if (getValid() == RecordValidity::INVALID) {
            return true;
        } else {
            return false;
        }
    }

    // purely virtual (abstract) functions to be defined on inheritance
    virtual bool setRecord(string &in_errorMessageHolder, const bool genRandom = false) =0;
    virtual bool editRecord(string &in_errorMessageHolder, const bool genRandom = false) =0;
    virtual bool compare(const BasicRecord *other, 
            const SortCriterion &criteria1, 
            const SortCriterion &criteria2,
            const SortCriterion &criteria3) const =0;
    virtual bool printInfo() const =0;
    virtual bool compatibilityCheck(const BasicRecord* other) const =0;

    // disable assignment operator
    void operator=(const BasicRecord &record) = delete;

    // disable comparision operators
    bool operator==(const BasicRecord &record) = delete;
    bool operator!=(const BasicRecord &record) = delete;
    bool operator<(const BasicRecord &record) = delete;
    bool operator>(const BasicRecord &record) = delete;
    bool operator<=(const BasicRecord &record) = delete;
    bool operator>=(const BasicRecord &record) = delete;
};

// This class is used to carry out actions on BasicRecord::includedIn.
// Declared as friend class to BasicRecord
class RecordAttorney {
public:
    // Increment includedIn
    static bool incrementIncludeCounter(BasicRecord& record) {
        return record.incrementIncludeCounter();
    }

    // Decrement includedIn
    static bool decrementIncludeCounter(BasicRecord& record) {
        return record.decrementIncludeCounter();
    }

    // Returns includedIn
    static int getIncludeCounter(const BasicRecord& record) {
        return record.getIncludeCounter();
    }

    template <class RecordType> friend class RecordArray;
    template <class RecordType> friend class RecordLinkedList;
};
#endif
