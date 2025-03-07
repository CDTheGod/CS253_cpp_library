#include "Faculty.h"
#include "Library.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <algorithm>

using namespace std::chrono;

Faculty::Faculty(const std::string& id, const std::string& name) 
    : User(id, name) {}

bool Faculty::borrowBook(Book& book) {
    // Check maximum book limit
    if(account->getBorrowedBooks().size() >= MAX_BOOKS) {
        std::cout << "Error: Faculty borrowing limit (5 books) reached\n";
        return false;
    }
    
    // Check for severe overdue books
    if(hasOverdueBooks()) {
        std::cout << "Cannot borrow - has books overdue >60 days\n";
        return false;
    }
    
    // Check book availability
    if(!book.isAvailable()) {
        std::cout << "Book not available\n";
        return false;
    }
    
    // Set due date (30 days from now)
    auto now = system_clock::now();
    auto dueDate = now + hours(24 * BORROW_DAYS);
    book.setDueDate(dueDate);
    book.setStatus("Borrowed");
    
    account->addBook(book);
    std::cout << "Book borrowed successfully!\n";
    return true;
}

bool Faculty::returnBook(Book& book) {
    // Remove from account
    if(!account->removeBook(book.getISBN())) {
        std::cout << "Error: Book not in your borrowed list\n";
        return false;
    }
    
    // Check and warn about severe overdue
    auto now = system_clock::now();
    if(now > book.getDueDate()) {
        auto overdue = duration_cast<hours>(now - book.getDueDate());
        int days = overdue.count() / 24;
        
        if(days > OVERDUE_LIMIT) {
            std::cout << "Warning: This book was overdue by " << days << " days\n";
        }
    }
    
    // Update book status
    book.setStatus("Available");
    std::cout << "Book returned successfully\n";
    return true;
}

void Faculty::displayMenu(Library& library) {
    int choice;
    do {
        std::cout << "\n=== FACULTY MENU ===\n"
                  << "1. Borrow Book\n"
                  << "2. Return Book\n"
                  << "3. View Borrowed Books\n"
                  << "4. Check Account Status\n"
                  << "5. Logout\n"
                  << "Choice: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1:{
                std::string isbn;
                std::cout << "Enter ISBN of the book to borrow: ";
                std::cin >> isbn;

                Book* book = library.findBook(isbn);
                if (book) {
                    if (borrowBook(*book)) {
                        std::cout << "Book borrowed successfully!\n";
                    } else {
                        std::cout << "Unable to borrow the book.\n";
                    }
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 2:{
                std::string isbn;
                std::cout << "Enter ISBN of the book to return: ";
                std::cin >> isbn;

                Book* book = library.findBook(isbn);
                if (book) {
                    if (returnBook(*book)) {
                        std::cout << "Book returned successfully!\n";
                    } else {
                        std::cout << "Unable to return the book.\n";
                    }
                } else {
                    std::cout << "Book not found.\n";
                }
                break;
            }
            case 3:
                showBorrowedBooks();
                break;
            case 4:
                showAccountStatus();
                break;
        }
    } while(choice != 5);
}

std::string Faculty::toCSV() const {
    std::stringstream ss;
    ss << "FACULTY," 
       << userID << ","
       << name << ","
       << account->saveToCSV();
    return ss.str();
}

Faculty Faculty::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;
    
    while(getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    Faculty faculty(tokens[1], tokens[2]);
    faculty.account->loadFromCSV(tokens[3]);
    return faculty;
}

bool Faculty::hasOverdueBooks() const {
    auto now = system_clock::now();
    return std::any_of(account->getBorrowedBooks().begin(),
                       account->getBorrowedBooks().end(),
                       [&](const Book& b) {
                           return now > b.getDueDate() + hours(24 * OVERDUE_LIMIT);
                       });
}

void Faculty::showAccountStatus() const {
    std::cout << "\nAccount Status:\n"
              << "- Borrowed Books: " << account->getBorrowedBooks().size() << "/5\n"
              << "- Overdue Status: " << (hasOverdueBooks() ? "BLOCKED" : "Active") << "\n";
}

void Faculty::showBorrowedBooks() const {
    const auto& books = account->getBorrowedBooks();
    if(books.empty()) {
        std::cout << "No books currently borrowed\n";
        return;
    }
    
    auto now = system_clock::now();
    std::cout << "Borrowed Books (" << books.size() << "/5):\n";
    
    for(const auto& book : books) {
        auto dueTime = system_clock::to_time_t(book.getDueDate());
        auto overdue = duration_cast<hours>(now - book.getDueDate());
        int daysOverdue = overdue.count() / 24;
        
        std::cout << "- " << book.getTitle() 
                  << " (Due: " << std::put_time(std::localtime(&dueTime), "%F")
                  << "), Overdue: " << daysOverdue << " days\n";
    }
}
