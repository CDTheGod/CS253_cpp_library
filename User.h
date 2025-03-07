#pragma once
#include <string>
#include "Account.h"

class Book; // Forward declaration
class Library; // Forward declaration

class User {
protected:
    std::string userID;
    std::string name;
    Account* account;

public:
    // Constructor/Destructor
    User(const std::string& id, const std::string& name);
    virtual ~User();

    // Pure virtual functions
    virtual bool borrowBook(Book& book) = 0;
    virtual bool returnBook(Book& book) = 0;
    virtual void displayMenu(Library& library) = 0;

    // Accessors
    std::string getID() const { return userID; }
    std::string getName() const { return name; }
    Account* getAccount() const { return account; }

    // CSV Serialization
    virtual std::string toCSV() const = 0;
    void fromCSV(const std::string& csvData);
};
