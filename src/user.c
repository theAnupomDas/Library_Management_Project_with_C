#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "../include/user.h"

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

    while (fscanf(file, "%19[^|]|%19[^|]|%d|%25[^\n]\n", buffer.username, buffer.password, &buffer.usercode, buffer.timestamp) == 4)
    {
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
    while (temp != NULL)
    {
        fprintf(file, "%s|%s|%d|%s\n", temp->username, temp->password, temp->usercode, temp->timestamp);
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
            saveToFile_userdata(head, "../data/userdata.txt");
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
    printf("Enter Username: ");
    scanf("%s", newUser->username);
    UserNode *temp = head;
    int currentUsercode = 1000;
    UserNode *tail;
    while (temp != NULL)
    {
        if (strcmp(temp->username, newUser->username) == 0)
        {
            printf("Username already exists. Please choose a different username.\n");
            free(newUser);
            return head;
        }
        currentUsercode = temp ? temp->usercode : 1000;
        tail = temp;
        temp = temp->next;
    }
    printf("Enter Password: ");
    scanf("%s", newUser->password);

    // Generate a unique usercode
    newUser->usercode = ++currentUsercode; // Random usercode for simplicity
    time_t now = time(NULL);
    strftime(newUser->timestamp, sizeof(newUser->timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    // time_t currentTime;
    // time(&currentTime);
    // newUser->timestamp = ctime(&currentTime);
    tail->next = newUser;
    newUser->next = NULL;
    char filepath[60];
    sprintf(filepath, "../data/borrowed_book/user_%d_borrowed_book.txt", newUser->usercode);
    FILE *file = fopen(filepath, "w");
    if (!file)
    {
        printf("File opening error!\n");
        free(newUser);
        return head;
    }
    fclose(file);
    saveToFile_userdata(head, "../data/userdata.txt");
    printf("Registration successful! Your usercode is %d\n", newUser->usercode);
    free(newUser);
    return head;
}

int loginUser(UserNode *head)
{
    while (true)
    {
        printf("Enter Username: ");
        char entered_username[30];
        scanf("%s", entered_username);
        // entered_username[strlen(entered_username)] = '\0';
        printf("Username entered: %s\n", entered_username);
        UserNode *temp = head;
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
                    // entered_password[strlen(entered_password)] = '\0';
                    if (strcmp(temp->password, entered_password) == 0)
                    {
                        printf("Login successful! Welcome %s\n", temp->username);
                        return temp->usercode;
                    }
                    else
                    {
                        printf("Incorrect password. Please try again.\n");
                    }
                }
            }
            temp = temp->next;
        }
        printf("User not found. Please check the username.\n");
    }
}
// End of src/user.c
