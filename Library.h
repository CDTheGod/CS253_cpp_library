#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Book.h"
#include "User.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"

class Library {
private:
    std::vector<Book> books;
    std::vector<std::unique_ptr<User>> users;

    void initializeDefaultData();
    void loadBooks(const std::string& filename);
    void loadUsers(const std::string& filename);
    void saveBooks(const std::string& filename) const;
    void saveUsers(const std::string& filename) const;

public:
    Library();
    
    // Book management
    void addBook(const Book& book);
    bool removeBook(const std::string& isbn);
    Book* findBook(const std::string& isbn);
    
    // User management
    void addUser(std::unique_ptr<User> user);
    bool removeUser(const std::string& userID);
    User* findUser(const std::string& userID);
    
    // File operations
    void loadFromFiles();
    void saveToFiles() const;
    
    // Getters
    const std::vector<Book>& getAllBooks() const { return books; }
    const std::vector<std::unique_ptr<User>>& getAllUsers() const { return users; }
};
