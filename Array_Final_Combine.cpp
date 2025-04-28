#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <map>
#include <algorithm>

using namespace std;
using namespace chrono;

const int MAX_TRANSACTIONS = 10000;
const int MAX_REVIEWS = 10000;

struct Transaction {
    string customerID;
    string product;
    string category;
    float price;
    string date;
    string paymentMethod;
};

struct Review {
    string productID;
    string customerID;
    int rating;
    string reviewText;
};

Transaction transactions[MAX_TRANSACTIONS];
Review reviews[MAX_REVIEWS];
int transactionCount = 0;
int reviewCount = 0;

bool parseTransaction(const string &line, Transaction &t) {
    size_t pos = 0;
    string token;
    string temp = line;
    string fields[6];
    int i = 0;

    while ((pos = temp.find(',')) != string::npos && i < 5) {
        fields[i++] = temp.substr(0, pos);
        temp.erase(0, pos + 1);
    }
    fields[i] = temp;

    if (i < 5) return false;

    t.customerID = fields[0];
    t.product = fields[1];
    t.category = fields[2];
    t.price = stof(fields[3]);
    t.date = fields[4];
    t.paymentMethod = fields[5];

    return true;
}

bool parseReview(const string &line, Review &r) {
    stringstream ss(line);
    string field;
    string fields[100];
    int count = 0;

    while (getline(ss, field, ',')) {
        fields[count++] = field;
    }

    if (count < 4) return false;

    r.productID = fields[0];
    r.customerID = fields[1];
    r.rating = stoi(fields[2]);
    r.reviewText = fields[3];

    for (int i = 4; i < count; ++i) {
        r.reviewText += "," + fields[i];
    }

    return true;
}

// Bubble Sort for sorting by date
void bubbleSortByDate(Transaction arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].date > arr[j + 1].date) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Quick Sort for sorting by date
int partition(Transaction arr[], int low, int high) {
    string pivot = arr[high].date;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].date < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSortByDate(Transaction arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSortByDate(arr, low, pi - 1);
        quickSortByDate(arr, pi + 1, high);
    }
}

void showSortedTransactions(Transaction arr[], int n) {
    cout << "\n--- Transactions Sorted by Date ---\n";
    int counter=0;
    for (int i = 0; i < n; ++i) {
        counter++;
        cout << "Date: " << arr[i].date
             << " | CustomerID: " << arr[i].customerID
             << " | Product: " << arr[i].product
             << " | Category: " << arr[i].category
             << " | Price: RM" << arr[i].price
             << " | Payment: " << arr[i].paymentMethod << "\n";
    }
    cout << "Total Number of Transactions: " << counter << endl;
}

// Function 2: Filter by category and payment method
void filterTransactions(Transaction arr[], int n) {
    string filterCategory, filterPayment;
    cout << "\nEnter category to filter: ";
    getline(cin, filterCategory);
    cout << "Enter payment method to filter: ";
    getline(cin, filterPayment);

    int categoryCount = 0;
    int filteredCount = 0;

    Transaction filtered[100]; 
    int filteredIndex = 0;

    for (int i = 0; i < n; ++i) {
        if (arr[i].category == filterCategory) {
            categoryCount++;
            if (arr[i].paymentMethod == filterPayment) {
                if (filteredIndex < 100) {
                    filtered[filteredIndex++] = arr[i];
                }
                filteredCount++;
            }
        }
    }

    cout << "\nCategory: " << filterCategory << " | Payment Method: " << filterPayment << "\n";
    cout << "Matching transactions: " << filteredCount << " out of " << categoryCount << "\n";

    if (categoryCount > 0) {
        float percentage = (filteredCount * 100.0f) / categoryCount;
        cout << "Percentage: " << percentage << "%\n";
    } else {
        cout << "No transactions found in that category.\n";
    }

    cout << "\n--- Showing up to 10 filtered transactions ---\n";
    int limit = (filteredIndex > 10) ? 10 : filteredIndex;
    for (int i = 0; i < limit; ++i) {
        cout << "Date: " << filtered[i].date
             << " | CustomerID: " << filtered[i].customerID
             << " | Product: " << filtered[i].product
             << " | Category: " << filtered[i].category
             << " | Price: RM" << filtered[i].price
             << " | Payment: " << filtered[i].paymentMethod << "\n";
    }
}

// Function 3: Filter transactions by date range
void filterTransactionsByDate(Transaction arr[], int n) {
    string startDate, endDate;
    cout << "\nEnter start date (YYYY-MM-DD): ";
    getline(cin, startDate);
    cout << "Enter end date (YYYY-MM-DD): ";
    getline(cin, endDate);

    quickSortByDate(arr, 0, n - 1); // Using Quick Sort before filtering

    cout << "\n--- Transactions from " << startDate << " to " << endDate << " ---\n";

    bool found = false;
    for (int i = 0; i < n; ++i) {
        if (arr[i].date >= startDate && arr[i].date <= endDate) {
            cout << "Date: " << arr[i].date
                 << " | CustomerID: " << arr[i].customerID
                 << " | Product: " << arr[i].product
                 << " | Category: " << arr[i].category
                 << " | Price: RM" << arr[i].price
                 << " | Payment: " << arr[i].paymentMethod << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found in the given date range.\n";
    }
}

// Function 4: Analyze 1-star reviews
void analyze1StarReviews() {
    map<string, int> wordFrequency;

    for (int i = 0; i < reviewCount; ++i) {
        if (reviews[i].rating == 1) {
            stringstream ss(reviews[i].reviewText);
            string word;

            while (ss >> word) {
                word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                if (word.length() > 2) { 
                    wordFrequency[word]++;
                }
            }
        }
    }

    pair<string, int> wordList[1000];
    int idx = 0;

    for (auto it = wordFrequency.begin(); it != wordFrequency.end(); ++it) {
        wordList[idx++] = *it;
    }

    for (int i = 0; i < idx - 1; ++i) {
        for (int j = 0; j < idx - i - 1; ++j) {
            if (wordList[j].second < wordList[j + 1].second) {
                swap(wordList[j], wordList[j + 1]);
            }
        }
    }

    cout << "\n--- Top Frequent Words in 1-Star Reviews ---\n";
    int displayed = 0;
    for (int i = 0; i < idx && displayed < 10; ++i) {
        cout << wordList[i].first << " : " << wordList[i].second << "\n";
        displayed++;
    }
}

// Function 5: Show all reviews
void showAllReviews() {
    int counter = 0;
    cout << "\n--- All Reviews ---\n";
    for (int i = 0; i < reviewCount; ++i) {
        counter++;
        cout << "Product ID: " << reviews[i].productID
             << " | Customer ID: " << reviews[i].customerID
             << " | Rating: " << reviews[i].rating
             << " | Review: " << reviews[i].reviewText << "\n";
    }
    cout << "Total Number of Reviews: " << counter << endl;
}

int main() {
    string line;

    ifstream file1("transactions_clean.csv");
    if (!file1.is_open()) {
        cout << "Failed to open transactions file.\n";
        return 1;
    }
    getline(file1, line); 
    while (getline(file1, line) && transactionCount < MAX_TRANSACTIONS) {
        Transaction t;
        if (parseTransaction(line, t)) {
            transactions[transactionCount++] = t;
        }
    }
    file1.close();
    bubbleSortByDate(transactions, transactionCount); // Bubble sort initially
    cout << "Transactions loaded and sorted: " << transactionCount << "\n";

    ifstream file2("reviews_clean.csv");
    if (!file2.is_open()) {
        cout << "Failed to open reviews file.\n";
        return 1;
    }
    getline(file2, line); 
    while (getline(file2, line) && reviewCount < MAX_REVIEWS) {
        Review r;
        if (parseReview(line, r)) {
            reviews[reviewCount++] = r;
        }
    }
    file2.close();
    cout << "Reviews loaded: " << reviewCount << "\n";

    // Main Menu
    while (true) {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Show all transactions sorted by date\n";
        cout << "2. Filter transactions by category and payment method\n";
        cout << "3. Filter transactions by date range\n";
        cout << "4. Analyze 1-star reviews (most frequent words)\n";
        cout << "5. Show all reviews\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            auto start = high_resolution_clock::now();

            showSortedTransactions(transactions, transactionCount);
        
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "Time taken: " << duration.count() << " microseconds" << endl;
        } else if (choice == "2") {
            auto start = high_resolution_clock::now();

            filterTransactions(transactions, transactionCount);
        
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "Time taken: " << duration.count() << " microseconds" << endl;
        } else if (choice == "3") {
            auto start = high_resolution_clock::now();

            filterTransactionsByDate(transactions, transactionCount);
        
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "Time taken: " << duration.count() << " microseconds" << endl;
        } else if (choice == "4") {
            auto start = high_resolution_clock::now();

            analyze1StarReviews();
        
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "Time taken: " << duration.count() << " microseconds" << endl;
        } else if (choice == "5") {
            auto start = high_resolution_clock::now();

            showAllReviews();
        
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start);
                cout << "Time taken: " << duration.count() << " microseconds" << endl;
        } else if (choice == "6") {
            cout << "Thanks for using the system!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
