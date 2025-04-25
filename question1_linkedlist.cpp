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
    string filename = "transactions_clean.csv";
    
    if (myTransactions.LoadReviewsFromCSV(filename, myTransactions)) {
        cout << "Reviews loaded successfully!" << endl;
        myTransactions.displaynodes();
        myTransactions.bubbleSortByDate();
        myTransactions.displaynodes();
        myTransactions.countNodes();
    } else {
        cout << "Failed to load reviews." << endl;
    }
    
    return 0;
}
