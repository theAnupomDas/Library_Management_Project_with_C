#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../include/book.h"


book *loadFromFile_booklist(const char *FILE_BOOK )
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

    while (fscanf(file, "%29[^,],%29[^,],%29[^,],%d,%d\n", buffer.book, buffer.author, buffer.genra, &buffer.totalcount, &buffer.avaiablecount) == 5)
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
//const char *str ="booklist.txt"; 
book *saveToFile_booklist(book *head, const char *FILE_BOOK)
{
    FILE *file = fopen(FILE_BOOK, "w");
    if (!file)
    {
        printf("File opening error!\n");
        return head;
    }
    book *temp = head;
    while (temp != NULL)
    {
        fprintf(file, "%s,%s,%s,%d,%d\n", temp->book, temp->author, temp->genra, temp->totalcount, temp->avaiablecount);
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
        printf("%s,%s,%s,%d,%d\n", temp->book, temp->author, temp->genra, temp->totalcount, temp->avaiablecount);
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
    scanf("%s", newBook->genra);
    printf("Enter Total Count: ");
    scanf("%d", &newBook->totalcount);
    newBook->avaiablecount = newBook->totalcount; // Initially, all books are available
    newBook->next = NULL;

    if (head == NULL)
    {
        head = newBook;
    }
    else
    {
        book *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newBook;
    }

    saveToFile_booklist(head, FILE_BOOK);
    printf("Book added successfully!\n");
    return head;
}