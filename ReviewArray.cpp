#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct Review {
    string ProdID;
    string CustID;
    int Rating;
    string ReviewTxt;
};

class ReviewArray {
private:
    vector<Review> reviews;

public:
    // Add review to array
    void AddReview(const string& prodID, const string& custID, int rating, const string& reviewTxt) {
        Review r;
        r.ProdID = prodID;
        r.CustID = custID;
        r.Rating = rating;
        r.ReviewTxt = reviewTxt;
        reviews.push_back(r);
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

            // Remove surrounding quotes if any
            if (!reviewTxt.empty() && reviewTxt.front() == '"' && reviewTxt.back() == '"') {
                reviewTxt = reviewTxt.substr(1, reviewTxt.size() - 2);
            }

            int rating = stoi(ratingStr);
            AddReview(prodID, custID, rating, reviewTxt);
        }

        file.close();
        return true;
    }

    // Display reviews
    void DisplayReviews() const {
        if (reviews.empty()) {
            cout << "No reviews to display." << endl;
            return;
        }

        cout << "Reviews in array:\n---------------\n";
        int count = 0;
        for (const auto& r : reviews) {
            count++;
            cout << count << ". Product: " << r.ProdID
                 << ", Customer: " << r.CustID
                 << ", Rating: " << r.Rating
                 << ", Review: \"" << r.ReviewTxt << "\"\n";
        }

        cout << "Total reviews: " << count << endl;
    }

    int GetTotalReviews() const {
        return reviews.size();
    }
};

int main() {
    ReviewArray myReviewArray;
    string filename = "C:/Users/kenji/OneDrive/Documents/Uni/SEM 4/DS/Project 1/reviews_clean.csv";

    if (myReviewArray.LoadReviewsFromCSV(filename)) {
        cout << "Reviews loaded successfully!\n";
        myReviewArray.DisplayReviews();
    } else {
        cout << "Failed to load reviews.\n";
    }

    return 0;
}
