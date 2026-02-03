#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std;

// Fine calculation: 0-5 days = 10/day, 6-10 = 20/day, 11+ = 50/day
extern "C" int __stdcall calculateFine(int days) {
    if (days <= 0) return 0;
    if (days <= 5) return days * 10;
    if (days <= 10) return days * 20;
    return days * 50;
}

// Constructor: load users and books
Library::Library() {
    ifstream ufile("users.txt");
    int id; string username, password, role;
    while (ufile >> id >> username >> password >> role) {
        users.push_back(User(id, username, password, role));
    }
    ufile.close();

    ifstream bfile("books.txt");
    string line, title, author;
    int available;
    while (getline(bfile, line)) {
        stringstream ss(line);
        ss >> id;
        ss.ignore(); // skip space
        getline(ss, title, '"'); // skip first quote
        getline(ss, title, '"'); // read title
        ss.ignore(); // skip space
        getline(ss, author, '"'); // skip first quote
        getline(ss, author, '"'); // read author
        ss >> available;
        books.push_back(Book(id, title, author, available));
    }
    bfile.close();

    ofstream("issued.txt", ios::app).close(); // ensure exists
}

// User login
User Library::login() {
    string uname, pwd;
    cout << "Username: "; cin >> uname;
    cout << "Password: "; cin >> pwd;

    for (auto& u : users) {
        if (u.username == uname && u.password == pwd) {
            cout << "Login successful! Welcome, " << u.username << endl;
            return u;
        }
    }
    cout << "Invalid login!\n";
}

// Show all books
void Library::showBooks() {
    cout << "\nBooks in Library:\n";
    for (auto& b : books) {
        cout << "ID: " << b.id
            << " Title: " << b.title
            << " Author: " << b.author
            << " Available: " << (b.available ? "Yes" : "No") << endl;
    }
}

// Search books by title or author
void Library::searchBooks() {
    string keyword;
    cin.ignore();
    cout << "Enter keyword to search: ";
    getline(cin, keyword);

    cout << "\nSearch Results:\n";
    for (auto& b : books) {
        if (b.title.find(keyword) != string::npos || b.author.find(keyword) != string::npos) {
            cout << "ID: " << b.id
                << " Title: " << b.title
                << " Author: " << b.author
                << " Available: " << (b.available ? "Yes" : "No") << endl;
        }
    }
}

// Show books issued by user
void Library::showUserIssuedBooks(int userId) {
    ifstream fin("issued.txt");
    int uid, bid; time_t issueTime;
    cout << "\nYour issued books:\n";
    while (fin >> uid >> bid >> issueTime) {
        if (uid == userId) {
            for (auto& b : books) {
                if (b.id == bid) {
                    char buf[26];
                    ctime_s(buf, sizeof(buf), &issueTime);
                    cout << "ID: " << b.id
                        << " Title: " << b.title
                        << " Author: " << b.author
                        << " Issue Date: " << buf;
                }
            }
        }
    }
    fin.close();
}

// Issue book
void Library::issueBook(int userId) {
    int bid;
    cout << "Enter Book ID to issue: "; cin >> bid;

    bool found = false;
    for (auto& b : books) {
        if (b.id == bid && b.available == 1) {
            b.available = 0;
            found = true;
            break;
        }
    }

    if (!found) { cout << "Book not available!\n"; return; }

    ofstream fout("issued.txt", ios::app);
    fout << userId << " " << bid << " " << time(0) << endl;
    fout.close();

    // Update books.txt
    ofstream bfile("books.txt");
    for (auto& b : books) {
        bfile << b.id << " \"" << b.title << "\" \"" << b.author << "\" " << b.available << endl;
    }
    bfile.close();

    cout << "Book issued successfully!\n";
}

// Return book
void Library::returnBook(int userId) {
    int bid;
    cout << "Enter Book ID to return: "; cin >> bid;

    time_t now = time(0);
    ifstream fin("issued.txt");
    vector<tuple<int, int, time_t>> entries;
    int uid; time_t issueTime; int b;
    while (fin >> uid >> b >> issueTime) {
        entries.push_back({ uid, b, issueTime });
    }
    fin.close();

    ofstream fout("issued.txt");
    bool returned = false;
    for (auto& e : entries) {
        tie(uid, b, issueTime) = e;
        if (uid == userId && b == bid && !returned) {
            int daysLate = difftime(now, issueTime) / (60 * 60 * 24) - 7; // 7 days limit
            int fine = calculateFine(daysLate);
            cout << "You are " << (daysLate > 0 ? daysLate : 0) << " days late. Fine = " << fine << " Rs\n";
            returned = true;
            continue; // remove this entry
        }
        fout << uid << " " << b << " " << issueTime << endl;
    }
    fout.close();

    if (!returned) { cout << "You have not issued this book!\n"; return; }

    // Mark book available
    for (auto& b : books) if (b.id == bid) b.available = 1;

    // Update books.txt
    ofstream bfile("books.txt");
    for (auto& b : books)
        bfile << b.id << " \"" << b.title << "\" \"" << b.author << "\" " << b.available << endl;
    bfile.close();

    cout << "Book returned successfully!\n";
}
