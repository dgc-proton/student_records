#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

template <class RecordType>
class RecordArray;

template <class RecordType>
class RecordFileManager
{
public:
    // Write the entire array to file
    bool writeAllToFile(const RecordArray<RecordType> &array, const string &filename) const
    {
        ofstream out(filename);
        if (!out.is_open())
        {
            cout << "Error opening file: " << filename << "\n";
            return false;
        }

        // Count actual records first
        // int recordCount = 0;
        int recordCount = array.getCurrentSize();
        // for (int i = 0; i < max; ++i)
        // {
        //     if (array.getRecord(i) != nullptr)
        //     {
        //         recordCount++;
        //     }
        // }

        // Write header with record count
        out << "RECORDS=" << recordCount << "\n";

        // Write all records
        for (int i = 0; i < recordCount; ++i)
        {
            RecordType *rec = array.getRecord(i);
            if (rec != nullptr)
            {
                if (!rec->writeToStream(out))
                {
                    cout << "Error writing record at index " << i << "\n";
                    return false;
                }
                out << "\n";
            }
        }
        return true;
    }

    bool loadFromFile(RecordArray<RecordType> &array, const string &filename) const
    {
        ifstream in(filename);
        if (!in.is_open())
        {
            cout << "Error opening file: " << filename << "\n";
            return false;
        }

        int index = array.getCurrentSize();
        int max = array.getMaxSize();

        string line;
        bool firstLine = true;
        int expectedRecords = 0;

        while (getline(in, line))
        {
            if (line.empty())
                continue;

            // Check if first line is header with record count
            if (firstLine && line.find("RECORDS=") == 0)
            {
                string countStr = line.substr(8); // Skip "RECORDS="
                expectedRecords = stoi(countStr);
                cout << "File contains " << expectedRecords << " records\n";
                firstLine = false;
                continue;
            }
            firstLine = false;

            if (index >= max)
            {
                cout << "Array full\n";
                return false;
            }

            if (!array.addRecord(index, false))
            {
                cout << "Error creating record at " << index << "\n";
                return false;
            }

            RecordType *rec = array.getRecord(index);
            istringstream iss(line);

            if (!rec->readFromStream(iss))
            {
                cout << "Error reading record at " << index << "\n";
                return false;
            }

            ++index;
        }
        return true;
    }

    bool appendToFile(const RecordArray<RecordType> &array, const string &filename) const
    {
        // First, read the current count from header
        int currentCount = 0;
        ifstream in(filename);
        if (in.is_open())
        {
            string line;
            if (getline(in, line) && line.find("RECORDS=") == 0)
            {
                currentCount = stoi(line.substr(8));
            }
            in.close();
        }

        // Update the header with new count
        fstream file(filename, ios::in | ios::out);
        if (file.is_open())
        {
            int newCount = currentCount + array.getCurrentSize();
            file.seekp(0);
            file << "RECORDS=" << newCount;
            file.close();
        }

        // Now append the new records
        ofstream out(filename, ios::app);
        if (!out.is_open())
        {
            cout << "Error opening file for append: " << filename << "\n";
            return false;
        }

        for (int i = 0; i < array.getCurrentSize(); ++i)
        {
            RecordType *rec = array.getRecord(i);
            if (rec != nullptr)
            {
                if (!rec->writeToStream(out))
                {
                    cout << "Error writing record at index " << i << "\n";
                    return false;
                }
                out << "\n";
            }
        }
        return true;
    }

    // bool updateFile(const RecordArray<RecordType> &array, const string &filename, int updateIndex = -1) const
    // {
    //     // If no specific index, update all records
    //     if (updateIndex == -1)
    //     {
    //         return writeAllToFile(array, filename);
    //     }

    //     // Check bounds
    //     if (updateIndex < 0 || updateIndex >= array.getCurrentSize() || array.getRecord(updateIndex) == nullptr)
    //     {
    //         cout << "Invalid index for update: " << updateIndex << "\n";
    //         return false;
    //     }

    //     // Simple approach: read entire file, update in memory, write back
    //     vector<string> allLines;

    //     // Read all lines from file
    //     ifstream in(filename);
    //     if (!in.is_open())
    //     {
    //         cout << "Error opening file for update: " << filename << "\n";
    //         return false;
    //     }

    //     string line;
    //     while (getline(in, line))
    //     {
    //         allLines.push_back(line);
    //     }
    //     in.close();

    //     // Check we have enough lines (header + records)
    //     if (allLines.size() <= updateIndex + 1) // +1 because of header
    //     {
    //         cout << "File doesn't have enough records for update index " << updateIndex << "\n";
    //         return false;
    //     }

    //     // Create the updated record string
    //     ostringstream newRecord;
    //     RecordType *rec = array.getRecord(updateIndex);
    //     if (!rec->writeToStream(newRecord))
    //     {
    //         cout << "Error creating updated record\n";
    //         return false;
    //     }

    //     // Replace the specific line (header is index 0, so records start at index 1)
    //     allLines[updateIndex + 1] = newRecord.str();

    //     // Write entire file back
    //     ofstream out(filename);
    //     if (!out.is_open())
    //     {
    //         cout << "Error opening file for writing update: " << filename << "\n";
    //         return false;
    //     }

    //     for (const string &fileLine : allLines)
    //     {
    //         out << fileLine << "\n";
    //     }

    //     out.close();
    //     return true;
    // }
};