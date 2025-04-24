#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
using namespace std;

const int MAX_REVIEWS = 1000;

struct Review {
    string ProdID;
    string CustID;
    int Rating;
    string ReviewTxt;
};

class ReviewArray {
private:
    Review reviews[MAX_REVIEWS];
    int size;

public:
    ReviewArray() {
        size = 0;
    }

    void AddReview(const string& prodID, const string& custID, int rating, const string& reviewTxt) {
        if (size >= MAX_REVIEWS) return; // silently ignore if full

        reviews[size].ProdID = prodID;
        reviews[size].CustID = custID;
        reviews[size].Rating = rating;
        reviews[size].ReviewTxt = reviewTxt;
        size++;
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

            int rating = stoi(ratingStr);
            AddReview(prodID, custID, rating, reviewTxt);
        }

        file.close();
        return true;
    }

    void DisplayReviewSummary() {
        cout << "Total number of reviews: " << size << endl;
    }

    void SearchByRating(int targetRating) {
        cout << "\nReviews with " << targetRating << "-star rating:\n";

        int count = 0;
        for (int i = 0; i < size; ++i) {
            if (reviews[i].Rating == targetRating) {
                cout << "- " << reviews[i].ReviewTxt << endl;
                count++;
            }
        }

        if (count == 0) {
            cout << "No reviews found with rating " << targetRating << "." << endl;
        } else {
            cout << "Total found: " << count << endl;
        }
    }

    void MostFrequentWordsIn1StarReviews() {
        struct WordFreq {
            string word;
            int count;
        };

        WordFreq wordCounts[500];
        int wordCountSize = 0;

        for (int i = 0; i < size; ++i) {
            if (reviews[i].Rating == 1) {
                stringstream ss(reviews[i].ReviewTxt);
                string word;
                while (ss >> word) {
                    for (char& c : word) {
                        c = tolower(c);
                        if (ispunct(c)) c = ' ';
                    }

                    string cleanedWord;
                    for (char c : word) {
                        if (!ispunct(c)) cleanedWord += c;
                    }

                    if (cleanedWord.empty()) continue;

                    bool found = false;
                    for (int j = 0; j < wordCountSize; ++j) {
                        if (wordCounts[j].word == cleanedWord) {
                            wordCounts[j].count++;
                            found = true;
                            break;
                        }
                    }
                    if (!found && wordCountSize < 500) {
                        wordCounts[wordCountSize].word = cleanedWord;
                        wordCounts[wordCountSize].count = 1;
                        wordCountSize++;
                    }
                }
            }
        }

        // Simple bubble sort by frequency
        for (int i = 0; i < wordCountSize - 1; ++i) {
            for (int j = 0; j < wordCountSize - i - 1; ++j) {
                if (wordCounts[j].count < wordCounts[j + 1].count) {
                    swap(wordCounts[j], wordCounts[j + 1]);
                }
            }
        }

        cout << "\nTop 10 most frequent words in 1-star reviews:\n";
        for (int i = 0; i < 10 && i < wordCountSize; ++i) {
            cout << wordCounts[i].word << ": " << wordCounts[i].count << " times" << endl;
        }
    }
};

int main() {
    ReviewArray reviews;
    string filename = "C:/path/to/reviews_clean.csv"; // 🔁 Update this to your real path

    if (reviews.LoadReviewsFromCSV(filename)) {
        cout << "✅ Reviews loaded successfully!" << endl;

        reviews.DisplayReviewSummary();
        reviews.MostFrequentWordsIn1StarReviews();
        reviews.SearchByRating(5); // 🔍 Example: find all 5-star reviews
    } else {
        cout << "❌ Failed to load reviews." << endl;
    }

    return 0;
}
