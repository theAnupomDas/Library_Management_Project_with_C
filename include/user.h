#pragma once
#include "book.h"
typedef struct UserNode
{
    char username[20];
    char password[20];
    int usercode;
    char timestamp[26];
    int user_point;
    struct UserNode *next;
} UserNode;

UserNode *loadFromFile_userdata(const char *filepath);
void user_traversal(UserNode *head);
void saveToFile_userdata(UserNode *head, const char *filepath);
UserNode *deleteUser(UserNode *head, int usercode);
UserNode *registerUser(UserNode *head);
UserNode* loginUser(UserNode *userlist_head, book *booklist_head);
void showUserDetails(UserNode *user);
void adminLogin(UserNode *userlist_head, book *booklist_head);