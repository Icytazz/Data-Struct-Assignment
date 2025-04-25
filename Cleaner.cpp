#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

bool is_valid_rating(const std::string& rating) {
    try {
        int r = std::stoi(rating);
        return r >= 1 && r <= 5;
    } catch (...) {
        return false;
    }
}

std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

int main() {
    std::ifstream infile("reviews.csv");
    std::ofstream outfile("cleaned_reviews.csv");

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(infile, line)) {
        if (isHeader) {
            outfile << line << "\n";  // Copy header
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string productId, customerId, rating, reviewText;

        std::getline(ss, productId, ',');
        std::getline(ss, customerId, ',');
        std::getline(ss, rating, ',');
        std::getline(ss, reviewText);

        productId = trim(productId);
        customerId = trim(customerId);
        rating = trim(rating);

        // Drop if Product ID, Customer ID, or Rating is invalid
        if (productId.empty() || customerId.empty() || !is_valid_rating(rating)) {
            continue;
        }

        outfile << productId << "," << customerId << "," << rating << "," << reviewText << "\n";
    }

    infile.close();
    outfile.close();

    std::cout << "Cleaned data saved to 'cleaned_reviews.csv'.\n";
    return 0;
}
