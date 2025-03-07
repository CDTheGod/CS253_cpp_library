#include "Library.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm> 
#include <parallel/compatibility.h>
namespace fs = std::filesystem;

Library::Library() {
    if(!fs::exists("books.csv") || !fs::exists("users.csv")) {
        initializeDefaultData();
    }
}

void Library::initializeDefaultData() {
    // Add 10 default books
    books = {
        Book("978-0321563842", "C++ Primer", "Stanley Lippman", "Addison-Wesley", 2012),
        Book("978-0201633610", "Design Patterns", "Erich Gamma", "Addison-Wesley", 1994),
        Book("978-0132350884", "Clean Code", "Robert Martin", "Prentice Hall", 2008),
        Book("978-0134685991", "Effective Modern C++", "Scott Meyers", "O'Reilly", 2014),
        Book("978-0262033848", "Introduction to Algorithms", "Thomas Cormen", "MIT Press", 2009),
        Book("978-1491903996", "The Go Programming Language", "Alan Donovan", "Addison-Wesley", 2015),
        Book("978-0135957059", "The Pragmatic Programmer", "David Thomas", "Addison-Wesley", 2019),
        Book("978-1617292231", "Grokking Algorithms", "Aditya Bhargava", "Manning", 2016),
        Book("978-0137081073", "Code Complete", "Steve McConnell", "Microsoft Press", 2004),
        Book("978-0596007126", "Head First Design Patterns", "Eric Freeman", "O'Reilly", 2004)
    };

    // Create default users
    users.push_back(std::make_unique<Student>("STU001", "Alice Johnson"));
    users.push_back(std::make_unique<Student>("STU002", "Bob Smith"));
    users.push_back(std::make_unique<Student>("STU003", "Charlie Brown"));
    users.push_back(std::make_unique<Student>("STU004", "Diana Prince"));
    users.push_back(std::make_unique<Student>("STU005", "Edward Norton"));
    
    users.push_back(std::make_unique<Faculty>("FAC001", "Dr. Smith"));
    users.push_back(std::make_unique<Faculty>("FAC002", "Prof. Jones"));
    users.push_back(std::make_unique<Faculty>("FAC003", "Dr. Williams"));
    
    users.push_back(std::make_unique<Librarian>("LIB001", "Admin"));
    
    saveToFiles();
}

void Library::addBook(const Book& book) {
    if(!findBook(book.getISBN())) {
        books.push_back(book);
    }
}

bool Library::removeBook(const std::string& isbn) {
    auto it = std::remove_if(books.begin(), books.end(),
        [&](const Book& b) { return b.getISBN() == isbn; });
    
    bool removed = (it != books.end());
    books.erase(it, books.end());
    return removed;
}

Book* Library::findBook(const std::string& isbn) {
    for(auto& book : books) {
        if(book.getISBN() == isbn) {
            return &book;
        }
    }
    return nullptr;
}

void Library::addUser(std::unique_ptr<User> user) {
    if(!findUser(user->getID())) {
        users.push_back(std::move(user));
    }
}

bool Library::removeUser(const std::string& userID) {
    auto it = std::remove_if(users.begin(), users.end(),
        [&](const std::unique_ptr<User>& u) { return u->getID() == userID; });
    
    bool removed = (it != users.end());
    users.erase(it, users.end());
    return removed;
}

User* Library::findUser(const std::string& userID) {
    for(auto& user : users) {
        if(user->getID() == userID) {
            return user.get();
        }
    }
    return nullptr;
}

void Library::loadFromFiles() {
    loadBooks("books.csv");
    loadUsers("users.csv");
}

void Library::loadBooks(const std::string& filename) {
    std::ifstream file(filename);
    if(!file) return;

    std::string line;
    std::getline(file, line); // Skip header
    
    while(std::getline(file, line)) {
        books.push_back(Book::fromCSV(line));
    }
}

void Library::loadUsers(const std::string& filename) {
    users.clear();
    std::cout << "Loading users from " << filename << "...\n";
    
    std::ifstream file(filename);
    if(!file) return;

    std::string line;
    std::getline(file, line); // Skip header
    
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string role;
        std::getline(ss, role, ',');
        
        if(role == "STUDENT") {
            users.push_back(std::make_unique<Student>(Student::fromCSV(line)));
        }
        else if(role == "FACULTY") {
            users.push_back(std::make_unique<Faculty>(Faculty::fromCSV(line)));
        }
        else if (role == "LIBRARIAN") {
            users.push_back(std::make_unique<Librarian>(Librarian::fromCSV(line)));
        }          
    }
}

void Library::saveToFiles() const {
    saveBooks("books.csv");
    saveUsers("users.csv");
    #ifdef _WIN32
        Sleep(1000); // Windows-specific (1 second delay)
    #endif
}

void Library::saveBooks(const std::string& filename) const {
    std::ofstream file(filename);
    file << "ISBN,Title,Author,Publisher,Year,Status,DueDate\n";
    for(const auto& book : books) {
        file << book.toCSV() << "\n";
    }
}

void Library::saveUsers(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "❌ Error: Failed to open " << filename << " for writing!\n";
        return;
    }
    file << "Role,UserID,Name,AccountData\n";
    for (const auto& user : users) {
        file << user->toCSV() << "\n";
    }

    if (!file.good()) { // Check if writes succeeded
        std::cerr << "❌ Error: Failed to write to " << filename << "!\n";
    }
    file.flush(); // Force write to disk
    file.close(); // Explicit close
}

