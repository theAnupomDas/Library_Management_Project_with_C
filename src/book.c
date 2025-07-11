#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../include/book.h"
#include "../include/filepaths.h"

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
    FILE *file = fopen(filepath_booklist, "w");
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
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                FILE *requested_book_file = fopen(filepath_requested_books, "a");
                if (!requested_book_file)
                {
                    printf("File opening error!\n");
                    return booklist_head;
                }
                fprintf(requested_book_file, "%d|%d|%04d-%02d-%02d %02d:%02d:%02d\n",
                        usercode, bookcode, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                        t->tm_hour, t->tm_min, t->tm_sec);
                fclose(requested_book_file);
                    
                char filepath[60];
                sprintf(filepath, filepath_borrowed_books "/user_%d_borrowed_book.txt", usercode);
                FILE *file = fopen(filepath, "a");
                if (!file)
                {
                    printf("File opening error!\n");
                    return booklist_head;
                }
                fprintf(file, "%d|%d|%04d-%02d-%02d %02d:%02d:%02d|0|null|null|0|null\n",
                        usercode, bookcode, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                        t->tm_hour, t->tm_min, t->tm_sec);
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
book *returnBook(book *booklist_head, int bookcode, int usercode)
{
    borrowed_book *borrowed_books = loadFromFile_BorrowedBooks(usercode);
    if (borrowed_books == NULL)
    {
        printf("No borrowed books found for user %d.\n", usercode);
        return booklist_head;
    }
    borrowed_book *temp = borrowed_books;
    while (temp != NULL)
    {
        if (temp->bookcode == bookcode && !temp->is_returned)
        {
            temp->is_returned = 1;
            // Update the book's available count
            book *book_temp = booklist_head;
            while (book_temp != NULL)
            {
                if (book_temp->bookcode == bookcode)
                {
                    book_temp->avaiablecount++;
                    break;
                }
                book_temp = book_temp->next;
            }

            printf("Book returned successfully!\n");
            saveToFile_BorrowedBooks(borrowed_books, usercode);
            free(borrowed_books);
            return booklist_head;
        }
        temp = temp->next;
    }
    printf("Book not found in borrowed books or already returned.\n");
    free(borrowed_books);
    return booklist_head;
}
borrowed_book *loadFromFile_BorrowedBooks(int usercode)
{
    char filepath[60];
    sprintf(filepath, filepath_borrowed_books "/user_%d_borrowed_book.txt", usercode);
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        printf("File opening error!\n");
        return NULL;
    }

    borrowed_book *head = NULL;
    borrowed_book *tail = NULL; // stores the current tail value
    borrowed_book buffer;

    while (fscanf(file, "%d|%d|%39[^|]|%d|%39[^|]|%39[^|]|%d|%39[^\n]",
           &buffer.usercode, &buffer.bookcode, buffer.request_timestamp, &buffer.is_returned,
           buffer.issue_timestamp, buffer.due_timestamp, &buffer.is_returned, buffer.return_timestamp) == 8)
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
borrowed_book *saveToFile_BorrowedBooks(borrowed_book *head, int usercode)
{
    char filepath[60];
    sprintf(filepath, filepath_borrowed_books "/user_%d_borrowed_book.txt", usercode);
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("File opening error!\n");
        return head;
    }
    borrowed_book *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%d|%d|%s|%d|%s|%s|%d|%s\n",
                temp->usercode, temp->bookcode, temp->request_timestamp, temp->is_issued,
                temp->issue_timestamp, temp->due_timestamp, temp->is_returned, temp->return_timestamp);
        temp = temp->next;
    }
    fclose(file);
    return head;
}
void showBorrowedBooks(borrowed_book *borrowed_book_head, book *booklist_head, SelectBookStatus book_status)
{
    if (borrowed_book_head == NULL)
    {
        printf("No borrowed books found.\n");
        return;
    }
    borrowed_book *temp = borrowed_book_head;
    if (book_status == NOT_RETURNED)
    {
        printf("Borrowed Books:\n");
        temp = borrowed_book_head;
        while (temp != NULL)
        {
            if (temp->is_returned == 0)
            {
                printf("Bookcode: %d, Book: %s, Issued Time: %s\n",
                       temp->bookcode, getBookName(booklist_head, temp->bookcode), temp->issue_timestamp);
            }

            temp = temp->next;
        }
    }
    else if (book_status == RETURNED)
    {
        printf("Returned Books:\n");
        temp = borrowed_book_head;
        while (temp != NULL)
        {
            if (temp->is_returned == 1)
            {
                printf("Bookcode: %d, Book: %s, Borrowed Time: %s, Returned: Yes, Returned time: %s\n",
                       temp->bookcode, getBookName(booklist_head, temp->bookcode), temp->issue_timestamp, temp->return_timestamp);
            }

            temp = temp->next;
        }
    }
    else if (book_status == ALL)
    {
        printf("All Borrowed Books:\n");
        temp = borrowed_book_head;
        while (temp != NULL)
        {
            printf("Bookcode: %d, Book: %s, Borrowed Time: %s, Returned: %s, Returned time: %s\n",
                   temp->bookcode, getBookName(booklist_head, temp->bookcode), temp->issue_timestamp, temp->is_returned ? "Yes" : "No", temp->return_timestamp);
            temp = temp->next;
        }
    }
    else
    {
        printf("Invalid book status.\n");
        return;
    }
}
const char *getBookName(book *booklist_head, int bookcode)
{
    book *temp = booklist_head;
    while (temp != NULL)
    {
        if (temp->bookcode == bookcode)
        {
            return temp->book;
        }
        temp = temp->next;
    }
    return "Book not found";
}
void viewRequestedBooks( requested_book *requested_books_head)
{
    if (requested_books_head == NULL)
    {
        printf("No requested books found.\n");
        return;
    }

    printf("Requested Books:\n");
    requested_book *temp = requested_books_head;
    while (temp != NULL)
    {
        printf("Usercode: %d, Bookcode: %d, Request Timestamp: %s\n", temp->usercode, temp->bookcode, temp->request_timestamp);
        temp = temp->next;
    }
}

requested_book* loadFromFIle_requested_books()
{
    FILE *file = fopen(filepath_requested_books, "r");
    if (!file)
    {
        printf("File opening error!\n");
        return NULL;
    }

    requested_book buffer;
    requested_book *head = NULL;
    requested_book *tail = NULL; // stores the current tail value
    while (fscanf(file, "%d|%d|%39[^\n]", &buffer.usercode, &buffer.bookcode, buffer.request_timestamp) == 3)
    {
        requested_book *newNode = (requested_book *)malloc(sizeof(requested_book));
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
void saveToFile_requested_books(requested_book *head)
{
    FILE *file = fopen(filepath_requested_books, "w");
    if (!file)
    {
        printf("File opening error!\n");
        return;
    }
    requested_book *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%d|%d|%s\n", temp->usercode, temp->bookcode, temp->request_timestamp);
        temp = temp->next;
    }
    fclose(file);
}