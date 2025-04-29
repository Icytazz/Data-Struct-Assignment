#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;
using namespace chrono;

const int MAX_REVIEWS = 4000;
const int MAX_WORDS = 1000;
const int MAX_TRANSACTIONS = 4500;

struct Transaction{
    string CusID;
    string Product;
    string Category;
    string Date;
    string PaymentMethod;
    float Price;
    Transaction* next;
};

struct Review{
    string ProdID;
    string CustID;
    int Rating;
    string ReviewTxt;
    Review* next;
};

struct WordNode {
    string word;
    int count;
    WordNode* next;
};

struct WordCount {
    string word;
    int count;
};

struct TransactionsLink{
    Transaction* Entry;

    TransactionsLink() { Entry = nullptr; }

    void AddNew(string CusID0, string Product0, string Category0, float Price0, string Date0, string PaymentMethod0){
        if (Entry == nullptr){
            Entry = new Transaction();
            Entry->CusID = CusID0;
            Entry->Product = Product0;
            Entry->Category = Category0;
            Entry->Price = Price0;
            Entry->Date = Date0;
            Entry->PaymentMethod = PaymentMethod0;
            Entry->next = nullptr;
        } 
        else{
            Transaction* FindEnd = Entry;

            while(FindEnd->next != nullptr){
                FindEnd = FindEnd->next;
            }
            
            Transaction* NewEntry = new Transaction();
            NewEntry->CusID = CusID0;
            NewEntry->Product = Product0;
            NewEntry->Category = Category0;
            NewEntry->Price = Price0;
            NewEntry->Date = Date0;
            NewEntry->PaymentMethod = PaymentMethod0;
            NewEntry->next = nullptr;
            FindEnd->next = NewEntry;              
        }
    }

    void displayTransactions() {
        Transaction* current = Entry;
        if (Entry == nullptr) {
            cout << "List is empty, no transactions to display." << endl;
            return;
        }
    
        while (current != nullptr) {
            cout << "Date: " << current->Date 
                 << " | CustomerID: " << current->CusID
                 << " | Product: " << current->Product
                 << " | Category: " << current->Category
                 << " | Price: RM" << fixed << setprecision(2) << current->Price
                 << " | Payment: " << current->PaymentMethod << endl;
            
            current = current->next;
        }
    }


    void bubbleSortByPrice() {
        if (Entry == nullptr || Entry->next == nullptr) return;
    
        bool swapped;
        Transaction* ptr1;
        Transaction* lptr = nullptr;
    
        do {
            swapped = false;
            ptr1 = Entry;
            Transaction* prev = nullptr;
    
            while (ptr1->next != lptr) {
                Transaction* next = ptr1->next;
    
                // swap by price
                if (ptr1->Price > next->Price) {
                    // Swap nodes
                    if (prev)
                        prev->next = next;
                    else
                        Entry = next;
    
                    ptr1->next = next->next;
                    next->next = ptr1;
    
                    swapped = true;
                    prev = next;
                } else {
                    prev = ptr1;
                    ptr1 = ptr1->next;
                }
            }
            lptr = ptr1;
        } while (swapped);
    }

    void insertionSortByDate() {
        if (Entry == nullptr || Entry->next == nullptr) return;
    
        Transaction* sorted = nullptr; // New sorted linked list
    
        Transaction* current = Entry;
        while (current != nullptr) {
            Transaction* next = current->next;
    
            // Insert current into sorted list at correct position
            if (sorted == nullptr || current->Date < sorted->Date) {
                // if the new Date in original linkedlist(current.Date) is smaller than head.Date, it becomes the head of new linkedlist
                current->next = sorted;
                sorted = current;
            } else {
                Transaction* temp = sorted;
                // Traverse sorted list to find first node where next node's Date is >= current node's Date
                while (temp->next != nullptr && temp->next->Date < current->Date) {
                    temp = temp->next;
                }

                // Inserting a node in between
                // Makes the current node point to what temp is pointing to 
                current->next = temp->next;
                // Makes the node before the insertion point (temp) point to node
                temp->next = current;
            }
            // the head of original linked list becomes the next node it points to
            current = next;
        }
        // Change the linked list originally to the sorted linked list
        Entry = sorted;
    }

    void bubbleSortByDate() {
        if (Entry == nullptr || Entry->next == nullptr) return;
    
        bool swapped;
        Transaction* ptr1;
        Transaction* lptr = nullptr;
    
        do {
            swapped = false;
            ptr1 = Entry;
            Transaction* prev = nullptr;
    
            while (ptr1->next != lptr) {
                Transaction* next = ptr1->next;
    
                if (ptr1->Date > next->Date) {
                    // Swap nodes
                    // if first node is larger, swap nodes
                    // if prev exists, point to next instead of the current node
                    if (prev){
                        // (A->ptr1->B->C   =>   A->B  )
                        prev->next = next;
                    }
                    else{
                    // if prev doesnt exist(currently at entry), next becomes the entry
                        Entry = next;
                    }
                    // the next node from current node is now the next node from next(ptr1->B->C  => ptr1-->C)
                    ptr1->next = next->next;
                    // the next node of next is now the current node(ptr1->B->C => B->ptr1-->C)
                    next->next = ptr1;
    
                    swapped = true;
                    prev = next;
                } else {
                    // if first Date is smaller, go to next node (current node becomes prev)
                    prev = ptr1;
                    ptr1 = ptr1->next;
                }
            }
            lptr = ptr1;
        } while (swapped);
    }

    void filterByDateRange(string startDate, string endDate) const {
        cout << "\n--- Transactions from " << startDate << " to " << endDate << " ---\n";
        bool found = false;
        Transaction* current = Entry;
        
        while (current != nullptr) {
            if (current->Date >= startDate && current->Date <= endDate) {
                cout << "Date: " << current->Date
                     << " | CustomerID: " << current->CusID
                     << " | Product: " << current->Product
                     << " | Category: " << current->Category
                     << " | Price: RM" << fixed << setprecision(2) << current->Price
                     << " | Payment: " << current->PaymentMethod << "\n";
                found = true;
            }
            current = current->next;
        }
        
        if (!found) cout << "No transactions found in the given range.\n";
    }

    void categoryPercentage(string userCat, string userPayment) {
        // Calculate statistics first
        int categoryTotal = 0;
        int categoryPayment = 0;
        
        Transaction* current = Entry;
        
        // Convert search terms to lowercase once
        string lowerUserCategory = userCat;
        string lowerUserPayment = userPayment;
        transform(lowerUserCategory.begin(), lowerUserCategory.end(), 
                 lowerUserCategory.begin(), ::tolower);
        transform(lowerUserPayment.begin(), lowerUserPayment.end(), 
                 lowerUserPayment.begin(), ::tolower);
        
        // First pass: count matching transactions
        while (current != nullptr) {
            string transactionCategory = current->Category;
            string transactionPayment = current->PaymentMethod;
            
            transform(transactionCategory.begin(), transactionCategory.end(), 
                     transactionCategory.begin(), ::tolower);
            transform(transactionPayment.begin(), transactionPayment.end(), 
                     transactionPayment.begin(), ::tolower);
            
            if (transactionCategory == lowerUserCategory) {
                categoryTotal++;
                if (transactionPayment == lowerUserPayment) {
                    categoryPayment++;
                }
            }
            current = current->next;
        }
        
        // Output statistics
        cout << "Category: " << userCat << " | Payment Method: " << userPayment << endl;
        cout << "Matching transactions: " << categoryPayment << " (" << userPayment << ") out of " << categoryTotal << " (" << userCat << ")" << endl;
        
        if (categoryTotal == 0) {
            cout << "Percentage: 0%" << endl;
            return;
        }
        
        float percentage = (static_cast<float>(categoryPayment) / categoryTotal) * 100.0f;
        cout << fixed << setprecision(4);
        cout << "Percentage: " << percentage << "%" << endl;
        
        // Second pass: display matching transactions (up to 10)
        cout << "\n--- Showing up to 10 filtered transactions ---" << endl;
        current = Entry;
        int displayed = 0;
        
        while (current != nullptr && displayed < 10) {
            string transactionCategory = current->Category;
            string transactionPayment = current->PaymentMethod;
            
            transform(transactionCategory.begin(), transactionCategory.end(), 
                     transactionCategory.begin(), ::tolower);
            transform(transactionPayment.begin(), transactionPayment.end(), 
                     transactionPayment.begin(), ::tolower);
            
            if (transactionCategory == lowerUserCategory && 
                transactionPayment == lowerUserPayment) {
                // Format the output to match your example
                cout << "Date: " << current->Date 
                     << " | CustomerID: " << current->CusID
                     << " | Product: " << current->Product
                     << " | Category: " << current->Category
                     << " | Price: RM" << fixed << setprecision(2) << current->Price
                     << " | Payment: " << current->PaymentMethod << endl;
                displayed++;
            }
            current = current->next;
        }
    }
    
    int countNodes() {
        int count = 0;
        Transaction* current = Entry;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        cout << "Current Number of Valid Transactions: "<< count << endl;
        cout << "Space used: " << count * sizeof(Transaction) << " bytes" << endl;
        return count;
    }

    bool LoadTransactionsFromCSV(const string& filename, TransactionsLink& list) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }
        
        string line;
        // Skip header line
        getline(file, line);
        
        // Process each line
        while (getline(file, line)) {
            stringstream ss(line);
            string prodID, prod, cat, Date, payment, priceStr;
            
            // Parse CSV line
            getline(ss, prodID, ',');
            getline(ss, prod, ',');
            getline(ss, cat, ',');
            getline(ss, priceStr, ',');
            getline(ss, Date, ',');
            getline(ss, payment, ','); 
        
            // Convert rating to integer
            float price = stof(priceStr);
            
            // Add to linked list
            list.AddNew(prodID, prod, cat, price, Date, payment);
        }
        
        file.close();
        return true;
    }

};

struct TransactionsArray {
    Transaction transactions[MAX_TRANSACTIONS];
    int size = 0;
    int sortType = 0; 

    bool LoadTransactionsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }
    
        string line;
        getline(file, line); // skip header
    
        while (getline(file, line) && size < MAX_TRANSACTIONS) {
            stringstream ss(line);
            string customerID, product, category, priceStr, Date, payment;
            getline(ss, customerID, ',');
            getline(ss, product, ',');
            getline(ss, category, ',');
            getline(ss, priceStr, ',');
            getline(ss, Date, ',');
            getline(ss, payment);
    
            try {
                // Create a Transaction object first
                Transaction newTransaction;
                newTransaction.CusID = customerID;
                newTransaction.Product = product;
                newTransaction.Category = category;
                newTransaction.Price = stof(priceStr);
                newTransaction.Date = Date;
                newTransaction.PaymentMethod = payment;
                
                // Then assign it to the array
                transactions[size++] = newTransaction;
            } catch (...) {
                continue;
            }
        }
        return true;
    }

    void bubbleSortByDate() {
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                if (transactions[j].Date > transactions[j + 1].Date) {
                    swap(transactions[j], transactions[j + 1]);
                }
            }
        }
        sortType = 1;
    }

    void insertionSortByDate() {
        for (int i = 1; i < size; ++i) {
            Transaction key = transactions[i];
            int j = i - 1;
            while (j >= 0 && transactions[j].Date > key.Date) {
                transactions[j + 1] = transactions[j];
                --j;
            }
            transactions[j + 1] = key;
        }
        sortType = 2;
    }

    void displayTransactions() const {
        cout << "\n--- Transactions Sorted by Date ---\n";
        for (int i = 0; i < size; ++i) {
            const auto& t = transactions[i];
            cout << "Date: " << t.Date
                 << " | CustomerID: " << t.CusID
                 << " | Product: " << t.Product
                 << " | Category: " << t.Category
                 << " | Price: RM" << t.Price
                 << " | Payment: " << t.PaymentMethod << "\n";
        }
        cout << "Total Number of Transactions: " << size << endl;
    }

    void categoryPercentage(string userCat, string userPayment) {
        int categoryTotal = 0;
        int categoryPayment = 0;
        Transaction toDisplay[10]; // only store 10 for display
        int displayIndex = 0;
    
        // Convert search terms to lowercase once
        string lowerUserCategory = userCat;
        string lowerUserPayment = userPayment;
        transform(lowerUserCategory.begin(), lowerUserCategory.end(), 
                 lowerUserCategory.begin(), ::tolower);
        transform(lowerUserPayment.begin(), lowerUserPayment.end(), 
                 lowerUserPayment.begin(), ::tolower);
    
        // First pass: count matching transactions
        for (int i = 0; i < size; ++i) {
            string transactionCategory = transactions[i].Category;
            string transactionPayment = transactions[i].PaymentMethod;
            
            transform(transactionCategory.begin(), transactionCategory.end(), 
                     transactionCategory.begin(), ::tolower);
            transform(transactionPayment.begin(), transactionPayment.end(), 
                     transactionPayment.begin(), ::tolower);
            
            if (transactionCategory == lowerUserCategory) {
                categoryTotal++;
                if (transactionPayment == lowerUserPayment) {
                    categoryPayment++;
                    if (displayIndex < 10) {
                        toDisplay[displayIndex++] = transactions[i];
                    }
                }
            }
        }
    
        // Output statistics
        cout << "Category: " << userCat << " | Payment Method: " << userPayment << endl;
        cout << "Matching transactions: " << categoryPayment << " (" << userPayment << ") out of " 
             << categoryTotal << " (" << userCat << ")" << endl;
        
        if (categoryTotal == 0) {
            cout << "Percentage: 0%" << endl;
            cout << "No matching transactions found." << endl;
            return;
        }
        
        float percentage = (static_cast<float>(categoryPayment) / categoryTotal) * 100.0f;
        cout << fixed << setprecision(4);
        cout << "Percentage: " << percentage << "%" << endl;
        
        // Display matching transactions
        if (categoryPayment > 0) {
            cout << "\n--- Showing up to 10 filtered transactions ---" << endl;
            for (int i = 0; i < displayIndex; ++i) {
                const auto& t = toDisplay[i];
                cout << "Date: " << t.Date
                     << " | CustomerID: " << t.CusID
                     << " | Product: " << t.Product
                     << " | Category: " << t.Category
                     << " | Price: RM" << fixed << setprecision(2) << t.Price
                     << " | Payment: " << t.PaymentMethod << endl;
            }
        } else {
            cout << "No matching transactions found." << endl;
        }
    }

    void filterByDateRange(string startDate, string endDate) const {
        cout << "\n--- Transactions from " << startDate << " to " << endDate << " ---\n";
        bool found = false;
        for (int i = 0; i < size; ++i) {
            if (transactions[i].Date >= startDate && transactions[i].Date <= endDate) {
                const auto& t = transactions[i];
                cout << "Date: " << t.Date
                     << " | CustomerID: " << t.CusID
                     << " | Product: " << t.Product
                     << " | Category: " << t.Category
                     << " | Price: RM" << t.Price
                     << " | Payment: " << t.PaymentMethod << "\n";
                found = true;
            }
        }
        if (!found) cout << "No transactions found in the given range.\n";
    }
};

struct ReviewLink{
    Review* head;
    WordNode* wordHead; // Head of our word frequency list

    ReviewLink() { 
        head = nullptr; 
        wordHead = nullptr;
    }

    //Add Review
    void AddReview(string prodID, string custID, int rating, string reviewTxt){
        Review* newReview = new Review();

        // Set the data fields
        newReview->ProdID = prodID;
        newReview->CustID = custID;
        newReview->Rating = rating;
        newReview->ReviewTxt = reviewTxt;
        newReview->next = nullptr;

        if (head == nullptr) {
            head = newReview;
        } 
        // Otherwise find the end and append
        else {
            Review* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newReview;
        }
    }

    void displayReview(){
        if(head == nullptr){
            cout << "List is empty, no reviews to display." << endl;
            return;
        }

        Review* current = head;
        int count = 0;
        
        while (current != nullptr) {
            count++;
            cout << count << ". Product: " << current->ProdID 
                 << ", Customer: " << current->CustID
                 << ", Rating: " << current->Rating
                 << ", Review: \"" << current->ReviewTxt << "\"" << endl;
            current = current->next;
        }
        
        cout << "Total reviews: " << count << endl;
        cout << "Space used: " << count * sizeof(Review) << " bytes" << endl;
    }

    bool LoadReviewsFromCSV(const string& filename, ReviewLink& list) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }
        
        string line;
        getline(file, line);
        
        // Process each line
        while (getline(file, line)) {
            stringstream ss(line);
            string prodID, custID, ratingStr, reviewTxt;
            
            // Parse CSV line
            getline(ss, prodID, ',');
            getline(ss, custID, ',');
            getline(ss, ratingStr, ',');
            getline(ss, reviewTxt);  // Get the rest of the line for review text
            
            // Remove leading and trailing quotes, if present
            if (!reviewTxt.empty() && reviewTxt.front() == '"' && reviewTxt.back() == '"') {
                reviewTxt = reviewTxt.substr(1, reviewTxt.size() - 2);
            }
            
            // Convert rating to integer
            int rating = stoi(ratingStr);
            
            // Add to linked list
            list.AddReview(prodID, custID, rating, reviewTxt);
        }
        
        file.close();
        return true;
    }

        // Helper function to convert string to lowercase
    string toLower(string str) {
        for (char &c : str) {
            c = tolower(c);
        }
        return str;
    }

    // Helper function to check if a character is a letter
    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    // Function to add or upDate word in our frequency list
    void addWord(string word) {
        if (word.empty() || word.length() <= 2) return;

        // Convert to lowercase for consistent counting
        for (char& c : word) c = tolower(c);

        WordNode* current = wordHead;
        WordNode* prev = nullptr;

        // Search for existing word
        while (current != nullptr) {
            if (current->word == word) {
                current->count++;
                return;
            }
            prev = current;
            current = current->next;
        }

        // Word not found - add new node
        WordNode* newNode = new WordNode{word, 1, nullptr};
        
        if (prev == nullptr) {
            wordHead = newNode;  // First word in list
        } else {
            prev->next = newNode;  // Append to end
        }
    }


    // Function to sort word frequency list (using bubble sort)
    void sortWordFrequency() {
        if (wordHead == nullptr || wordHead->next == nullptr) return;

        bool swapped;
        WordNode* ptr1;
        WordNode* lptr = nullptr;

        do {
            swapped = false;
            ptr1 = wordHead;

            while (ptr1->next != lptr) {
                if (ptr1->count < ptr1->next->count) {
                    // Swap data
                    swap(ptr1->word, ptr1->next->word);
                    swap(ptr1->count, ptr1->next->count);
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

    // New function to find most frequent words in 1-star reviews
    void sortTop10Words(int topN = 10) {
        if (head == nullptr) {
            cout << "No reviews available." << endl;
            return;
        }

        // Reset word frequency list
        while (wordHead != nullptr) {
            WordNode* temp = wordHead;
            wordHead = wordHead->next;
            delete temp;
        }

        int oneStarCount = 0;
        Review* currentReview = head;

        while (currentReview != nullptr) {
            if (currentReview->Rating == 1) {
                oneStarCount++;
                string rawText = currentReview->ReviewTxt;

                string cleanText;
                for (char c : rawText) {
                    if (isalpha(c)) {
                        cleanText += tolower(c);
                    } else {
                        cleanText += ' ';
                    }
                }

                stringstream ss(cleanText);
                string word;
                while (ss >> word) {
                    addWord(word);
                }
            }
            currentReview = currentReview->next;
        }

        sortWordFrequency();

        // Display final results
        cout << "\nTop 10 most frequent words in 1-star reviews (" << oneStarCount << " reviews found, Linked List):\n";
        cout << "----------------------------------------\n";
        cout << left << setw(6) << "Rank" << setw(16) << "Word" << "Frequency\n";
        cout << "----------------------------------------\n";
        
        WordNode* current = wordHead;
        for (int i = 0; i < topN && current != nullptr; i++, current = current->next) {
            cout << left << setw(6) << (to_string(i+1) + ".") 
                 << setw(16) << current->word 
                 << current->count << endl;
        }

        // Cleanup
        while (wordHead != nullptr) {
            WordNode* temp = wordHead;
            wordHead = wordHead->next;
            delete temp;
        }
    }

};

struct ReviewArray {
    Review reviews[MAX_REVIEWS];
    int size = 0;

    void AddReview(const string& prodID, const string& custID, int rating, const string& reviewTxt) {
        if (size < MAX_REVIEWS) {
            reviews[size].ProdID = prodID;
            reviews[size].CustID = custID;
            reviews[size].Rating = rating;
            reviews[size].ReviewTxt = reviewTxt;
            size++;
        }
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
                AddReview(prodID, custID, rating, reviewTxt);
            } catch (...) {
                continue;
            }
        }

        file.close();
        return true;
    }

    void displayReview() {
        if (size == 0) {
            cout << "No reviews to display." << endl;
            return;
        }

        for (int i = 0; i < size; ++i) {
            cout << i + 1 << ". Product: " << reviews[i].ProdID
                 << ", Customer: " << reviews[i].CustID
                 << ", Rating: " << reviews[i].Rating
                 << ", Review: \"" << reviews[i].ReviewTxt << "\"" << endl;
        }

        cout << "Total reviews: " << size << endl;
    }

    string CleanWord(string word) {
        string clean;
        for (char c : word) {
            if (isalnum(c)) clean += tolower(c);
        }
        return clean;
    }

    void sortTop10Words(int topN = 10) {
        WordCount wordList[MAX_WORDS];
        int wordCountSize = 0;
        int oneStarCount = 0;

        for (int i = 0; i < size; ++i) {
            if (reviews[i].Rating == 1) {
             oneStarCount++;
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
                        wordList[wordCountSize].word = word;
                        wordList[wordCountSize].count = 1;
                        wordCountSize++;
                    }
                }
            }
        }

        // Sort descending by frequency
        for (int i = 0; i < wordCountSize - 1; ++i) {
            for (int j = 0; j < wordCountSize - i - 1; ++j) {
                if (wordList[j].count < wordList[j + 1].count) {
                    WordCount temp = wordList[j];
                    wordList[j] = wordList[j + 1];
                    wordList[j + 1] = temp;
                }
            }
        }

        cout << "\nTop 10 most frequent words in 1-star reviews (" << oneStarCount << " reviews found, Array):\n";
        cout << "---------------------------------------------------------------\n";
        cout << "Rank   Word         Frequency\n";
        cout << "---------------------------------------------------------------\n";

        for (int i = 0; i < topN && i < wordCountSize; ++i) {
            cout.width(6); cout << left << (to_string(i + 1) + ".");
            cout.width(13); cout << left << wordList[i].word;
            cout << wordList[i].count << endl;
        }
    }
};

int main(){
    system("cls");
    ReviewArray revArray;
    ReviewLink revList;
    TransactionsLink traList;
    TransactionsArray traArray;

    string revFile = "reviews_clean.csv";
    string traFile = "transactions_clean.csv";
    revArray.LoadReviewsFromCSV(revFile);
    revList.LoadReviewsFromCSV(revFile, revList);
    traList.LoadTransactionsFromCSV(traFile, traList);
    traArray.LoadTransactionsFromCSV(traFile);
    int array_or_linklist, rev_or_tra, operationChoice;
    int SizeRevArray = sizeof(revArray.reviews);
    
    while(true) {
        cout << "\nChoose the structure you want to use:\n";
        cout << "1. Array\n";
        cout << "2. Linked List\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        
        // ValiDate input
        if (!(cin >> array_or_linklist)) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a number (1-3).\n";
            system("pause");
            continue;
        }
        
        if (array_or_linklist == 3) {
            break; // Exit program
        }
        
        if (array_or_linklist != 1 && array_or_linklist != 2) {
            cout << "Invalid choice. Please enter 1, 2, or 3.\n";
            system("pause");
            continue;
        }
        
        cout << (array_or_linklist == 1 ? "\nArray" : "\nLinked List") << endl;
        cout << "1. Reviews\n";
        cout << "2. Transactions\n";
        cout << "3. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> rev_or_tra;
        
        if(rev_or_tra == 3) {
            continue; 
        }
        
        if(rev_or_tra != 1 && rev_or_tra != 2) {
            cout << "Invalid choice. Please try again.\n";
            system("pause");
            continue;
        }
        
        // Reviews Operations
        if(rev_or_tra == 1) while (true) {
            cout << (array_or_linklist == 1 ? "\nArray: Reviews" : "\nLinked List: Reviews") << endl;
            cout << "1. Sort Top 10 Words Used in 1-Star Reviews\n";
            cout << "2. Display Reviews\n";
            cout << "3. Clear Terminal\n";
            cout << "4. Back\n";
            cout << "Enter your choice: ";
            
            // validate input
            if (!(cin >> operationChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice.\n";
                system("pause");
                continue;
            }
            
            if (operationChoice == 4) {
                break; 
            }
        
            if (operationChoice == 3) {
                system("cls");
                continue;
            }
            
            if (operationChoice == 1) {
                if (array_or_linklist == 1) {
                    auto start = high_resolution_clock::now();
                    revArray.sortTop10Words(10);
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    cout << "Time taken: " << duration.count() << " microseconds" << endl;
                } else {
                    auto start = high_resolution_clock::now();
                    revList.sortTop10Words(10);
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    cout << "Time taken: " << duration.count() << " microseconds" << endl;
                }
            } 
            else if (operationChoice == 2) {
                if (array_or_linklist == 1) {
                    auto start = high_resolution_clock::now();
                    revArray.displayReview();
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    cout << "Space used: " << SizeRevArray << " bytes" << endl;
                    cout << "Time taken: " << duration.count() << " microseconds" << endl;
                } else {
                    auto start = high_resolution_clock::now();
                    revList.displayReview();
                    auto end = high_resolution_clock::now();
                    auto duration = duration_cast<microseconds>(end - start);
                    cout << "Time taken: " << duration.count() << " microseconds" << endl;
                }
            }
            else {
                cout << "Invalid choice.\n";
                system("pause");
                system("cls");
                continue; 
            }
        }
        // Transactions Operations
        else if(rev_or_tra == 2) {
            while (true) {
                cout << (array_or_linklist == 1 ? "\nArray: Transactions" : "\nLinked List: Transactions") << endl;
                cout << "1. Sort Customer Transactions by Date (BubbleSort)\n";
                cout << "2. Sort Customer Transactions by Date (InsertionSort)\n";
                cout << "3. Search and Filter Transactions by Category and Payment Method\n";
                cout << "4. Filter transactions by Date range\n";
                cout << "5. Display Transactions\n";
                cout << "6. Clear Terminal\n";
                cout << "7. Back\n";
                cout << "Enter your choice: ";
                
                // validate input
                if (!(cin >> operationChoice)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << "Invalid choice.\n";
                    system("pause");
                    system("cls");
                    continue;
                }
                
                if (operationChoice == 7) {
                    break;
                }
            
                if (operationChoice == 6) {
                    system("cls");
                    continue; 
                }
                
                if (operationChoice == 1) {
                    if (array_or_linklist == 1) {
                        auto start = high_resolution_clock::now();
                        traArray.bubbleSortByDate();
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Bubble sort by Date took(Array): " << duration.count() << " microseconds" << endl;
                    } else {
                        auto start = high_resolution_clock::now();
                        traList.bubbleSortByDate();
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Bubble sort by Date took(Linked List): " << duration.count() << " microseconds" << endl;
                    }
                } 
                else if (operationChoice == 2) {
                    if (array_or_linklist == 1) {
                        auto start = high_resolution_clock::now();
                        traArray.insertionSortByDate();
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Insertion sort by Date took(Array): " << duration.count() << " microseconds" << endl;
                    } else {
                        auto start = high_resolution_clock::now();
                        traList.insertionSortByDate();
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Insertion sort by Date took(Linked List): " << duration.count() << " microseconds" << endl;
                    }
                }
                else if (operationChoice == 3) {
                    if (array_or_linklist == 1) {
                        string userCategory, userPayment;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter the category you want to analyze: ";
                        while(getline(cin, userCategory) && userCategory.empty()) {
                            cout << "Category cannot be empty. Please try again: ";
                        }
                        
                        cout << "Enter the payment method you want to analyze: ";
                        while(getline(cin, userPayment) && userPayment.empty()) {
                            cout << "Payment method cannot be empty. Please try again: ";
                        }
                        
                        auto start = high_resolution_clock::now();
                        traArray.categoryPercentage(userCategory, userPayment);
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Time taken: " << duration.count() << " microseconds" << endl;
                    } else {
                        string userCategory, userPayment;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter the category you want to analyze: ";
                        while(getline(cin, userCategory) && userCategory.empty()) {
                            cout << "Category cannot be empty. Please try again: ";
                        }
                        
                        cout << "Enter the payment method you want to analyze: ";
                        while(getline(cin, userPayment) && userPayment.empty()) {
                            cout << "Payment method cannot be empty. Please try again: ";
                        }
                        
                        auto start = high_resolution_clock::now();
                        traList.categoryPercentage(userCategory, userPayment);
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Time taken: " << duration.count() << " microseconds" << endl;
                    }
                }
                else if (operationChoice == 4) {
                    if (array_or_linklist == 1) {
                        string startDate, endDate;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter start Date (YYYY-MM-DD): ";
                        while(getline(cin, startDate) && startDate.empty()) {
                            cout << "Date cannot be empty ";
                        }
                        
                        cout << "Enter end Date (YYYY-MM-DD): ";
                        while(getline(cin, endDate) && endDate.empty()) {
                            cout << "Date cannot be empty ";
                        }
                        
                        auto start = high_resolution_clock::now();
                        traArray.filterByDateRange(startDate, endDate);
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Time taken: " << duration.count() << " microseconds" << endl;
                    } else {
                        string startDate, endDate;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Enter start Date (YYYY-MM-DD): ";
                        while(getline(cin, startDate) && startDate.empty()) {
                            cout << "Date cannot be empty ";
                        }
                        
                        cout << "Enter end Date (YYYY-MM-DD): ";
                        while(getline(cin, endDate) && endDate.empty()) {
                            cout << "Date cannot be empty ";
                        }
                        
                        auto start = high_resolution_clock::now();
                        traList.filterByDateRange(startDate, endDate);
                        auto end = high_resolution_clock::now();
                        auto duration = duration_cast<microseconds>(end - start);
                        cout << "Time taken: " << duration.count() << " microseconds" << endl;
                    }
                }
                else if (operationChoice == 5) {
                    if (array_or_linklist == 1) {
                        traArray.displayTransactions();
                    } else {
                        traList.displayTransactions();
                    }
                }
                else {
                    cout << "Invalid choice.\n";
                    system("pause");
                    system("cls");
                    continue;
                }
            }
        }
    }    
    return 0;    
}
