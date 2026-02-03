#include <iostream>
#include "Library.h"
#include "User.h"
using namespace std;

int main() {
    Library lib;
   User u = lib.login();
    int choice;

    do {
        cout << "\n--- Library Menu ---\n";
        cout << "1. Show all books\n";
        cout << "2. Search book\n";
        cout << "3. Show my issued books\n";
        cout << "4. Issue a book\n";
        cout << "5. Return a book\n";
        cout << "0. Logout\nChoice: ";
        cin >> choice;
       
        switch (choice) {
        case 1: lib.showBooks(); break;
        case 2: lib.searchBooks(); break;
        case 3: lib.showUserIssuedBooks(u.id); break;
        case 4: lib.issueBook(u.id); break;
        case 5: lib.returnBook(u.id); break;
        case 0: cout << "Logging out...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
