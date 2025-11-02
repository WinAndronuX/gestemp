#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gestemp/users.h>
#include <gestemp/utils.h>
#include <listview/listview.h>


static User* listUsers = NULL;
static int numUsers = 0;

static int loadUsers() {

    FILE* file = fopen("users.dat", "rb");
    if (file == NULL)
        return false;

    if (listUsers != NULL)
        free(listUsers);

    listUsers = NULL;
    numUsers = 0;
    User tempUser;

    while (fread(&tempUser, sizeof(User), 1, file)) {

        User* temp = (User*) realloc(listUsers, (numUsers + 1) * sizeof(User));
        if (temp == NULL) {
            fclose(file);
            return false;
        }
        listUsers = temp;

        listUsers[numUsers] = tempUser;
        numUsers++;
    }

    fclose(file);

    return true;
}

static int writeUsers() {

    FILE* file = fopen("users.dat", "wb");
    if (file == NULL)
        return false;

    fwrite(listUsers, sizeof(User), numUsers, file);

    fclose(file);

    return true;
}

static int existsUsername(char username[16]) {

    int i;
    for(i = 0; i < numUsers; i++)
        if (strcmp(i[listUsers].username, username) == 0)
            return true;

    return false;
}

static int existUserId(unsigned int id) {

    int i;
    for(i = 0; i < numUsers; i++)
        if (i[listUsers].id == id)
            return true;

    return false;
}

static unsigned int getUserId() {

    unsigned int id = 0;

    int i;
    for(i = 0; i < numUsers; i++)
        if(i[listUsers].id > id)
            id = i[listUsers].id;

    return ++id;
}

static User* getUserById(unsigned int id) {

    int i;
    for(i = 0; i < numUsers; i++)
        if (i[listUsers].id == id)
            return &i[listUsers];

    return NULL;
}

User actualUser;

int usersAdd() {

    loadUsers();

    User user;

    while (1) {
        printf("Ingrese un nombre de usuario (max: 15 letras):\n%c ", PROMPT);
        scanf("%s", user.username);

        if (!existsUsername(user.username)) break;

        printf("Error: Este nombre de usuario ya existe.\n");
    }

    printf("Ingrese la contraseña (max: 15 letras):\n%c ", PROMPT);
    scanf("%s", user.password);

    while (1) {
        int opc;

        printf("Seleccione el rol del usuario:\n\t1 - Administrador\n\t2 - Operador\n\t3 - Visitante\n> ");
        scanf("%d", &opc);

        switch (opc) {
        case 1:
            user.role = UserRoleAdmin;
            break;
        case 2:
            user.role = UserRoleOperator;
            break;
        case 3:
            user.role = UserRoleViewer;
            break;
        default:
            printf("Opcion no valida\n");
            continue;
        }

        break;
    }

    user.id = getUserId();

    User* temp = (User*) realloc(listUsers, (numUsers + 1) * sizeof(User));
    temp[numUsers].id = user.id;
    strcpy(temp[numUsers].username, user.username);
    strcpy(temp[numUsers].password, user.password);
    temp[numUsers].role = user.role;
    listUsers = temp;

    numUsers++;

    writeUsers();

    return true;
}


int usersRemove() {

    if (actualUser.role != UserRoleAdmin) {
        printf("Accion no permitida. Permisos insuficientes");
        return false;
    }

    loadUsers();

    unsigned int id;
    printf("Ingrese el id del usuario a eliminar:\n%c ", PROMPT);
    scanf("%ud", &id);

    User* user;
    int indexToRemove = -1;

    for (int i = 0; i < numUsers; i++) {
        if (i[listUsers].id == id) {
            user = &listUsers[i];
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        printf("Error. Id de usuario invalido\n");
        return false;
    }

    if (strcmp(user->username, "admin") == 0) {
        printf("Error. El usuario admin no puede ser eliminado\n");
        return false;
    }

    for (int i = indexToRemove; i < numUsers - 1; i++) {
        listUsers[i] = listUsers[i+1];
    }

    numUsers--;

    listUsers = (User*) realloc(listUsers, numUsers * sizeof(User));

    writeUsers();

    return false;
}

int usersChangePasswd() {

    unsigned int id = actualUser.id;
    if (actualUser.role == UserRoleAdmin) {
        printf("Ingrese el id del usuario al que quiere cambiar la contraseña:\n%c ", PROMPT);
        scanf("%ud", &id);
    }

    User* user = getUserById(id);
    if (user == NULL) {
        printf("Error. Usuario con id invalido\n");
        return false;
    }

    char pass1[16];
    char pass2[16];

    while (1) {

        printf("Ingrese una nueva contraseña (Max: 15):\n%c ", PROMPT);
        scanf("%15s", pass1);

        printf("Vuelva a ingresar la contraseǹa:\n%c ", PROMPT);
        scanf("%15s", pass2);

        if (strcmp(pass1, pass2) == 0) break;

        printf("Error. Las contraseñas no coinciden\n");
    }

    strcpy(user->password, pass1);

    writeUsers();

    return true;
}

int usersLogin() {

    char username[16], passwd[16];

    printf("Usuario: ");
    scanf("%15s", username);

    printf("Contraseña: ");
    scanf("%15s", passwd);

    User* user = NULL;

    int i;
    for (i = 0; i < numUsers; i++)
        if(strcmp(i[listUsers].username, username) == 0)
            user = &i[listUsers];

    if (user == NULL || strcmp(user->password, passwd) != 0)
        return false;

    actualUser = *user;

    return true;
}


void usersInit() {

    bool loaded = loadUsers();

    if(!loaded) {

        listUsers = (User*) malloc(sizeof(User));
        listUsers[0] = (User) {0, "admin", "admin", UserRoleAdmin};
        numUsers = 1;

        writeUsers();
    }

}

void usersPrint() {

    ListView* lv = listviewCreate("Usuarios", 3);

    listviewHeadAdd(lv, "Id", 3);
    listviewHeadAdd(lv, "Nombre", 20);
    listviewHeadAdd(lv, "Rol", 9);

    int i;
    for (i = 0; i < numUsers; i++) {

        char sId[5];
        sprintf(sId, "%d", listUsers[i].id);

        listviewAdd(lv, sId);

        listviewAdd(lv, listUsers[i].username);

        char role[10];
        switch (listUsers[i].role) {
            case UserRoleAdmin:
                strcpy(role, "Admin");
                break;
            case UserRoleOperator:
                strcpy(role, "Operador");
                break;
            case UserRoleViewer:
                strcpy(role, "Visitante");
                break;
        }

        listviewAdd(lv, role);
    }

    listviewFootPrint(lv);
}