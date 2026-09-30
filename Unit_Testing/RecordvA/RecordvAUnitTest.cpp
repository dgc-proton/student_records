#include <iostream>
using namespace std;

#include "../../RecordvA_R1.h"
#include <time.h>

int main() {
    srand(time(NULL));
    string errorMessageHolder = "";
    cout << "Record 1: Random Generation" << endl;
    RecordvA myRecord;
    myRecord.setRecord(errorMessageHolder, true);
    cout << "Record 2:" << endl;
    RecordvA myRecord2;
    myRecord2.setRecord(errorMessageHolder, false);
    cout << "Record 1:" << endl;
    myRecord.printInfo();
    cout << "Record 2:" << endl;
    myRecord2.printInfo();
    bool swap;
    SortCriterion criteria;
    criteria.ascending = true;
    criteria.field = RecordFields::INT_W_LIMITS;
    swap = myRecord.compare(&myRecord2, criteria);

    if (!myRecord.getRecordInvalid() || !myRecord2.getRecordInvalid()) {
        cout << "Record 1 should ";
        if (!swap) {
            cout << "not ";
        }
        cout << "be swapped with Record 2 (sort ascending)" << endl;
    } else {
        cout << "Comparision cannot be done. One of the records is invalid." << endl;
    }
    RecordAttorney attorney1;
    RecordAttorney attorney2;
    bool equal0;
    try {
        equal0 = attorney1.incrementIncludeCounter(myRecord);
    } catch (const runtime_error& e) {
        if (string(e.what()) == ATTORNEY_ERR_STR) {
            cout << "Attorney returned error: " << e.what() << endl << endl;
        } else {throw;}
    }
    cout << "Counter Increment. Attorney returned " << equal0 << endl;
    cout << "Record 1 include counter: " << attorney1.getIncludeCounter(myRecord) << endl << endl;;
    try {
        equal0 = attorney2.incrementIncludeCounter(myRecord);
    } catch (const runtime_error& e) {
        if (string(e.what()) == ATTORNEY_ERR_STR) {
            cout << "Attorney returned error: " << e.what() << endl << endl;
        } else {throw;}
    }
    cout << "Counter Increment. Attorney returned " << equal0 << endl;
    cout << "Record 1 include counter: " << attorney1.getIncludeCounter(myRecord) << endl << endl;;
    try {
        equal0 = attorney1.decrementIncludeCounter(myRecord);
    } catch (const runtime_error& e) {
        if (string(e.what()) == ATTORNEY_ERR_STR) {
            cout << "Attorney returned error: " << e.what() << endl << endl;
        } else {throw;}
    }
    cout << "Counter Decrement. Attorney returned " << equal0 << endl;
    cout << "Record 1 include counter: " << attorney1.getIncludeCounter(myRecord) << endl << endl;;
    try {
        equal0 = attorney1.decrementIncludeCounter(myRecord);
    } catch (const runtime_error& e) {
        if (string(e.what()) == ATTORNEY_ERR_STR) {
            cout << "Attorney returned error: " << e.what() << endl << endl;
        } else {throw;}
    }
    cout << "Counter Decrement. Attorney returned " << equal0 << endl;
    cout << "Record 1 include counter: " << attorney1.getIncludeCounter(myRecord) << endl << endl;;
    try {
        equal0 = attorney1.decrementIncludeCounter(myRecord);
    } catch (const runtime_error& e) {
        if (string(e.what()) == ATTORNEY_ERR_STR) {
            cout << "Attorney returned error: " << e.what() << endl << endl;
        } else {throw;}
    }
    cout << "Counter Decrement. Attorney returned " << equal0 << endl;
    cout << "Record 1 include counter: " << attorney1.getIncludeCounter(myRecord) << endl << endl;

    cout << "Copy Constructor" << endl;
    RecordAttorney attorney;
    attorney.incrementIncludeCounter(myRecord2);
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    RecordvA myRecord3(myRecord2);
    cout << "Copy Complete." << endl;
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    cout << "Record 3 Included In count: " << attorney.getIncludeCounter(myRecord3) << endl;
    attorney.decrementIncludeCounter(myRecord2);
    cout << "Decrement Counter Rec 2" << endl;
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    cout << "Record 3 Included In count: " << attorney.getIncludeCounter(myRecord3) << endl;
    cout << "Record 2:" << endl;
    myRecord2.printInfo();
    cout << "Record 3 (Copied from Record 2):" << endl;
    myRecord3.printInfo();

    criteria.ascending = false;
    myRecord2.compare(&myRecord3, criteria);
    if (!myRecord3.getRecordInvalid() || !myRecord2.getRecordInvalid()) {
        cout << "Record 2 should ";
        if (!swap) {
            cout << "not ";
        }
        cout << "be swapped with Record 3 (sort descending)" << endl;
    } else {
        cout << "Comparision cannot be done. One of the records is invalid." << endl;
    }
    return 0;
}