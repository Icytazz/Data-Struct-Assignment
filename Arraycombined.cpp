#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <limits>

using namespace std;

const int MAX_TRANSACTIONS = 10000;
const int MAX_REVIEWS = 10000;
const int MAX_WORDS = 1000;

struct Transaction {
    string customerID;
    string product;
    string category;
    float price;
    string date;
    string paymentMethod;
};

struct Review {
    string ProdID;
    string CustID;
    int Rating;
    string ReviewTxt;
};

struct WordCount {
    string word;
    int count;
};

// Parse a transaction line into a Transaction struct
bool parseTransaction(const string &line, Transaction &t) {
    stringstream ss(line);
    string fields[6];
    for (int i = 0; i < 6; ++i) {
        if (!getline(ss, fields[i], ',')) return false;
    }

    t.customerID = fields[0];
    t.product = fields[1];
    t.category = fields[2];
    t.price = stof(fields[3]);
    t.date = fields[4];
    t.paymentMethod = fields[5];

    return true;
}

// Sort transactions by date (ascending)
void sortByDate(Transaction transactions[], int n) {
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (transactions[j].date > transactions[j + 1].date)
                swap(transactions[j], transactions[j + 1]);
}

// Display all sorted transactions
void showSortedTransactions(Transaction transactions[], int n) {
    cout << "\n--- Transactions Sorted by Date ---\n";
    for (int i = 0; i < n; ++i) {
        cout << "[" << i + 1 << "] "
             << "Date: " << transactions[i].date
             << " | CustomerID: " << transactions[i].customerID
             << " | Product: " << transactions[i].product
             << " | Category: " << transactions[i].category
             << " | Price: RM" << transactions[i].price
             << " | Payment: " << transactions[i].paymentMethod << "\n";
    }
}

// Filter by category and payment method
void filterTransactions(Transaction transactions[], int n) {
    string filterCategory, filterPayment;
    cout << "\nEnter category to filter: ";
    getline(cin, filterCategory);
    cout << "Enter payment method to filter: ";
    getline(cin, filterPayment);

    int categoryCount = 0;
    int filteredCount = 0;

    for (int i = 0; i < n; ++i) {
        if (transactions[i].category == filterCategory) {
            categoryCount++;
            if (transactions[i].paymentMethod == filterPayment) {
                filteredCount++;
            }
        }
    }

    cout << "\nResults:\n";
    cout << "Category: " << filterCategory << " | Payment Method: " << filterPayment << "\n";
    cout << "Matching transactions: " << filteredCount << " out of " << categoryCount << "\n";

    if (categoryCount > 0) {
        float percentage = (filteredCount * 100.0f) / categoryCount;
        cout << "Percentage: " << percentage << "%\n";
    } else {
        cout << "No transactions found in that category.\n";
    }
}

// Manage Review Data
struct ReviewArray {
    Review reviews[MAX_REVIEWS];
    int size = 0;

    void AddReview(const string& prodID, const string& custID, int rating, const string& reviewTxt) {
        if (size < MAX_REVIEWS) {
            reviews[size++] = {prodID, custID, rating, reviewTxt};
        }
    }

    bool LoadReviewsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return false;

        string line;
        getline(file, line); // skip header

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
                AddReview(prodID, custID, rating, reviewTxt);
            } catch (...) {
                continue;
            }
        }

        file.close();
        return true;
    }

    string CleanWord(string word) {
        string clean;
        for (char c : word) {
            if (isalnum(c)) clean += tolower(c);
        }
        return clean;
    }

    void MostFrequentWordsIn1StarReviews(int topN = 10) {
        WordCount wordList[MAX_WORDS];
        int wordCountSize = 0;

        for (int i = 0; i < size; ++i) {
            if (reviews[i].Rating == 1) {
                stringstream ss(reviews[i].ReviewTxt);
                string word;
                while (ss >> word) {
                    word = CleanWord(word);
                    if (word.empty()) continue;

                    bool found = false;
                    for (int j = 0; j < wordCountSize; ++j) {
                        if (wordList[j].word == word) {
                            wordList[j].count++;
                            found = true;
                            break;
                        }
                    }
                    if (!found && wordCountSize < MAX_WORDS) {
                        wordList[wordCountSize++] = {word, 1};
                    }
                }
            }
        }

        // Sort words by frequency
        for (int i = 0; i < wordCountSize - 1; ++i)
            for (int j = 0; j < wordCountSize - i - 1; ++j)
                if (wordList[j].count < wordList[j + 1].count)
                    swap(wordList[j], wordList[j + 1]);

        cout << "\nTop " << topN << " most frequent words in 1-star reviews:\n";
        for (int i = 0; i < topN && i < wordCountSize; ++i) {
            cout << i+1 << ". " << wordList[i].word << ": " << wordList[i].count << " times\n";
        }
    }
};

void showMenu() {
    cout << "\n=====================================\n";
    cout << "        E-Commerce Analyzer          \n";
    cout << "=====================================\n";
    cout << "1. Show all transactions sorted by date\n";
    cout << "2. Filter transactions by category and payment method\n";
    cout << "3. Show frequent words in 1-star reviews\n";
    cout << "4. Exit\n";
    cout << "=====================================\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount = 0;

    ifstream file("C:/SEMESTER 4/Data Structures/project1_code/transactions_clean.csv");
    if (!file.is_open()) {
        cout << "Failed to open transactions file.\n";
        return 1;
    }

    string line;
    getline(file, line); 

    while (getline(file, line) && transactionCount < MAX_TRANSACTIONS) {
        Transaction t;
        if (parseTransaction(line, t)) {
            transactions[transactionCount++] = t;
        }
    }
    file.close();

    sortByDate(transactions, transactionCount);

    // Load reviews
    ReviewArray reviews;
    bool reviewsLoaded = reviews.LoadReviewsFromCSV("C:/SEMESTER 4/Data Structures/project1_code/reviews_clean.csv");
    if (!reviewsLoaded) {
        cout << "Warning: Failed to load reviews file. Option 3 may not work.\n";
    }

    while (true) {
        showMenu();

        string choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear any leftover input
        getline(cin, choice);

        if (choice == "1") {
            showSortedTransactions(transactions, transactionCount);
        } else if (choice == "2") {
            filterTransactions(transactions, transactionCount);
        } else if (choice == "3") {
            if (reviewsLoaded) {
                reviews.MostFrequentWordsIn1StarReviews();
            } else {
                cout << "Reviews data not loaded properly.\n";
            }
        } else if (choice == "4") {
            cout << "\nThank you for using the system. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
