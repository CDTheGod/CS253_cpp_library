#pragma once
#include <vector>
#include <string>
#include "Book.h"

class User; // Forward declaration

class Account {
private:
    std::vector<Book> borrowedBooks;
    double fines;

public:
    Account();
    
    // Book management
    void addBook(const Book& book);
    bool removeBook(const std::string& isbn);
    
    // Fine operations
    void calculateFines(User* user);
    void payFines();
    
    // CSV handling
    std::string saveToCSV() const;
    void loadFromCSV(const std::string& csvData);
    
    // Getters
    const std::vector<Book>& getBorrowedBooks() const;
    double getFines() const;
    
    // Utility
    bool hasOverdueBooks() const;
};
