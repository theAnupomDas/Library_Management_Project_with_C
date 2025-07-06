#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "../include/book.h"
#include "../include/dashboard.h"
#include "../include/user.h"
#include "../include/filepaths.h"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void userDashboard(UserNode *userlist_head, book * booklist_head)
{
    printf("Please enter your credidentials for Log in\n");
    int current_user = loginUser(userlist_head);
    label1:
    clearScreen();
    printf("Welcome to the User Dashboard!\n");
    printf("Logged in successfully with usercode: %d\n", current_user);
    printf("1. view all books\n"
           "2. logout and return to home\n"
           "3. logout and exit\n"
           "4. delete my account\n"
           "5. borrow a book\n"
           "Please select an option: ");
    int user_choice;
    scanf("%d", &user_choice);
    if (user_choice == 1)
    {
        if (booklist_head == NULL)
        {
            printf("No books available.\n");
        }
        else
        {
            viewBooks(booklist_head);
        }
        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label1;
    }
    else if (user_choice == 2)
    {
        printf("Logging out...\n");
        printf("Press any key to return to home...\n");
        _getch();
        initiateProgram();
    }
    else if (user_choice == 3)
    {
        printf("Logging out and exiting...\n");
        free(userlist_head);
        free(booklist_head);
        exit(0);
    }
    else if (user_choice == 4)
    {
        userlist_head = deleteUser(userlist_head, current_user);
        printf("Press any key to return to home... ");
        _getch();
        initiateProgram();
    }
    else if (user_choice == 5)
    {
        if (booklist_head == NULL)
        {
            printf("No books available to borrow.\n");
        }
        else
        {
            int bookcode;
            printf("Enter the book code you want to borrow: ");
            scanf("%d", &bookcode);
            booklist_head = borrowBook(booklist_head, bookcode, current_user);
            borrowed_book *borrowed_books = readBorrowedBooks(current_user);
            if (borrowed_books != NULL)
            {
                saveBorrowedBooks(borrowed_books, current_user);
                free(borrowed_books);
            }
            saveToFile_booklist(booklist_head);
        }

        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label1;
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
        userDashboard(userlist_head, booklist_head);
    }
}
void initiateProgram()
{
    clearScreen();
    printf("Welcome to the Library Management System!\n");

    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. About\n");
    printf("4. Exit\n");
    printf("Please select an option: ");
    int choice;
    scanf("%d", &choice);
    UserNode *userlist_head = loadFromFile_userdata(filepath_userdata);
    book *booklist_head = loadFromFile_booklist(filepath_booklist);

    if (choice == 1)
    {
        clearScreen();
        userDashboard(userlist_head, booklist_head);
    }
    else if (choice == 2)
    {
        clearScreen();
        userlist_head = registerUser(userlist_head);
        printf("Press any key to return to home...\n");
        _getch();
        initiateProgram();
    }
    else if (choice == 3)
    {
        clearScreen();
        printf("About the Library Management System:\n");
        printf("This is a Library Management System developed by Anupom Das and supervised by A S M Delowar Hossain, Associate Professor,"
               "Department of Computer Science and Engineering, Mawlana Bhashani Science and Technology University.\n"
               "This program allows users to register, login, and manage books in the library.\n"
               "This program is under development. Some Features may not work properly.\n");
        printf("\n\n Press any key to return to home...\n");
        _getch(); // Wait for user input before returning to home
        initiateProgram();
    }
    else if (choice == 4)
    {
        printf("Exiting the program. Thank you for being with us!\n");
        free(userlist_head);
        free(booklist_head);
        exit(0);
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
        initiateProgram();
    }

}
