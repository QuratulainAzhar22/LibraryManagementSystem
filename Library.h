#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "User.h"
#include "Book.h"

extern "C" int __stdcall calculateFine(int days);

class Library {
    std::vector<User> users;
    std::vector<Book> books;

public:
    Library();
    User login();
    void showBooks();
    void searchBooks();
    void showUserIssuedBooks(int userId);
    void issueBook(int userId);
    void returnBook(int userId);
};

#endif
