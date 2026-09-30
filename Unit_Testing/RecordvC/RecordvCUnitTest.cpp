#include "../../RecordvC_R4.h"

int main() {
    RecordvC myRecord1, myRecord2;
    string error;
    myRecord1.setRecord(error, false);
    cout << "Record 1:" << endl;
    myRecord1.printInfo();

    myRecord1.editRecord(error, false);
    cout << "Record 1 (edited)" << endl;
    myRecord1.printInfo();

    myRecord2.setRecord(error, true);
    cout << "Record 2 (Randomly Generated):" << endl;
    myRecord2.printInfo();

    cout << endl << endl;
    cout << "Record 1 Detailed Info" << endl;
    myRecord1.printInfoDetailed();
    cout << endl;
    cout << "Record 2 Detailed Info" << endl;
    myRecord2.printInfoDetailed();

    if (!myRecord1.getRecordInvalid() && !myRecord2.getRecordInvalid()) {
        cout << "Comparision (by CGS ascending, first enrollment year descending, and family name ascending)" << endl;
        cout << "Record 1 should ";
        if (!myRecord1.compare(&myRecord2, {RecordFields::CGS, true}, {RecordFields::ENROLLMENT_YEAR, false}, {RecordFields::FAMILY_NAME, true})) {
            cout << "not ";
        }
        cout << "be swapped with record 2" << endl;
    }
    
}