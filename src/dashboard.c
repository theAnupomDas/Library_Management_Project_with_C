#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "../include/book.h"
#include "../include/dashboard.h"
#include "../include/filepaths.h"

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void adminDashboard(UserNode *userlist_head, book *booklist_head)
{
    label_admin_dashboard:
    printf("Welcome to the Admin Dashboard!\n");
    printf("1. view all books\n"
           "2. add a book\n"
           "3. delete a book\n"
           "4. view all users\n"
           "5. logout and return to home\n"
           "6. logout and exit\n"
           "7. update password\n"
           "8. view requested books\n");

    printf("Please select an option: ");
    int admin_choice;
    scanf("%d", &admin_choice);
    // view all books
    if (admin_choice == 1)
    {
        if (booklist_head == NULL)
        {
            printf("No books available.\n");
        }
        else
        {
            viewBooks(booklist_head);
        }
        printf("\nPress any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    // add a book
    else if (admin_choice == 2)
    {
        booklist_head = addBook(booklist_head, filepath_booklist);
        printf("\nPress any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    // delete a book
    else if (admin_choice == 3)
    {
        if (booklist_head == NULL)
        {
            printf("No books available to delete.\n");
        }
        else
        {
            viewBooks(booklist_head);
            int bookcode;
            printf("Enter the book code you want to delete:\n"
                   "or Press 0 to return to admin dashboard: \n"
                   "Select an option: ");
            scanf("%d", &bookcode);
            if (bookcode == 0)
            {
                adminLogin(userlist_head, booklist_head);
            }
            booklist_head = deleteBook(booklist_head, bookcode);
            saveToFile_booklist(booklist_head);
        }
        printf("\nPress any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    // view all users
    else if (admin_choice == 4)
    {
        user_traversal(userlist_head);
        printf("\nPress any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    // logout and return to home
    else if (admin_choice == 5)
    {
        printf("Logging out...\n");
        printf("Press any key to return to home...\n");
        _getch();
        initiateProgram();
    }
    // logout and exit
    else if (admin_choice == 6)
    {
        printf("Logging out and exiting...\n");
        free(userlist_head);
        free(booklist_head);
        exit(0);
    }
    else if (admin_choice == 7)
    {
        clearScreen();
        char new_password[20];
        printf("Enter new admin password: ");
        scanf("%s", new_password);
        char new_password_confirm[20];
        printf("Confirm new admin password: ");
        scanf("%s", new_password_confirm);
        if (strcmp(new_password, new_password_confirm) != 0)
        {
            printf("Passwords do not match. Please try again.\n");
            _getch(); // Wait for user input before returning to home
            goto label_admin_dashboard;
        }
        FILE *admin_file = fopen(filepath_admin, "w");
        if (admin_file == NULL)
        {
            printf("Error opening admin file.\n");
            exit(1);
        }
        fprintf(admin_file, "%s\n", new_password);
        fclose(admin_file);
        printf("Admin password updated successfully!\n");
        printf("Press any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    else if (admin_choice == 8)
    {
        clearScreen();
        requested_book *requested_books_head = loadFromFIle_requested_books();
        viewRequestedBooks(requested_books_head);
        free(requested_books_head);
        printf("Press any key to return to admin dashboard...\n");
        _getch();
        adminLogin(userlist_head, booklist_head);
    }
    else
    {
        printf("Invalid choice. Please try again.\n");
        adminLogin(userlist_head, booklist_head);
    }
}

void userDashboard(UserNode *userlist_head, book *booklist_head)
{
    printf("Please enter your credidentials for Log in\n");
    UserNode *current_user = loginUser(userlist_head, booklist_head);
label_user_dashboard:
    clearScreen();
    printf("Welcome to the User Dashboard!\n");
    printf("Logged in successfully. username: %s\n", current_user->username);
    printf("1. view all books\n"
           "2. borrow a book\n"
           "3. view my borrowed books\n"
           "4. return a book\n"
           "5. logout and return to home\n"
           "6. logout and exit\n"
           "7. about\n"
           "8. delete my account\n"

           "Please select an option: ");
    int user_choice;
    scanf("%d", &user_choice);
    // view all books
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
        printf("\nPress 1 to borrow a book\n"
               "Press 2 to return to dashboard\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            goto label_borrow_book;
        }
        else if (choice == 2)
        {
            goto label_user_dashboard;
        }
        else
        {
            printf("Invalid choice..\n");
        }
        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label_user_dashboard;
    }
    // borrow a book
    else if (user_choice == 2)
    {
        if (booklist_head == NULL)
        {
            printf("No books available to borrow.\n");
        }
        else
        {
            printf("Available books:\n");
            viewBooks(booklist_head);
        label_borrow_book:
            int bookcode;
            printf("Enter the book code you want to borrow:\n"
                   "or Press 0 to return to dashboard: \n"
                   "Select an option: ");
            scanf("%d", &bookcode);
            if (bookcode == 0)
            {
                goto label_user_dashboard;
            }
            booklist_head = borrowBook(booklist_head, bookcode, current_user->usercode);
            borrowed_book *borrowed_books = loadFromFile_BorrowedBooks(current_user->usercode);
            if (borrowed_books != NULL)
            {
                saveToFile_BorrowedBooks(borrowed_books, current_user->usercode);
                free(borrowed_books);
            }
            saveToFile_booklist(booklist_head);
        }

        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label_user_dashboard;
    }
    // view my borrowed books
    else if (user_choice == 3)
    {
        borrowed_book *borrowed_books = loadFromFile_BorrowedBooks(current_user->usercode);
        showBorrowedBooks(borrowed_books, booklist_head, ALL);
        free(borrowed_books);
        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label_user_dashboard;
    }
    // return a book
    else if (user_choice == 4)
    {
        borrowed_book *borrowed_books = loadFromFile_BorrowedBooks(current_user->usercode);
        showBorrowedBooks(borrowed_books, booklist_head, NOT_RETURNED);
        free(borrowed_books);

        printf("Enter the book code you want to return:\n"
               "or Press 0 to return to dashboard: \n"
               "Select an option: ");
        int bookcode;
        scanf("%d", &bookcode);
        booklist_head = returnBook(booklist_head, bookcode, current_user->usercode);
        printf("\nPress any key to return to dashboard...\n");
        _getch();
        goto label_user_dashboard;
    }
    // logout and return to home
    else if (user_choice == 5)
    {
        printf("Logging out...\n");
        printf("Press any key to return to home...\n");
        _getch();
        initiateProgram();
    }
    // logout and exit
    else if (user_choice == 6)
    {
        printf("Logging out and exiting...\n");
        free(userlist_head);
        free(booklist_head);
        exit(0);
    }
    // about
    else if (user_choice == 7)
    {
        clearScreen();
        showUserDetails(current_user);
        printf("Press any key to return to dashboard... ");
        _getch();
        goto label_user_dashboard;
    }
    // delete my account
    else if (user_choice == 8)
    {
        userlist_head = deleteUser(userlist_head, current_user->usercode);
        printf("Press any key to return to home... ");
        _getch();
        initiateProgram();
    }
    // invalid choice
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
    free(userlist_head);
    free(booklist_head);
}
