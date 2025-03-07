#pragma once
#include "User.h"
#include "Book.h"
#include <chrono>

class Library; // Forward declaration

class Faculty : public User {
public:
    Faculty(const std::string& id, const std::string& name);
    
    // Inherited pure virtual functions
    bool borrowBook(Book& book) override;
    bool returnBook(Book& book) override;
    void displayMenu(Library& library) override;
    
    // CSV serialization
    std::string toCSV() const override;
    static Faculty fromCSV(const std::string& csvLine);

private:
    static const int MAX_BOOKS = 5;
    static const int BORROW_DAYS = 30;
    static const int OVERDUE_LIMIT = 60;
    
    bool hasOverdueBooks() const;
    void showBorrowedBooks() const;
    void showAccountStatus() const;
};
