#include <iostream>
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

    void AddNew(string CusID0, string Product0, string Category0, string Date0, string PaymentMethod0, float Price0){
        if (Entry == nullptr){
            Entry = new Transaction();
            Entry->CusID = CusID0;
            Entry->Product = Product0;
            Entry->Category = Category0;
            Entry->Date = Date0;
            Entry->PaymentMethod = PaymentMethod0;
            Entry->Price = Price0;
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
            NewEntry->Date = Date0;
            NewEntry->PaymentMethod = PaymentMethod0;
            NewEntry->Price = Price0; 
            NewEntry->next = nullptr;
            FindEnd->next = NewEntry;              
        }
    }

    void displaynodes(){
        Transaction* FindEnd = Entry;

        while(FindEnd->next != nullptr){
            cout << "CusID: " << FindEnd->CusID
            << ", Product: " << FindEnd->Product
            << ", Category: " << FindEnd->Category
            << ", Date: " << FindEnd->Date
            << ", PaymentMethod: " << FindEnd->PaymentMethod
            << ", Price: " << FindEnd->Price << endl;            
            FindEnd = FindEnd->next;
        }
    }


};

int main(){
    TransactionLinked* Start = new TransactionLinked();
    Start->AddNew("C001", "Laptop", "Electronics", "2024-04-01", "Card", 1200.0f);
    Start->AddNew("C002", "Headphones", "Accessories", "2024-04-02", "Cash", 150.0f);
    Start->AddNew("C003", "Tablet", "Electronics", "2024-04-03", "Card", 500.0f);
    Start->AddNew("C004", "Shoes", "Apparel", "2024-04-04", "Online", 80.0f);
    Start->AddNew("C005", "Watch", "Accessories", "2024-04-05", "Card", 300.0f);
    Start->displaynodes();
    return 0;
}
