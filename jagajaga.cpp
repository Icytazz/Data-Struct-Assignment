// void call_functions(int array_or_linked, int review_or_transaction, ReviewArray revArray0, ReviewLink revList0, TransactionsLink traList0, TransactionsArray traArray0){
//     ReviewArray revArray            = revArray0;
//     ReviewLink revList              = revList0;
//     TransactionsLink traList        = traList0;
//     TransactionsArray traArray      = traArray0;

//     int SizeRevArray = sizeof(revArray.reviews);

//     int function;
//         // ValiDate input
//         if (review_or_transaction == 3) {
//             return; // Exit program

//         }else if (review_or_transaction != 1 && review_or_transaction != 2) {
//             cout << "Invalid choice. Please enter 1, 2, or 3.\n";
//             system("pause");
//             return;

//         }else if (review_or_transaction == 1){
//             while (true){
//                 cout << (array_or_linked == 1 ? "\nArray: Reviews" : "\nLinked List: Reviews") << endl;
//                 cout << "1. Sort Top 10 Words Used in 1-Star Reviews\n";
//                 cout << "2. Display Reviews\n";
//                 cout << "3. Clear Terminal\n";
//                 cout << "4. Back\n";
//                 cout << "Enter your choice: ";

//                 cin >> function;

//                 // Reviews
//                 if (array_or_linked == 1) {
//                     // Array
//                     if (function == 1){
//                         auto start = high_resolution_clock::now();
//                         revArray.sortTop10Words(10);
//                         auto end = high_resolution_clock::now();
//                         auto duration = duration_cast<microseconds>(end - start);
//                         cout << "Time taken: " << duration.count() << " microseconds" << endl;

//                     }else if(function == 2){
//                         auto start = high_resolution_clock::now();
//                         revArray.displayReview();
//                         auto end = high_resolution_clock::now();
//                         auto duration = duration_cast<microseconds>(end - start);
//                         cout << "Space used: " << SizeRevArray << " bytes" << endl;
//                         cout << "Time taken: " << duration.count() << " microseconds" << endl;

//                     }else if(function == 3){
//                         system("cls");

//                     }else if (function == 4){
//                         break;

//                     }
//                     // LinkedList
//                 }else if (array_or_linked == 2){
//                     if (function == 1){
//                         auto start = high_resolution_clock::now();
//                         revList.sortTop10Words(10);
//                         auto end = high_resolution_clock::now();
//                         auto duration = duration_cast<microseconds>(end - start);
//                         cout << "Time taken: " << duration.count() << " microseconds" << endl;

//                     }else if(function == 2){
//                         auto start = high_resolution_clock::now();
//                         revList.displayReview();
//                         auto end = high_resolution_clock::now();
//                         auto duration = duration_cast<microseconds>(end - start);
//                         cout << "Time taken: " << duration.count() << " microseconds" << endl;

//                     }else if(function == 3){
//                         system("cls");

//                     }else if (function == 4){
//                         break;
//                     }

//                 }
//             }   

//         }else if (review_or_transaction == 2){
//             cout << (array_or_linked == 1 ? "\nArray: Transactions" : "\nLinked List: Transactions") << endl;
//             cout << "1. Sort Customer Transactions by Date (BubbleSort)\n";
//             cout << "2. Sort Customer Transactions by Date (InsertionSort)\n";
//             cout << "3. Search and Filter Transactions by Category and Payment Method\n";
//             cout << "4. Filter transactions by Date range\n";
//             cout << "5. Display Transactions\n";
//             cout << "6. Clear Terminal\n";
//             cout << "7. Back\n";
//             cout << "Enter your choice: ";
//             // Array
//             if (array_or_linked == 1) {
//                 if (function == 1){
//                     cout << "Sorting Customer Transactions by Date (BubbleSort)\n";
//                     auto start = high_resolution_clock::now();
//                     traArray.BubbleSortByDate();
//                     auto end = high_resolution_clock::now();
//                     auto duration = duration_cast<microseconds>(end - start);
//                     cout << "Time taken: " << duration.count() << " microseconds" << endl;
//                     cout << "Invalid choice.\n";
//                 }else if(function == 2)
//             //Linked List
//             }else if (array_or_linked == 2) {}


//         }

    
    
// }

// void array_or_linked(ReviewArray revArray0, ReviewLink revList0, TransactionsLink traList0, TransactionsArray traArray0){
//     ReviewArray revArray            = revArray0;
//     ReviewLink revList              = revList0;
//     TransactionsLink traList        = traList0;
//     TransactionsArray traArray      = traArray0;
//     int array_or_linked, review_or_transaction;
//     while(true) {
//         cout << "Choose the structure you want to use:\n";
//         cout << "1. Array\n";
//         cout << "2. Linked List\n";
//         cout << "3. Exit\n";
//         cout << "Enter your choice: ";
//             // ValiDate input
//             if (!(cin >> array_or_linked)) {
//                 cin.clear(); // Clear error flag
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
//                 cout << "Invalid input. Please enter a number (1-3).\n";
//                 system("pause");
//                 continue;
//             }else if (array_or_linked == 3) {
//                 break; // Exit program
//             }else if (array_or_linked != 1 && array_or_linked != 2) {
//                 cout << "Invalid choice. Please enter 1, 2, or 3.\n";
//                 system("pause");
//                 continue;
//             } else{
//                 cout << (array_or_linked == 1 ? "Array" : "Linked List") << endl;
//                 cout << "1. Reviews\n";
//                 cout << "2. Transactions\n";
//                 cout << "3. Back to Main Menu\n";
//                 cout << "Enter your choice: ";
//                 cin >> review_or_transaction;        
//             }
//         call_functions(array_or_linked, review_or_transaction, revArray, revList, traList, traArray);
//     }

// }
