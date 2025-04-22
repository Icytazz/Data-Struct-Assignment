#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Transaction{
    Transaction* next;
    string CusID;
    string Product;
    string Category;
    string Date;
    string PaymentMethod;
    float Price;
};

struct TransactionLinked{
    Transaction* Entry;

    TransactionLinked() { Entry = nullptr; }

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

    bool LoadReviewsFromCSV(const string& filename, TransactionLinked& list) {
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

int main(){
    TransactionLinked myTransactions;
    string filename = "C:/Users/User/OneDrive - Asia Pacific University/SEM_4/Data_Structures/Assignment/transactions_clean.csv";
    
    if (myTransactions.LoadReviewsFromCSV(filename, myTransactions)) {
        cout << "Reviews loaded successfully!" << endl;
        myTransactions.displaynodes();
    } else {
        cout << "Failed to load reviews." << endl;
    }
    
    return 0;
}
