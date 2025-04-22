#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Review{
    string ProdID;
    string CustID;
    int Rating;
    string ReviewTxt;
    Review* next;
};

struct ReviewLink{
    Review* head;

    //Initialize list
    ReviewLink() { head = nullptr; }

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
        
        cout << "Reviews in list:" << endl;
        cout << "---------------" << endl;
        
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

};

int main() {
    ReviewLink myReviews;
    string filename = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/reviews_clean.csv";
    
    if (myReviews.LoadReviewsFromCSV(filename, myReviews)) {
        cout << "Reviews loaded successfully!" << endl;
        myReviews.displayReview();
    } else {
        cout << "Failed to load reviews." << endl;
    }
    
    return 0;
}