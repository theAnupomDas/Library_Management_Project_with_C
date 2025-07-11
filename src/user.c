#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdint.h>
#include "../include/user.h"
#include "../include/filepaths.h"
#include "../include/dashboard.h"
#include "../include/book.h"
#include "../include/security.h"

UserNode *loadFromFile_userdata(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        printf("File opening error!\n");
        return NULL;
    }

    UserNode *head = NULL;
    UserNode *tail = NULL; // stores the current tail value
    UserNode buffer;

    while (fscanf(file, "%19[^|]|%19[^|]|%d|%25[^|]|%d\n", buffer.username, buffer.password, &buffer.usercode, buffer.timestamp, &buffer.user_point) == 5)
    {
        caesarDecrypt(buffer.username, DEFAULT_SHIFT);
        UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
        if (newNode == NULL)
        {
            printf("allocatioin faild");
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
void user_traversal(UserNode *head)
{
    UserNode *temp = head;

    while (temp != NULL)
    {
        printf("%s,%s,%d,%s\n", temp->username, temp->password, temp->usercode, temp->timestamp);
        temp = temp->next;
    }
    printf("NULL");
}
void saveToFile_userdata(UserNode *head, const char *filepath)
{
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("File opening error!\n");
        return;
    }
    UserNode *temp = head;
    UserNode encryptNode;
    while (temp != NULL)
    {
        encryptNode = *temp; // Copy the current node to encryptNode
        caesarEncrypt(encryptNode.username, DEFAULT_SHIFT);
        fprintf(file, "%s|%s|%d|%s|%d\n",encryptNode.username, temp->password, temp->usercode, temp->timestamp, temp->user_point);
        temp = temp->next;
    }
    fclose(file);
}

UserNode *deleteUser(UserNode *head, int usercode)
{
    printf("Are you sure you want to delete your account? (1 for Yes, 0 for No): ");
    int confirm;
    scanf("%d", &confirm);
    if (confirm != 1)
    {
        printf("Account deletion cancelled.\n");
        return head;
    }
    UserNode *current = head;
    UserNode *previous = NULL;

    while (current)
    {
        if (current->usercode == usercode)
        {
            if (previous == NULL)
            {
                UserNode *temp = head;
                head = head->next;
                free(temp);
            }
            else
            {
                previous->next = current->next;
                free(current);
            }
            printf("Account deleted successfully!\n");
            saveToFile_userdata(head, filepath_userdata);
            return head;
        }
        previous = current;
        current = current->next;
    }
    printf("User not found\n");
    return head;
}

UserNode *registerUser(UserNode *head)
{
    UserNode *newUser = (UserNode *)malloc(sizeof(UserNode));
    if (newUser == NULL)
    {
        printf("Memory allocation failed!\n");
        return head;
    }
    printf("Registering a new user...\n");
    printf("Enter 0 to return to home\n ");
    printf("Enter Username: ");
    scanf("%s", newUser->username);
    if(!strcmp(newUser->username, "0"))
    {
        printf("Returning to home...\n");
        free(newUser);
        initiateProgram();
        return head;
    }
    if (!strcmp(newUser->username, "admin"))
    {
        printf("Username 'admin' is reserved. Please choose a different username.\n");
        free(newUser);
        return head;
    }
    UserNode *temp = head;
    int currentUsercode = 1000;
    UserNode *tail = NULL;
    
    // Check for duplicate username and find the tail
    while (temp != NULL)
    {
        if (strcmp(temp->username, newUser->username) == 0)
        {
            printf("Username already exists. Please choose a different username.\n");
            free(newUser);
            return head;
        }
        currentUsercode = temp->usercode;
        tail = temp;
        temp = temp->next;
    }
    
    printf("Enter Password: ");
    scanf("%s", newUser->password);
    //hash the password
    uint32_t hashed_password = fnv1_32(newUser->password);
    sprintf(newUser->password, "%u", hashed_password);

    // Generate a unique usercode
    newUser->usercode = currentUsercode + 1; 
    time_t now = time(NULL);
    strftime(newUser->timestamp, sizeof(newUser->timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    newUser->user_point = 1; 
    newUser->next = NULL;
    

    if (head == NULL)
    {
        head = newUser;
    }
    else
    {
        tail->next = newUser;
    }
    
    char filepath[60];
    sprintf(filepath, filepath_borrowed_books"/user_%d_borrowed_book.txt", newUser->usercode);
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("File opening error!\n");
        free(newUser);
        return head;
    }
    fclose(file);
    saveToFile_userdata(head, filepath_userdata);
    printf("Registration successful! Your usercode is %d\n", newUser->usercode);
    return head;
}

UserNode* loginUser(UserNode *userlist_head, book *booklist_head)
{
    while (true)
    {
        printf("Enter Username: ");
        char entered_username[30];
        scanf("%s", entered_username);
        if(strcmp(entered_username, "0") == 0)
        {
            printf("Returning to home...\n");
            initiateProgram();
            return NULL;
        }
        // entered_username[strlen(entered_username)] = '\0';
        printf("Username entered: %s\n", entered_username);
        if (strcmp(entered_username, "admin") == 0)
        {
            adminLogin(userlist_head, booklist_head);
        }
        UserNode *temp = userlist_head;
        while (temp != NULL)
        {
            if (strcmp(temp->username, entered_username) == 0)
            {
                printf("User found!\n");
                while (true)
                {
                    printf("Enter Password: ");
                    char entered_password[30];
                    scanf("%s", entered_password);

                    uint32_t hashed_password = fnv1_32(entered_password);
                    sprintf(entered_password, "%u", hashed_password);

                    // entered_password[strlen(entered_password)] = '\0';
                    if (strcmp(temp->password, entered_password) == 0)
                    {
                        printf("Login successful! Welcome %s\n", temp->username);
                        return temp;;
                    }
                    else
                    {
                        printf("Incorrect password. Please try again.\n");
                    }
                }
            }
            temp = temp->next;
        }
        printf("User not found. Please check the username. \nor Enter 0 to return to home\n");
    }
}

void adminLogin(UserNode *userlist_head, book *booklist_head)
{
    printf("Please enter your admin credidentials for Log in\n");
    char admin_password[20];
    printf("Enter Admin Password: ");
    scanf("%s", admin_password);

    uint32_t hashed_admin_password = fnv1_32(admin_password);
    sprintf(admin_password, "%u", hashed_admin_password);

    FILE *admin_file = fopen(filepath_admin, "r");
    if (admin_file == NULL)
    {
        printf("Admin file not found. Please contact the administrator.\n");
        exit(1);
    }
    char stored_password[20];
    fscanf(admin_file, "%19[^\n]", stored_password);
    fclose(admin_file);
    if(strcmp(admin_password, stored_password) != 0)
    {
        printf("Incorrect admin password. Please try again.\n");
        _getch(); // Wait for user input before returning to home
        initiateProgram();
    }
    printf("Admin login successful!\n");
    adminDashboard(userlist_head, booklist_head);

}

void showUserDetails(UserNode *user)
{
    if (user == NULL)
    {
        printf("No user details available.\n");
        return;
    }
    printf("Username: %s\n", user->username);
    printf("Usercode: %d\n", user->usercode);
    printf("Registered at: %s\n", user->timestamp);
}
// End of src/user.c
