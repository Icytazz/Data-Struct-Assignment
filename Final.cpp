#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>
using namespace std;

const int MAX_REVIEWS = 10000;
const int MAX_WORDS = 1000;

struct Transaction{
    Transaction* next;
    string CusID;
    string Product;
    string Category;
    string Date;
    string PaymentMethod;
    float Price;
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

    void displaynodes(){
        Transaction* FindEnd = Entry;
        if(Entry == nullptr){
            cout << "List is empty, no reviews to display." << endl;
            return;
        }

        while (FindEnd != nullptr){
            cout << "CusID: " << FindEnd->CusID
            << ", Product: " << FindEnd->Product
            << ", Category: " << FindEnd->Category
            << ", Date: " << FindEnd->Date
            << ", Payment Method: " << FindEnd->PaymentMethod
            << ", Price: " << FindEnd->Price << endl;            
            FindEnd = FindEnd->next;
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
                    // if first date is smaller, go to next node (current node becomes prev)
                    prev = ptr1;
                    ptr1 = ptr1->next;
                }
            }
            lptr = ptr1;
        } while (swapped);
    }
    
    int countNodes() {
        int count = 0;
        Transaction* current = Entry;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        cout << "Current Number of Valid Transactions: "<< count << endl;
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
            string prodID, prod, cat, date, payment, priceStr;
            
            // Parse CSV line
            getline(ss, prodID, ',');
            getline(ss, prod, ',');
            getline(ss, cat, ',');
            getline(ss, priceStr, ',');
            getline(ss, date, ',');
            getline(ss, payment, ','); 
        
            // Convert rating to integer
            float price = stof(priceStr);
            
            // Add to linked list
            list.AddNew(prodID, prod, cat, price, date, payment);
        }
        
        file.close();
        return true;
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
    }

    bool LoadReviewsFromCSV(const string& filename, ReviewLink& list) {
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

    // Function to add or update word in our frequency list
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

    void debugPrintWordList() {
        cout << "\nDEBUG - Current Word List:\n";
        WordNode* current = wordHead;
        while (current != nullptr) {
            cout << "'" << current->word << "': " << current->count << endl;
            current = current->next;
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
    void findMostFrequentWordsIn1StarReviews(int topN = 10) {
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
                cout << "\nDEBUG - Processing review: " << rawText << endl;  // Debug output

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
                    cout << "DEBUG - Found word: " << word << endl;  // Debug output
                    addWord(word);
                }
            }
            currentReview = currentReview->next;
        }

        debugPrintWordList();  // Show word list before sorting

        if (oneStarCount == 0) {
            cout << "No 1-star reviews found." << endl;
            return;
        }

        sortWordFrequency();

        // Display final results
        cout << "\nTop " << topN << " most frequent words in 1-star reviews:\n";
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

    void DisplayReviews() {
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

    void MostFrequentWordsIn1StarReviews(int topN = 10) {
        WordCount wordList[MAX_WORDS];
        int wordCountSize = 0;
        int total1Star = 0;

        for (int i = 0; i < size; ++i) {
            if (reviews[i].Rating == 1) {
                total1Star++;
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

        cout << "\nTop 10 most frequent words in 1-star reviews (" << total1Star << " reviews found):\n";
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
    ReviewArray revArray;
    ReviewLink revLink;
    TransactionsLink traLink;
    string revFile = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/reviews_clean.csv";
    string traFile = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/transactions_clean.csv";

    
}
