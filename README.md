# Library Management System in C 📚

![Language](https://img.shields.io/badge/language-C-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

A comprehensive, file-based library automation system developed in **C**. Unlike simple array-based projects, this system utilizes **Singly and Doubly Linked Lists** for efficient memory usage and dynamic data handling.

## 🚀 Key Features

* **Dynamic Data Structures:** Uses **Linked Lists** to manage Students, Authors, and Books dynamically without fixed array limits.
* **Complex Data Modeling:** Implements `structs` with pointers to handle relationships (e.g., a Book having multiple Copies with different statuses).
* **Penalty System:** Automatically calculates overdue fines based on dates. If a book is returned late (>15 days), the student's library score is deducted.
* **Persistence (File I/O):** All data (Students, Books, Borrows, Authors) is saved to and loaded from CSV files (`.csv`), ensuring data isn't lost when the program closes.
* **Smart Search:** Functions to locate books by ISBN or Name, and Students by ID.

## 🛠️ Technical Implementation

* **Memory Management:** Extensive use of `malloc`, `realloc`, and `free` to handle dynamic lists and prevent memory leaks.
* **Relations:** Manages Many-to-Many relationships between Books and Authors using a relational mapping structure.
* **Time Manipulation:** Uses `<time.h>` to fetch current dates and calculate the difference between borrow and return dates.

## 📂 File Structure

* `main.c`: The core source code containing the menu logic and all helper functions.
* `*.csv`: The system automatically generates/reads these files to store data:
    * `ogrenciler.csv` (Students)
    * `kitaplar.csv` (Books inventory)
    * `yazarlar.csv` (Authors)
    * `odunctakip.csv` (Borrowing logs)

## 💻 How to Run

1. **Clone the repository:**
   ```bash
   git clone [https://github.com/busesln/library-management-system-c.git](https://github.com/busesln/library-management-system-c.git)
