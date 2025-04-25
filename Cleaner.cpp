#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool is_valid_rating(const string& rating) {
    try {
        int r = stoi(rating);
        return r >= 1 && r <= 5;
    } catch (...) {
        return false;
    }
}

string trim(const string& str) {
    const string whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

int main() {
    ifstream infile("reviews.csv");
    ofstream outfile("cleaned_reviews.csv");

    if (!infile.is_open() || !outfile.is_open()) {
        cerr << "Error opening file.\n";
        return 1;
    }

    string line;
    bool isHeader = true;

    while (getline(infile, line)) {
        if (isHeader) {
            outfile << line << "\n";  // Keep header
            isHeader = false;
            continue;
        }

        stringstream ss(line);
        string productId, customerId, rating, reviewText;

        getline(ss, productId, ',');
        getline(ss, customerId, ',');
        getline(ss, rating, ',');
        getline(ss, reviewText); // Rest is review text

        // Trim all fields
        productId = trim(productId);
        customerId = trim(customerId);
        rating = trim(rating);
        reviewText = trim(reviewText);

        // Drop invalid rows
        if (productId.empty() || customerId.empty() || !is_valid_rating(rating) || reviewText.empty()) {
            continue;
        }

        outfile << productId << "," << customerId << "," << rating << "," << reviewText << "\n";
    }

    infile.close();
    outfile.close();

    cout << "Cleaned data saved to 'cleaned_reviews.csv'.\n";
    return 0;
}
