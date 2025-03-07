#include "Student.h"
#include "Library.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

using namespace std::chrono;

Student::Student(const std::string& id, const std::string& name) 
    : User(id, name) {}

bool Student::borrowBook(Book& book) {
    // Check borrowing limits
    if(account->getBorrowedBooks().size() >= MAX_BOOKS) {
        std::cout << "Error: You can't borrow more than 3 books\n";
        return false;
    }
    
    // Check outstanding fines
    if(account->getFines() > 0) {
        std::cout << "Clear your fines (₹" << account->getFines() 
                  << ") before borrowing\n";
        return false;
    }
    
    // Check book availability
    if(!book.isAvailable()) {
        std::cout << "Book not available\n";
        return false;
    }
    
    // Set due date (15 days from now)
    auto now = system_clock::now();
    auto dueDate = now + hours(24 * BORROW_DAYS);
    book.setDueDate(dueDate);
    book.setStatus("Borrowed");
    
    account->addBook(book);
    std::cout << "Book borrowed successfully!\n";
    return true;
}

bool Student::returnBook(Book& book) {
    // Remove from account
    if(!account->removeBook(book.getISBN())) {
        std::cout << "Error: Book not in your borrowed list\n";
        return false;
    }
    
    // Calculate fines
    auto now = system_clock::now();
    if(now > book.getDueDate()) {
        auto overdue = duration_cast<hours>(now - book.getDueDate());
        int days = overdue.count() / 24;
        account->calculateFines(this);
        std::cout << "Overdue by " << days << " days. Fine: ₹" 
                  << (days * FINE_RATE) << "\n";
    }
    
    // Update book status
    book.setStatus("Available");
    std::cout << "Book returned successfully\n";
    return true;
}

void Student::displayMenu(Library& library) {
    int choice;
    do {
        std::cout << "\n=== STUDENT MENU ===\n"
                  << "1. Borrow Book\n"
                  << "2. Return Book\n"
                  << "3. View Borrowed Books\n"
                  << "4. Check Fines\n"
                  << "5. Pay Fines\n"
                  << "6. Logout\n"
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
                std::cout << "Current fines: ₹" << account->getFines() << "\n";
                break;
            case 5:
                account->payFines();
                std::cout << "Fines cleared!\n";
                break;
        }
    } while(choice != 6);
}

std::string Student::toCSV() const {
    std::stringstream ss;
    ss << "STUDENT," 
       << userID << "," 
       << name << "," 
       << account->saveToCSV();
    return ss.str();
}

Student Student::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;
    
    while(getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    
    Student student(tokens[1], tokens[2]);
    student.account->loadFromCSV(tokens[3]);
    return student;
}

void Student::showBorrowedBooks() const {
    const auto& books = account->getBorrowedBooks();
    if(books.empty()) {
        std::cout << "No books currently borrowed\n";
        return;
    }
    
    auto now = system_clock::now();
    std::cout << "Borrowed Books (" << books.size() << "/3):\n";
    
    for(const auto& book : books) {
        auto dueTime = system_clock::to_time_t(book.getDueDate());
        std::cout << "- " << book.getTitle() 
                  << " (Due: " << std::put_time(std::localtime(&dueTime), "%F %R")
                  << ")\n";
    }
}
