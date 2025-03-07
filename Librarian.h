#pragma once
#include "User.h"
#include "Library.h"

class Library; // Add this at the top of Librarian.h

class Librarian : public User {
public:
    Librarian(const std::string& id, const std::string& name);
    
    // Disable book borrowing/returning
    bool borrowBook(Book& book) override { return false; }
    bool returnBook(Book& book) override { return false; }
    
    void displayMenu(Library& library) override;
    
    // CSV serialization
    std::string toCSV() const override;
    static Librarian fromCSV(const std::string& csvLine);

private:
    void showBookManagement(Library& library) const;
    void showUserManagement(Library& library) const;
};
