#include "Library.h"
#include "Student.h"
#include "Faculty.h"
#include "Librarian.h"
#include <iostream>
#include <limits>

void displayMainMenu() {
    std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===\n"
              << "1. Login\n"
              << "2. Exit\n"
              << "Choice: ";
}

void handleLogin(Library& library) {
    std::string userID;
    std::cout << "Enter User ID: ";
    std::cin >> userID;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if(User* user = library.findUser(userID)) {
        if(auto student = dynamic_cast<Student*>(user)) {
            student->displayMenu(library);
        }
        else if(auto faculty = dynamic_cast<Faculty*>(user)) {
            faculty->displayMenu(library);
        }
        else if(auto librarian = dynamic_cast<Librarian*>(user)) {
            librarian->displayMenu(library);
        }        
    } else {
        std::cout << "User not found!\n";
    }
}

int main() {
    Library library;
    library.loadFromFiles();

    int choice;
    do {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1:
                handleLogin(library);
                break;
            case 2:
                std::cout << "Saving data and exiting...\n";
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while(choice != 2);
    
    library.saveToFiles();
    std::cout<<"Thank you for using the library management system"<<std::endl;
    return 0;
}
