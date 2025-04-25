#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

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

int main() {
    ReviewLink myReviews;
    string filename = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/reviews_clean.csv";
    
    if (myReviews.LoadReviewsFromCSV(filename, myReviews)) {
        cout << "Reviews loaded successfully!" << endl;
        myReviews.findMostFrequentWordsIn1StarReviews();
    } else {
        cout << "Failed to load reviews." << endl;
    }
    
    return 0;
}
