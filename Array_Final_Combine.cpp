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

struct TransactionsArray {
    Transaction data[MAX_TRANSACTIONS];
    int size = 0;
};

struct Review {
    string productID;
    string customerID;
    int rating;
    string reviewText;
};

Review reviews[MAX_REVIEWS];
int reviewCount = 0;

bool LoadTransactionsFromCSV(const string& filename, TransactionsArray& txArray) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    getline(file, line);

    while (getline(file, line) && txArray.size < MAX_TRANSACTIONS) {
        stringstream ss(line);
        string custID, product, category, priceStr, date, paymentMethod;

        getline(ss, custID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod);

        try {
            float price = stof(priceStr);
            txArray.data[txArray.size++] = {custID, product, category, price, date, paymentMethod};
        } catch (...) {
            continue;
        }
    }

    file.close();
    return true;
}

bool LoadReviewsFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return false;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string prodID, custID, ratingStr, reviewTxt;

        getline(ss, prodID, ',');
        getline(ss, custID, ',');
        getline(ss, ratingStr, ',');
        getline(ss, reviewTxt);

        if (!reviewTxt.empty() && reviewTxt.front() == '"' && reviewTxt.back() == '"') {
            reviewTxt = reviewTxt.substr(1, reviewTxt.size() - 2);
        }

        try {
            int rating = stoi(ratingStr);
            reviews[reviewCount++] = {prodID, custID, rating, reviewTxt};
        } catch (...) {
            continue;
        }
    }

    file.close();
    return true;
}

void insertionSortByDate(Transaction arr[], int n) {
    for (int i = 1; i < n; ++i) {
        Transaction key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].date > key.date) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void showSortedTransactions(Transaction arr[], int n) {
    cout << "\n--- Transactions Sorted by Date ---\n";
    for (int i = 0; i < n; ++i) {
        cout << "Date: " << arr[i].date
             << " | CustomerID: " << arr[i].customerID
             << " | Product: " << arr[i].product
             << " | Category: " << arr[i].category
             << " | Price: RM" << arr[i].price
             << " | Payment: " << arr[i].paymentMethod << "\n";
    }
    cout << "Total Number of Transactions: " << n << endl;
}

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

void filterTransactionsByDate(Transaction arr[], int n) {
    string startDate, endDate;
    cout << "\nEnter start date (YYYY-MM-DD): ";
    getline(cin, startDate);
    cout << "Enter end date (YYYY-MM-DD): ";
    getline(cin, endDate);

    insertionSortByDate(arr, n);

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
    if (!found) cout << "No transactions found in the given date range.\n";
}

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
    for (auto& p : wordFrequency) {
        wordList[idx++] = p;
    }
    sort(wordList, wordList + idx, [](auto &a, auto &b) {
        return a.second > b.second;
    });

    cout << "\n--- Top Frequent Words in 1-Star Reviews ---\n";
    for (int i = 0; i < min(10, idx); ++i) {
        cout << wordList[i].first << " : " << wordList[i].second << "\n";
    }
}

void showAllReviews() {
    cout << "\n--- All Reviews ---\n";
    for (int i = 0; i < reviewCount; ++i) {
        cout << "Product ID: " << reviews[i].productID
             << " | Customer ID: " << reviews[i].customerID
             << " | Rating: " << reviews[i].rating
             << " | Review: " << reviews[i].reviewText << "\n";
    }
    cout << "Total Number of Reviews: " << reviewCount << endl;
}

int main() {
    TransactionsArray txArray;
    if (!LoadTransactionsFromCSV("transactions_clean.csv", txArray)) return 1;
    if (!LoadReviewsFromCSV("reviews_clean.csv")) return 1;

    insertionSortByDate(txArray.data, txArray.size);

    while (true) {
        cout << "\n--- MAIN MENU ---\n";
        cout << "1. Show all transactions sorted by date\n";
        cout << "2. Filter transactions by category and payment method\n";
        cout << "3. Filter transactions by date range\n";
        cout << "4. Analyze 1-star reviews\n";
        cout << "5. Show all reviews\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        string choice;
        getline(cin, choice);

        auto start = high_resolution_clock::now();

        if (choice == "1") showSortedTransactions(txArray.data, txArray.size);
        else if (choice == "2") filterTransactions(txArray.data, txArray.size);
        else if (choice == "3") filterTransactionsByDate(txArray.data, txArray.size);
        else if (choice == "4") analyze1StarReviews();
        else if (choice == "5") showAllReviews();
        else if (choice == "6") break;
        else cout << "Invalid choice. Try again.\n";

        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        cout << "Time taken: " << duration.count() << " microseconds\n";
    }

    cout << "Thanks for using the system!\n";
    return 0;
}
