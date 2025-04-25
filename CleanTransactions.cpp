#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int MAX_COLUMNS = 6;

int splitCSVLine(const string& line, string columns[]) {
    stringstream ss(line);
    string item;
    int count = 0;

    while (getline(ss, item, ',') && count < MAX_COLUMNS) {
        columns[count++] = item;
    }

    return count;
}

bool isValidRow(string columns[], int columnCount) {
    if (columnCount != MAX_COLUMNS) return false;

    for (int i = 0; i < columnCount; i++) {
        if (columns[i].empty() || columns[i] == "NaN") return false;
    }

    // Date column is at index 4
    if (columns[4] == "Invalid Date") return false;

    return true;
}

int main() {
    ifstream infile("C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/transactions.csv");
    ofstream outfile("C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/transactions_cleaned.csv");

    if (!infile || !outfile) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    string line;
    bool isHeader = true;

    while (getline(infile, line)) {
        string columns[MAX_COLUMNS];
        int count = splitCSVLine(line, columns);

        if (isHeader) {
            outfile << line << endl;
            isHeader = false;
        } else if (isValidRow(columns, count)) {
            outfile << line << endl;
        }
    }

    infile.close();
    outfile.close();

    cout << "Cleaning complete. Output saved to transactions_cleaned.csv" << endl;
    return 0;
}
