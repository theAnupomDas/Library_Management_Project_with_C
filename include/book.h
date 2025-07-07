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
    char timestamp[40];
    bool is_returned; 
    struct borrowed_book *next;
} borrowed_book;

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