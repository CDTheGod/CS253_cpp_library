# CS253_cpp_library

A C++ implementation of a Library Management System for CS253 course.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Requirements](#requirements)
4. [Installation](#installation)
5. [Running the Program](#running-the-program)
6. [Usage](#usage)
7. [Troubleshooting](#troubleshooting)

## Introduction
This project provides a comprehensive library management system using C++. It supports user roles such as students, faculty, and librarians, each with distinct privileges.

## Features
- **User Management**: Supports multiple user types (students, faculty, librarians).
- **Book Management**: Allows adding, removing, and borrowing books.
- **Fine Calculation**: Automatically calculates fines for overdue books.
- **CSV Persistence**: Saves and loads data from CSV files.

## Requirements
- **Compiler**: C++17 compliant compiler (e.g., GCC, Clang).
- **Operating System**: Windows, Linux, or macOS.
- **Dependencies**: No external libraries required.

## Installation
1. Clone the repository:

```
git clone https://github.com/CDTheGod/CS253_cpp_library.git
```


2. Navigate to the project directory:

```
cd CS253_cpp_library
```



## Running the Program
1. Compile the program:

```
g++ -std=c++17 *.cpp -o library_system
```


2. Run the executable:

```
./library_system
```



## Usage
1. **Login Menu**:
- Enter user ID to log in.
- Available roles: student, faculty, librarian.
2. **Student Menu**:
- Borrow books.
- Return books.
- View borrowed books.
- Check fines.
3. **Faculty Menu**:
- Borrow books (no fines).
- Return books.
- View borrowed books.
4. **Librarian Menu**:
- Manage books (add/remove).
- Manage users (add/remove).

## Troubleshooting
- **Compilation Errors**: Ensure all files are in the same directory.
- **File Not Found Errors**: Verify that `books.csv` and `users.csv` exist.
- **Memory Errors**: Run with memory sanitizers to detect leaks.

---

### Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

### License
[MIT License](https://choosealicense.com/licenses/mit/)
