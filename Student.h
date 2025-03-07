#pragma once
#include "User.h"
#include "Book.h"
#include <chrono>

class Library; // Forward declaration

class Student : public User {
public:
    Student(const std::string& id, const std::string& name);
    
    // Inherited pure virtual functions
    bool borrowBook(Book& book) override;
    bool returnBook(Book& book) override;
    void displayMenu(Library& library) override;
    
    // CSV serialization
    std::string toCSV() const override;
    static Student fromCSV(const std::string& csvLine);

private:
    static const int MAX_BOOKS = 3;
    static const int BORROW_DAYS = 15;
    static const int FINE_RATE = 10;
    
    void showBorrowedBooks() const;
};
