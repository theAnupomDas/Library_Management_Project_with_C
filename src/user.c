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

    while (fscanf(file, "%19[^,],%19[^,],%d,%25[^\n]\n", buffer.username, buffer.password, &buffer.usercode, buffer.timestamp) == 4)
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
        fprintf(file, "%s,%s,%d,%s\n", temp->username, temp->password, temp->usercode, temp->timestamp);
        temp = temp->next;
    }
    fclose(file);
}

UserNode *deleteUser(UserNode *head, int usercode)
{
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
            printf("User deleted\n");
            return head;
        }
        previous = current;
        current = current->next;
    }
    printf("User not found\n");
    return head;
}

// UserNode *deleteUserFromList(UserNode *head, int usercode)
// {
//     UserNode *current = head;
//     UserNode *prev = NULL;

//     while (current)
//     {
//         if (current->usercode == usercode)
//         {
//             if (prev == NULL)
//             {
//                 UserNode *temp = head;
//                 head = head->next;
//                 free(temp);
//             }
//             else
//             {
//                 prev->next = current->next;
//                 free(current);
//             }
//             printf("User deleted.\n");
//             return head;
//         }
//         prev = current;
//         current = current->next;
//     }

//     printf("Usercode not found.\n");
//     return head;
// }