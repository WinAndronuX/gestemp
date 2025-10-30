#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gestemp/users.h>


static User* listUsers = NULL;
static int numUsers = 0;

static int loadUsers() {

    FILE* file = fopen("users.dat", "rb");
    if (file == NULL)
        return false;

    if(listUsers != NULL)
        free(listUsers);

    User* temp = (User*) malloc(sizeof(User));
    numUsers = 0;

    int i = 0;
    while (fread(&i[temp], sizeof(User), 1, file)) {

        temp = realloc(temp, (i + 2) * sizeof(User));

        numUsers++;
    }

    listUsers = temp;

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
        printf("Ingrese un nombre de usuario (max: 15 letras): ");
        scanf("%s", user.username);

        if (!existsUsername(user.username)) break;

        printf("Error: Este nombre de usuario ya existe.\n");
    }

    printf("Ingrese la contraseña (max: 15 letras): ");
    scanf("%s", user.password);

    while (1) {
        int opc;

        printf("Seleccione el rol del usuario:\n1 - Administrador\n 2 - Operador\n3 - Visitante\n\t> ");
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

    FILE* file = fopen("users.dat", "ab");
    if (file == NULL) {
        printf("Error al guardar el usuario.\n");
        return false;
    }

    fwrite(&user, sizeof(User), 1, file);
    fclose(file);

    return true;
}


int usersRemove(unsigned int id) {

    loadUsers();

    User* user = getUserById(id);
    if (user == NULL) {
        printf("Error. Usuario con id invalido\n");
        return false;
    }

    if (strcmp(user->username, "admin") == 0) {
        printf("Error. El usuario admin no puede ser eliminado\n");
        return false;
    }

    User* temp = listUsers;

    listUsers = (User*) realloc(temp, --numUsers * sizeof(User));

    int i, j = 0;
    for (i = 0; i < numUsers; i++) {

        if (i[temp].id == id)
            j = 1;

        i[listUsers] = temp[i+j];
    }

    writeUsers();

    return false;
}

int usersChangePasswd(unsigned int id) {

    User* user = getUserById(id);
    if (user == NULL) {
        printf("Error. Usuario con id invalido\n");
        return false;
    }

    char pass1[16];
    char pass2[16];

    while (1) {

        printf("Ingrese una nueva contraseña (Max: 15): ");
        scanf("%15s", pass1);

        printf("Vuelva a ingresar la contraseǹa: ");
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