#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;

const int MAX_REVIEWS = 10000;
const int MAX_WORDS = 1000;

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
        } else {
            cout << "Error: Reached maximum number of reviews." << endl;
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
                cout << "Warning: Skipping invalid line." << endl;
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
                        wordList[wordCountSize].word = word;
                        wordList[wordCountSize].count = 1;
                        wordCountSize++;
                    }
                }
            }
        }

        // Simple bubble sort (descending order)
        for (int i = 0; i < wordCountSize - 1; ++i) {
            for (int j = 0; j < wordCountSize - i - 1; ++j) {
                if (wordList[j].count < wordList[j + 1].count) {
                    WordCount temp = wordList[j];
                    wordList[j] = wordList[j + 1];
                    wordList[j + 1] = temp;
                }
            }
        }

        cout << "\nTop " << topN << " most frequent words in 1-star reviews:\n";
        for (int i = 0; i < topN && i < wordCountSize; ++i) {
            cout << wordList[i].word << ": " << wordList[i].count << " times\n";
        }
    }
};

int main() {
    ReviewArray reviews;
    string filename = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/reviews_clean.csv";

    if (reviews.LoadReviewsFromCSV(filename)) {
        cout << "Reviews loaded successfully!\n" << endl;
        reviews.DisplayReviews();
        reviews.MostFrequentWordsIn1StarReviews();
    } else {
        cout << "Failed to load reviews." << endl;
    }

    return 0;
}
