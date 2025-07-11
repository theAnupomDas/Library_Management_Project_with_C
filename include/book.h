#pragma once
#include <stdbool.h>
typedef struct book
{
    int bookcode; 
    char book[50]; 
    char author[50];
    char category[30];
    int totalcount; 
    int avaiablecount; 
    struct book *next;

} book;
typedef struct borrowed_book
{
    int usercode; 
    int bookcode; 
    char request_timestamp[40];
    bool is_issued;
    char issue_timestamp[40];
    char due_timestamp[40];
    bool is_returned; 
    char return_timestamp[40];
    struct borrowed_book *next;
} borrowed_book;
typedef struct requested_book
{
    int usercode; 
    int bookcode; 
    char request_timestamp[40];
    struct requested_book *next;
} requested_book;
typedef enum {
    NOT_RETURNED = 0,
    RETURNED = 1,
    ALL = 2

} SelectBookStatus;

book *loadFromFile_booklist(const char *FILE_BOOK);
book *saveToFile_booklist(book *head);
void book_traversal(book *head);
book *addBook(book *head, const char *FILE_BOOK);
void viewBooks(book *booklist_head);
book *deleteBook(book *head, int bookcode);
book *borrowBook(book *booklist_head, int bookcode, int usercode);
book *returnBook(book *booklist_head, int bookcode, int usercode);
borrowed_book *loadFromFile_BorrowedBooks(int usercode); 
borrowed_book *saveToFile_BorrowedBooks(borrowed_book *head, int usercode);
void showBorrowedBooks(borrowed_book *borrowed_book_head, book *booklist_head, SelectBookStatus book_status);
const char* getBookName(book *booklist_head, int bookcode);
void viewRequestedBooks(requested_book *requested_books_head);
requested_book* loadFromFIle_requested_books();
void saveToFile_requested_books(requested_book *head);
requested_book* issueRequestedBook(requested_book *requested_books_head, int usercode, int bookcode);
void updateRequestedBookStatus(int usercode, int bookcode);