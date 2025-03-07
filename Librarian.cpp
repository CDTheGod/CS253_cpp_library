#include "Librarian.h"
#include <iostream>
#include <limits>
#include <sstream>

Librarian::Librarian(const std::string& id, const std::string& name)
    : User(id, name) {}


void Librarian::displayMenu(Library& library){
    int choice;
    do {
        std::cout << "\n=== LIBRARIAN ADMIN MENU ===\n"
                  << "1. Manage Books\n"
                  << "2. Manage Users\n"
                  << "3. Logout\n"
                  << "Choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice) {
            case 1:
                showBookManagement(library);
                break;
            case 2:
                showUserManagement(library);
                break;
        }
    } while(choice != 3);
}

void Librarian::showBookManagement(Library& library) const {
    int choice;
    do {
        std::cout << "\n=== BOOK MANAGEMENT ===\n"
                  << "1. Add Book\n"
                  << "2. Remove Book\n"
                  << "3. List All Books\n"
                  << "4. Back\n"
                  << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: {
                std::string isbn, title, author, publisher;
                int year;
                
                std::cout << "Enter ISBN: ";
                std::getline(std::cin, isbn);
                std::cout << "Enter Title: ";
                std::getline(std::cin, title);
                std::cout << "Enter Author: ";
                std::getline(std::cin, author);
                std::cout << "Enter Publisher: ";
                std::getline(std::cin, publisher);
                std::cout << "Enter Publication Year: ";
                std::cin >> year;
                
                library.addBook(Book(isbn, title, author, publisher, year));
                std::cout << "Book added successfully!\n";
                break;
            }
            case 2: {
                std::string isbn;
                std::cout << "Enter ISBN to remove: ";
                std::getline(std::cin, isbn);
                
                if(library.removeBook(isbn)) {
                    std::cout << "Book removed successfully\n";
                } else {
                    std::cout << "Book not found\n";
                }
                break;
            }
            case 3: {
                const auto& books = library.getAllBooks();
                std::cout << "\nLibrary Catalog (" << books.size() << " books):\n";
                for(const auto& book : books) {
                    std::cout << "- " << book.getTitle() << " [" << book.getISBN() << "]\n";
                }
                break;
            }
        }
    } while(choice != 4);
}

void Librarian::showUserManagement(Library& library) const {
    int choice;
    do {
        std::cout << "\n=== USER MANAGEMENT ===\n"
                  << "1. Add User\n"
                  << "2. Remove User\n"
                  << "3. List All Users\n"
                  << "4. Back\n"
                  << "Choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: {
                std::string id, name, role;
                std::cout << "Enter User ID: ";
                std::getline(std::cin, id);
                std::cout << "Enter Name: ";
                std::getline(std::cin, name);
                std::cout << "Enter Role (student/faculty/librarian): ";
                std::getline(std::cin, role);
                
                std::unique_ptr<User> newUser;
                if(role == "student") {
                    newUser = std::make_unique<Student>(id, name);
                } else if(role == "faculty") {
                    newUser = std::make_unique<Faculty>(id, name);
                } else if(role == "librarian") {
                    newUser = std::make_unique<Librarian>(id, name);
                } else {
                    std::cout << "Invalid role\n";
                    break;
                }
                
                library.addUser(std::move(newUser));
                std::cout << "User added successfully\n";
                break;
            }
            case 2: {
                std::string id;
                std::cout << "Enter User ID to remove: ";
                std::getline(std::cin, id);
                
                if(library.removeUser(id)) {
                    std::cout << "User removed successfully\n";
                } else {
                    std::cout << "User not found\n";
                }
                break;
            }
            case 3: {
                const auto& users = library.getAllUsers();
                std::cout << "\nRegistered Users (" << users.size() << "):\n";
                for(const auto& user : users) {
                    std::cout << "- " << user->getName() << " [" << user->getID() << "]\n";
                }
                break;
            }
        }
    } while(choice != 4);
}

std::string Librarian::toCSV() const {
    return "LIBRARIAN," + userID + "," + name + "," + account->saveToCSV();
}

Librarian Librarian::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;
    
    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    // Create Librarian object through make_unique
    Librarian librarian(tokens[1], tokens[2]);
    librarian.account->loadFromCSV(tokens[3]);
    return librarian;
}

