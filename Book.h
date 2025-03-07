#pragma once
#include <string>
#include <chrono>
#include <ctime>

class Book {
private:
    std::string ISBN;
    std::string title;
    std::string author;
    std::string publisher;
    int publicationYear;
    std::string status;
    std::chrono::system_clock::time_point dueDate;

public:
    // Constructor
    Book(const std::string& isbn, const std::string& title, 
         const std::string& author, const std::string& publisher, 
         int year);

    // Getters
    std::string getISBN() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getPublisher() const;
    int getPublicationYear() const;
    std::string getStatus() const;
    std::chrono::system_clock::time_point getDueDate() const;

    // Setters
    void setStatus(const std::string& newStatus);
    void setDueDate(const std::chrono::system_clock::time_point& date);

    // CSV Serialization
    std::string toCSV() const;
    static Book fromCSV(const std::string& csvLine);

    // Utility
    bool isAvailable() const;
};
