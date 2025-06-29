#pragma once
typedef struct book
{
    char book[30]; 
    char author[30];
    char genra[30];
    int totalcount; 
    int avaiablecount; 
    struct book *next;

} book;

book *loadFromFile_booklist(const char *FILE_BOOK);
book *saveToFile_booklist(book *head, const char *FILE_BOOK);
void book_traversal(book *head);
book *addBook(book *head, const char *FILE_BOOK);