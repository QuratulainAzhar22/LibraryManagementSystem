#ifndef BOOK_H
#define BOOK_H
#include <string>
using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    int available; // 1=available, 0=issued

    Book() : id(0), title(""), author(""), available(1) {}
    Book(int i, const string& t, const string& a, int av)
        : id(i), title(t), author(a), available(av) {
    }
};

#endif
