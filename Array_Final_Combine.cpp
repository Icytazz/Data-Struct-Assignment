#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <limits>  // Added this for numeric_limits

using namespace std;
using namespace chrono;

const int MAX_TRANSACTIONS = 10000;

struct Transaction {
    string customerID;
    string product;
    string category;
    float price;
    string date;
    string paymentMethod;
};

struct TransactionsArray {
    Transaction transactions[MAX_TRANSACTIONS];
    int size = 0;
    int sortType = 0; // 1 for bubble, 2 for insertion

    bool LoadFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }

        string line;
        getline(file, line); // skip header

        while (getline(file, line) && size < MAX_TRANSACTIONS) {
            stringstream ss(line);
            string customerID, product, category, priceStr, date, payment;
            getline(ss, customerID, ',');
            getline(ss, product, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, date, ',');
            getline(ss, payment);

            try {
                transactions[size++] = { customerID, product, category, stof(priceStr), date, payment };
            } catch (...) {
                continue;
            }
        }
        return true;
    }

    void BubbleSortByDate() {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (transactions[j].date > transactions[j + 1].date) {
                    swap(transactions[j], transactions[j + 1]);
                }
            }
        }
        sortType = 1;
    }

    void InsertionSortByDate() {
        for (int i = 1; i < size; ++i) {
            Transaction key = transactions[i];
            int j = i - 1;
            while (j >= 0 && transactions[j].date > key.date) {
                transactions[j + 1] = transactions[j];
                --j;
            }
            transactions[j + 1] = key;
        }
        sortType = 2;
    }

    void DisplayTransactionsByDate() const {
        cout << "\n--- Transactions Sorted by Date ---\n";
        for (int i = 0; i < size; ++i) {
            const auto& t = transactions[i];
            cout << "Date: " << t.date
                 << " | CustomerID: " << t.customerID
                 << " | Product: " << t.product
                 << " | Category: " << t.category
                 << " | Price: RM" << t.price
                 << " | Payment: " << t.paymentMethod << "\n";
        }
        cout << "Total Number of Transactions: " << size << endl;
    }

    void FilterByCategoryAndPayment() {
        string filterCategory, filterPayment;
        cout << "\nEnter category to filter: ";
        getline(cin, filterCategory);
        cout << "Enter payment method to filter: ";
        getline(cin, filterPayment);
    
        int categoryCount = 0;
        int filteredCount = 0;
        Transaction toDisplay[10]; // only store 10 for display
        int displayIndex = 0;
    
        for (int i = 0; i < size; ++i) {
            if (transactions[i].category == filterCategory) {
                categoryCount++;
                if (transactions[i].paymentMethod == filterPayment) {
                    filteredCount++;
                    if (displayIndex < 10) {
                        toDisplay[displayIndex++] = transactions[i];
                    }
                }
            }
        }
    
        cout << "\n" << filteredCount << " " << filterPayment << " out of " << categoryCount << " " << filterCategory << "\n";
    
        if (filteredCount > 0) {
            cout << "\n--- Showing up to 10 filtered transactions ---\n";
            for (int i = 0; i < displayIndex; ++i) {
                const auto& t = toDisplay[i];
                cout << "Date: " << t.date
                     << " | CustomerID: " << t.customerID
                     << " | Product: " << t.product
                     << " | Category: " << t.category
                     << " | Price: RM" << t.price
                     << " | Payment: " << t.paymentMethod << "\n";
            }
        } else {
            cout << "No matching transactions found.\n";
        }
    }

    void FilterByDateRange() const {
        string startDate, endDate;
        cout << "\nEnter start date (YYYY-MM-DD): ";
        getline(cin, startDate);
        cout << "Enter end date (YYYY-MM-DD): ";
        getline(cin, endDate);

        cout << "\n--- Transactions from " << startDate << " to " << endDate << " ---\n";
        bool found = false;
        for (int i = 0; i < size; ++i) {
            if (transactions[i].date >= startDate && transactions[i].date <= endDate) {
                const auto& t = transactions[i];
                cout << "Date: " << t.date
                     << " | CustomerID: " << t.customerID
                     << " | Product: " << t.product
                     << " | Category: " << t.category
                     << " | Price: RM" << t.price
                     << " | Payment: " << t.paymentMethod << "\n";
                found = true;
            }
        }
        if (!found) cout << "No transactions found in the given range.\n";
    }
};

int main() {
    TransactionsArray tArray;
    if (!tArray.LoadFromCSV("transactions_clean.csv")) {
        return 1;
    }

    int choice;
    while (true) {
        cout << "\n" << (1 == 1 ? "Array: Transactions" : "Linked List: Transactions") << endl;
        cout << "1. Sort Customer Transactions by Date (BubbleSort)\n";
        cout << "2. Sort Customer Transactions by Date (InsertionSort)\n";
        cout << "3. Search and Filter Transactions by Category and Payment Method\n";
        cout << "4. Filter transactions by date range\n";
        cout << "5. Display Transactions by date\n";
        cout << "6. Back\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Fixed input handling

        auto start = high_resolution_clock::now();

        if (choice == 1) {
            tArray.BubbleSortByDate();
        } else if (choice == 2) {
            tArray.InsertionSortByDate();
        } else if (choice == 3) {
            tArray.FilterByCategoryAndPayment();
        } else if (choice == 4) {
            tArray.FilterByDateRange();
        } else if (choice == 5) {
            tArray.DisplayTransactionsByDate();
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Time taken: " << duration.count() << " microseconds\n";
    }

    return 0;
}
