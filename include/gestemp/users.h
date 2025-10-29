#ifndef USERS_H
#define USERS_H

typedef enum {
    UserRoleAdmin = 0,
    UserRoleOperator = 1,
    UserRoleViewer = 2
} UserRole;

typedef struct {
    unsigned int id;
    char username[16];
    char password[16];
    UserRole role;
} User;

extern User actualUser;

void usersInit();
int usersAdd();
int usersRemove(unsigned int id);
int usersChangePasswd(unsigned int id);
int usersLogin();

#endif