#pragma once
#include "user.h"
int loginUser(UserNode *head);
UserNode *registerUser(UserNode *head);
void initiateProgram();
void userDashboard(UserNode *userlist_head, book * booklist_head);
void clearScreen();