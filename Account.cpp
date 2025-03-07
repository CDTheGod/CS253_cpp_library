#include "Account.h"
#include "User.h"
#include "Book.h"
#include "Student.h"
#include <chrono>
#include <algorithm>
#include <sstream>
#include <iostream>

Account::Account() : fines(0.0) {}

void Account::addBook(const Book& book) {
    borrowedBooks.push_back(book);
}

bool Account::removeBook(const std::string& isbn) {
    auto it = std::remove_if(borrowedBooks.begin(), borrowedBooks.end(),
        [&](const Book& b) { return b.getISBN() == isbn; });
    
    bool removed = (it != borrowedBooks.end());
    borrowedBooks.erase(it, borrowedBooks.end());
    return removed;
}

void Account::calculateFines(User* user) {
    auto now = std::chrono::system_clock::now();
    fines = 0.0;

    // Student-specific fine calculation
    if (auto student = dynamic_cast<Student*>(user)) {
        for (const auto& book : borrowedBooks) {
            if (book.getStatus() == "Borrowed" && now > book.getDueDate()) {
                auto overdue = std::chrono::duration_cast<std::chrono::hours>(
                    now - book.getDueDate());
                fines += (overdue.count() / 24) * 10.0;
            }
        }
    }
    // Faculty has no fines by policy
}

void Account::payFines() {
    fines = 0.0;
}

std::string Account::saveToCSV() const {
    std::stringstream ss;
    ss << fines << ",";
    
    for(size_t i = 0; i < borrowedBooks.size(); ++i) {
        ss << borrowedBooks[i].getISBN();
        if(i != borrowedBooks.size() - 1) ss << ";";
    }
    
    return ss.str();
}

void Account::loadFromCSV(const std::string& csvData) {
    std::stringstream ss(csvData);
    std::string token;
    
    // Parse fines
    std::getline(ss, token, ',');
    fines = std::stod(token);
    
    // Parse borrowed books
    std::string isbns;
    std::getline(ss, isbns);
    
    std::stringstream iss(isbns);
    while(std::getline(iss, token, ';')) {
        borrowedBooks.emplace_back(token, "", "", "", 0); // Minimal book data
    }
}

const std::vector<Book>& Account::getBorrowedBooks() const {
    return borrowedBooks;
}

double Account::getFines() const {
    return fines;
}

bool Account::hasOverdueBooks() const {
    auto now = std::chrono::system_clock::now();
    return std::any_of(borrowedBooks.begin(), borrowedBooks.end(),
        [&](const Book& b) { return now > b.getDueDate(); });
}
