#include "Book.h"
#include <sstream>
#include <iomanip>
#include <vector>

Book::Book(const std::string& isbn, const std::string& title,
         const std::string& author, const std::string& publisher,
         int year)
    : ISBN(isbn), title(title), author(author),
      publisher(publisher), publicationYear(year),
      status("Available") {}

// Getters
std::string Book::getISBN() const { return ISBN; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getPublisher() const { return publisher; }
int Book::getPublicationYear() const { return publicationYear; }
std::string Book::getStatus() const { return status; }
std::chrono::system_clock::time_point Book::getDueDate() const { return dueDate; }

// Setters
void Book::setStatus(const std::string& newStatus) {
    if(newStatus == "Available" || newStatus == "Borrowed" || newStatus == "Reserved") {
        status = newStatus;
    }
}

void Book::setDueDate(const std::chrono::system_clock::time_point& date) {
    dueDate = date;
}

// CSV Serialization
std::string Book::toCSV() const {
    std::time_t dueTime = std::chrono::system_clock::to_time_t(dueDate);
    std::stringstream ss;
    ss << ISBN << ","
       << title << ","
       << author << ","
       << publisher << ","
       << publicationYear << ","
       << status << ","
       << dueTime;
    return ss.str();
}

Book Book::fromCSV(const std::string& csvLine) {
    std::stringstream ss(csvLine);
    std::string token;
    std::vector<std::string> tokens;

    while(std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    Book book(
        tokens[0],  // ISBN
        tokens[1],  // Title
        tokens[2],  // Author
        tokens[3],  // Publisher
        std::stoi(tokens[4])  // Year
    );

    book.status = tokens[5];

    if(tokens.size() > 6 && !tokens[6].empty()) {
        std::time_t dueTime = std::stol(tokens[6]);
        book.dueDate = std::chrono::system_clock::from_time_t(dueTime);
    }

    return book;
}

bool Book::isAvailable() const {
    return status == "Available";
}
