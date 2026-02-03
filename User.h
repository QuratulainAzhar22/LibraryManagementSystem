#ifndef USER_H
#define USER_H
#include <string>
using namespace std;

class User {
public:
    int id;
    string username;
    string password;
    string role;

    User() : id(0), username(""), password(""), role("") {}
    User(int i, const string& u, const string& p, const string& r)
        : id(i), username(u), password(p), role(r) {
    }
};

#endif
