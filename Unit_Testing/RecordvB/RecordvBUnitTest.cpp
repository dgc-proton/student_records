#include <iostream>
using namespace std;

#include "../../RecordvB_R2.h"
//#include <time.h>

int main() {
    cout << "Record 1: Generating Random" << endl;
    string in_errorMessageHolder;
    RecordvB myRecord1;
    myRecord1.setRecord(in_errorMessageHolder, true);
    cout << "Record 2:" << endl;
    RecordvB myRecord2;
    myRecord2.setRecord(in_errorMessageHolder, false);
    cout << "Record 1:" << endl;
    myRecord1.printInfo();
    cout << "Record 2" << endl;
    myRecord2.printInfo();
    SortCriterion criteria1, criteria2, criteria3, criteria4, criteria5;
    criteria1.ascending = true;
    criteria1.field = RecordFields::FIRST_NAME;
    criteria2.ascending = false;
    criteria2.field = RecordFields::FAMILY_NAME;
    criteria3.ascending = true;
    criteria3.field = RecordFields::STUDENT_ID;
    criteria4.ascending = false;
    criteria4.field = RecordFields::DEGREE;
    criteria5.ascending = true;
    criteria5.field = RecordFields::DEGREE_TYPE;
    if (!myRecord1.getRecordInvalid() && !myRecord2.getRecordInvalid()) {
        cout << "Comparision 1: " << "By First Name (Ascending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, criteria1)) {
            cout << "not ";
        }
        cout << "be swapped with Record 2" << endl;

        cout << "Comparision 2: " << "By Family Name (Descending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, criteria2)) {
            cout << "not ";
        }
        cout << "be swapped with Record 2" << endl;

        cout << "Comparision 3: " << "By Student ID (Ascending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, criteria3)) {
            cout << "not ";
        }
        cout << "be swapped with Record 2" << endl;

        cout << "Comparision 4: " << "By Degree (Descending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, criteria4)) {
            cout << "not ";
        }
        cout << "be swapped with Record 2" << endl;

        cout << "Comparision 5: " << "By Degree Type (Ascending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, criteria5)) {
            cout << "not ";
        }
        cout << "be swapped with Record 2" << endl;
    } else {
        cout << "Records are not comparable. One of them is invalid" << endl;
    }

    cout << "Copy Constructor" << endl;
    RecordAttorney attorney;
    cout << "Increment record 2 include counter" << endl;
    attorney.incrementIncludeCounter(myRecord2);
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    RecordvB myRecord3(myRecord2);
    cout << "Copy Complete: Record 3 copied from Record 2." << endl;
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    cout << "Record 3 Included In count: " << attorney.getIncludeCounter(myRecord3) << endl;
    attorney.decrementIncludeCounter(myRecord2);
    cout << "Decrement record 2 include counter" << endl;
    cout << "Record 2 Included In count: " << attorney.getIncludeCounter(myRecord2) << endl;
    cout << "Record 3 Included In count: " << attorney.getIncludeCounter(myRecord3) << endl;
    cout << "Record 2:" << endl;
    myRecord2.printInfo();
    cout << "Record 3 (Copied from Record 2):" << endl;
    myRecord3.printInfo();

    if (!myRecord3.getRecordInvalid() && !myRecord2.getRecordInvalid()) {
        cout << "Comparision 1: " << "By First Name (Ascending)" << endl;
        cout << "Record 2 should ";
        if (!myRecord2.compare(&myRecord3, criteria1)) {
            cout << "not ";
        }
        cout << "be swapped with Record 3" << endl;

        cout << "Comparision 2: " << "By Family Name (Descending)" << endl;
        cout << "Record 2 should ";
        if (!myRecord2.compare(&myRecord3, criteria2)) {
            cout << "not ";
        }
        cout << "be swapped with Record 3" << endl;

        cout << "Comparision 3: " << "By Student ID (Ascending)" << endl;
        cout << "Record 2 should ";
        if (!myRecord2.compare(&myRecord3, criteria3)) {
            cout << "not ";
        }
        cout << "be swapped with Record 3" << endl;

        cout << "Comparision 4: " << "By Degree (Descending)" << endl;
        cout << "Record 2 should ";
        if (!myRecord2.compare(&myRecord3, criteria4)) {
            cout << "not ";
        }
        cout << "be swapped with Record 3" << endl;

        cout << "Comparision 5: " << "By Degree Type (Ascending)" << endl;
        cout << "Record 2 should ";
        if (!myRecord2.compare(&myRecord3, criteria5)) {
            cout << "not ";
        }
        cout << "be swapped with Record 3" << endl;
    } else {
        cout << "Records are not comparable. One of them is invalid" << endl;
    }

    myRecord3.lock();
    cout << "Locking Record 3 and trying to edit" << endl;
    if (!myRecord3.editRecord(in_errorMessageHolder, true)) {
        cout << in_errorMessageHolder << endl;
    }
    cout << "Printing Record 3: " << endl;
    myRecord3.printInfo();
    cout << "Unlock Record 3 and try to edit" << endl;
    myRecord3.unlock();
    if (!myRecord3.editRecord(in_errorMessageHolder, false)) {
        cout << in_errorMessageHolder << endl;
    }
    cout << "Printing Record 3: " << endl;
    myRecord3.printInfo();
}