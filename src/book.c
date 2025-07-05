#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../include/book.h"

book *loadFromFile_booklist(const char *FILE_BOOK)
{
    FILE *file = fopen(FILE_BOOK, "r");
    if (!file)
    {
        printf("File opening error!\n");
        return NULL;
    }

    book *head = NULL;
    book *tail = NULL; // stores the current tail value
    book buffer;

    while (fscanf(file, "%d|%29[^|]|%29[^|]|%29[^|]|%d|%d\n", &buffer.bookcode, buffer.book, buffer.author, buffer.category, &buffer.totalcount, &buffer.avaiablecount) == 6)
    {
        book *newNode = (book *)malloc(sizeof(book));
        if (newNode == NULL)
        {
            printf("allocation failed");
            exit(0);
        }

        *newNode = buffer;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);
    return head;
}
book *saveToFile_booklist(book *head)
{
    FILE *file = fopen("../data/booklist.txt", "w");
    if (!file)
    {
        printf("File opening error!\n");
        return head;
    }
    book *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%d|%s|%s|%s|%d|%d\n", temp->bookcode, temp->book, temp->author, temp->category, temp->totalcount, temp->avaiablecount);
        temp = temp->next;
    }
    fclose(file);
    return head;
}
void book_traversal(book *head)
{
    book *temp = head;

    while (temp != NULL)
    {
        printf("%s, %s, %s, %d, %d\n", temp->book, temp->author, temp->category, temp->totalcount, temp->avaiablecount);
        temp = temp->next;
    }
    printf("NULL");
}

book *addBook(book *head, const char *FILE_BOOK)
{
    book *newBook = (book *)malloc(sizeof(book));
    if (newBook == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }

    printf("Enter Book Name: ");
    scanf("%s", newBook->book);
    printf("Enter Author Name: ");
    scanf("%s", newBook->author);
    printf("Enter Genre: ");
    scanf("%s", newBook->category);
    printf("Enter Total Count: ");
    scanf("%d", &newBook->totalcount);
    newBook->avaiablecount = newBook->totalcount; // Initially, all books are available
    newBook->next = NULL;

    if (head == NULL)
    {
        newBook->bookcode = 100; // Starting book code
        head = newBook;
    }
    else
    {
        book *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        newBook->bookcode = temp->bookcode + 1; // Increment book code for the new book
        temp->next = newBook;
    }

    saveToFile_booklist(head);
    printf("Book added successfully!\n");
    return head;
}

book *deleteBook(book *head, int bookcode)
{
    if (head == NULL)
    {
        printf("No books available to delete.\n");
        return head;
    }

    book *current = head;
    book *previous = NULL;

    while (current != NULL)
    {
        if (current->bookcode == bookcode)
        {
            if (previous == NULL)
            {
                book *temp = head;
                head = head->next;
                free(temp);
            }
            else
            {
                previous->next = current->next;
                free(current);
            }
            printf("Book deleted successfully!\n");
            saveToFile_booklist(head);
            return head;
        }
        previous = current;
        current = current->next;
    }
    printf("Book not found\n");
    return head;
}

void viewBooks(book *booklist_head)
{
    if (booklist_head == NULL)
    {
        printf("No books available.\n");
        return;
    }

    printf("Library Books:\n");
    book *temp = booklist_head;
    while (temp != NULL)
    {
        printf("Bookcode: %d, Book: %s, Author: %s, Category: %s, Total Count: %d, Available: %s\n",
               temp->bookcode, temp->book, temp->author, temp->category, temp->totalcount, temp->avaiablecount ? "Yes" : "No");
        temp = temp->next;
    }
}
book *borrowBook(book *booklist_head, int bookcode, int usercode)
{
    book *temp = booklist_head;
    while (temp != NULL)
    {
        if (temp->bookcode == bookcode)
        {
            if (temp->avaiablecount > 0)
            {
                temp->avaiablecount--;
                // Here you would also need to log the borrowing in a separate structure or file
                char filepath[60];
                sprintf(filepath, "../data/borrowed_book/user_%d_borrowed_book.txt", usercode);
                FILE *file = fopen(filepath, "a");
                if (!file)
                {
                    printf("File opening error!\n");
                    return booklist_head;
                }
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                fprintf(file, "%d|%d|%04d-%02d-%02d %02d:%02d:%02d|%d\n",
                        usercode, bookcode,
                        t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                        t->tm_hour, t->tm_min, t->tm_sec, 0); // 0 for not returned
                fclose(file);
                printf("Book borrowed successfully!\n");
                return booklist_head;
            }
            else
            {
                printf("No copies available for borrowing.\n");
                return booklist_head;
            }
        }
        temp = temp->next;
    }
    printf("Book not found.\n");
    return booklist_head;
}
borrowed_book *readBorrowedBooks(int usercode)
{
    char filepath[60];
    sprintf(filepath, "../data/borrowed_book/user_%d_borrowed_book.txt", usercode);
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        printf("File opening error!\n");
        return NULL;
    }

    borrowed_book *head = NULL;
    borrowed_book *tail = NULL; // stores the current tail value
    borrowed_book buffer;

    while (fscanf(file, "%d|%d|%39[^|]|%d\n", &buffer.usercode, &buffer.bookcode, buffer.timestamp, &buffer.is_returned) == 4)
    {
        borrowed_book *newNode = (borrowed_book *)malloc(sizeof(borrowed_book));
        if (newNode == NULL)
        {
            printf("allocation failed");
            exit(0);
        }

        *newNode = buffer;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(file);
    return head;
}
borrowed_book *saveBorrowedBooks(borrowed_book *head, int usercode)
{
    char filepath[60];
    sprintf(filepath, "../data/borrowed_book/user_%d_borrowed_book.txt", usercode);
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("File opening error!\n");
        return head;
    }
    borrowed_book *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%d|%d|%s|%d\n", temp->usercode, temp->bookcode, temp->timestamp, temp->is_returned);
        temp = temp->next;
    }
    fclose(file);
    return head;
}
