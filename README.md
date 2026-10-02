This repo contains a group project to create a student record system, created over a few weeks as part of a course we (Raghav, Yousef, Dave) were taking.

Issues and design discussion were tracked using github issues on the original (private) repo. Some design details remain below.

All references to course task specific details including the release stages, exact requirements etc have been removed as requested by course staff.


# Conventions & Standards

**Target: C++11 using clang++ and gcc++ compilers on Windows and Linux**

## Naming

- lowerCamelCase *for functions*
- UpperCamelCase *for classes*
- snake_case *for variables*
- enum {UPPER_SNAKE=0, INSIDE_ENUMS}

## Files

- Define all functions in their respective header file (as per course convention) rather than having a separate .cpp file.


# Design

https://mermaid.live


## Classes

### DataArray

Owner: Dave

```mermaid
classDiagram
    class RecordArray {
        - Record* record_array[]
        - int max_size
        - int current_size
        - int current_free_index
        - SortCriterion sorted_by
        - bool array_locked
        - static const int default_size
        - RecordFileManager file_manager

        - findNextFree()
        - createArray()
        + RecordArray() [copy constructor]
        + RecordArray() [default constructor]
        + ~RecordArray() [destructor]
        + addRecord()
        + editRecord()
        + enterArrayFromKeyboard()
        + fillRandomValueArray()
        + deleteRecord()
        + clearArray()
        + bubblesort()
        + isValidRecord()
        + swapRecords()
        + setArrayLocked()
        + printArrayInfoOnScreen()
        + printArrayOnScreen()
        + printArrayType()
        + getIsFull()
        + getSortedBy()
        + getMaxSize()
        + getCurrentSize()
        + getRecord()
        + writeAllToFile()
        + loadFromFile()
        + appendToFile() 
    }

        class RecordLinkedList {
        - std::list<Record*> record_list
        - SortCriterion sorted_by

        - mergeSortPriv()
        + RecordLinkedList() [copy constructor]
        + RecordLinkedList() [default constructor]
        + ~RecordLinkedList() [destructor]
        + addRecord()
        + editRecord()
        + enterListFromKeyboard()
        + fillRandomValueList()
        + deleteRecord()
        + clearList()
        + swapRecords()
        + mergeSort()
        + simpleSearchList()
        + complexSearchList()
        + printListInfoOnScreen()
        + printListOnScreen()
        + printListType()
        + getSortedBy()
        + getCurrentSize()
        + getRecord()
    }
```

## Items

Owner: Yousef

```mermaid
classDiagram
    DataArray
    
    BasicField
    BasicField <|-- IntegerWithLimits
    BasicField <|-- Name
    BasicField <|-- StudentID
    BasicField <|-- DegreeProgramme
    BasicField <|-- EnrollmentYear

    class BasicField {
        + printFieldType() string
        + virtual compare() bool
        + virtual generateRandom() bool
    }

    class IntegerWithLimits {
        -int minValue
        -int maxValue
        -int value
        -bool initFlag
        -setInitFlag() void

        +getInitFlag() bool
        +getMinValue() bool
        +getMaxValue() bool
        +getValue() bool
        +printFieldType() string
        +setMinAndMaxValues() bool
        +setValue() bool
        +compare() bool
        +generateRandom() bool
    }

    class Name {
        -string firstname
        -string lastname
        bool firstnameInitFlag
        bool lastnameInitFlag
        -setFirstnameInitFlag() void
        -setLastnameInitFlag() void

        +getFirstnameInitFlag() bool
        +getLastnameInitFlag() bool
        +getFirstname() bool
        +setFirstname() bool
        +getLastname() bool
        +setLastname() bool
        +printFieldType() string
        +compareFirstname() bool
        +compareLastname() bool
        +partialMatchFirstname() bool
        +partialMatchLastname() bool
        +generateRandom() bool
        +generateRandomFirstName() bool
        +generateRandomLastName() bool
    }

    class StudentID {
        -int studentID
        -bool initFlag
        -setInitFlag() void

        +getStudentID() bool
        +setStudentID() bool
        +printFieldType() string
        +checkIfStudentIdAdheresToRules() bool
        +compare() bool
        +partialMatchStudentId() bool
        +generateRandom() bool
        +getInitFlag() bool
    }

    class DegreeProgramme {
        -DegreeProgramme programme
        -DegreeType programme_type
        -bool programmeInitFlag
        -bool programmeTypeInitFlag
        -setProgrammeInitFlag() void
        -setProgrammeTypeInitFlag() void

        +getProgrammeInitFlag() bool
        +getProgrammeTypeInitFlag() bool
        +getProgramme() bool
        +setProgramme() bool
        +printFieldType() string
        +getProgrammeType() bool
        +setProgrammeType() bool
        +compareProgramme() bool
        +compareProgrammeType() bool
        +partialMatchProgramme() bool
        +partialMatchProgrammeType() bool
        +generateRandom() bool
        +generateRandomProg() bool
        +generateRandomDegType() bool
    }

    class EnrollmentYear {
        -int enrollmentYear
        -int CGS
        -bool enrollmentYearFieldInitFlag
        -bool CGSInitFlag
        -setEnrollmentYearFieldInitFlag() void
        -setCGSInitFlag() void

        +getEnrollmentYearFieldInitFlag() bool
        +getCGSInitFlag() bool
        +getEnrollmentYear() int
        +getCGS() int
        +setCGS() bool
        +printFieldType() string
        +getEnrollmentYear() int
        +setEnrollmentYear() bool
        +compareEnrollmentYears() bool
        +compareCGS() bool
        +generateRandom() bool
        +generateRandomCGS() bool
        +generateRandomEnrollmentYear() bool
        +partialMatchEnrollmentYear() bool
        +partialMatchCGS() bool
    }
```

## Records

Owner: Raghav

```mermaid
classDiagram
    BasicRecord
    BasicRecord <|-- RecordvA
    BasicRecord <|-- RecordvB
    RecordvB <|-- RecordvC
    RecordAttorney--BasicRecord

    class BasicRecord {
        #RecordValidity valid
        #int includedIn
        #bool locked

        #incrementIncludeCounter()
        #decrementIncludeCounter()
        #getIncludeCounter()
        #stringInputToLongInt()
        #stringInputToInt()
        #stringClean()
        #getValid()
        #setRecordValid()
        #setRecordPartValid()
        #setRecordInvalid()
        #virtual setFromKeyboard()*
        #virtual generateRandom()*
        #virtual setRecordValidity()*

        +BasicRecord() [default constructor]
        +BasicRecord() [copy constructor]
        +getLocked()
        +unlock()
        +lock()
        +getRecordPartValid()
        +getRecordValid()
        +getRecordInvalid()
        +virtual setRecord()*
        +virtual editRecord()*
        +virtual compare()*
        +virtual printInfo()*
        +virtual compatibilityCheck()*
    }

    class RecordAttorney {
        +incrementIncludeCounter()$
        +decrementIncludeCounter()$
        +getIncludeCounter()$
    }

    class RecordvA {
        #IntegerWithLimits my_int

        +RecordvA() [default constructor]
        +RecordvA() [copy constructor]
        +setRecord()
        +editRecord()
    }

    class RecordvB {
        #NameField student_name
        #StudentIdField student_id
        #DegreeProgrammeField degree

        #virtual setRecordSingleField()*
        #getFirstNameFromKeyboard()
        #getLastNameFromKeyboard()
        #getStudentIDFromKeyboard()
        #getProgrammeChoiceFromKeyboard()
        #getDegreeTypeFromKeyboard()
        #getFieldChoiceFromKeyboard()
        #getSingleSortCriteriaFromUser()$
        #intToDegreeProg()
        #intToDegreeType()
        #intToField()

        +RecordvB() [default constructor]
        +RecordvB() [copy constructor]
        +writeToStream()
        +readFromStream()
        +compareSingleField()
        +setFirstName()
        +setLastName()
        +setStudentID()
        +setDegreeProgramme()
        +setDegreeType()
        +getSortCriteriaFromUser()$
        +partialMatchCompare()
    }

    class RecordvC {
        #EnrollmentYearField years[NUM_YEARS]

        +RecordvC() [default constructor]
        +RecordvC() [copy constructor]
        +printInfoDetailed()
        +matchesField()
        +matchesFieldRange()
        #findFirstYearIndex()
        #compareFirstYear()
        #compareAvgCGS()
        #getAvgCGS()
        #setEnrollmentYear()
        #setCGS()
        #checkCGSExists()
        #checkEnrollmentYearExists()
        #checkEnrollmentYearsInRange()
        #getCGSFromKeyboard()
        #getEnrollmentYearFromKeyboard()
        #getAtleastOneYearInit()
        #setFirstYear()
        #genRandomEnrollmentYears()
        #genRandomCGS()
    }
```
