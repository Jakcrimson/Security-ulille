#define _OPEN_SYS_ITOA_EXT

#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include "pwg.c"

char password[20] = {0};
unsigned long hashed_password;
// char password_file[] = "/home/admin/passwd"
char password_file[] = "passwd";

struct group *grp;

unsigned long hash(char *str) {
    // https://stackoverflow.com/questions/7666509/hash-function-for-string
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int main(int argc, char **argv) {

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("SYSTEM : File to delete: %s\n", argv[1]);
    if (access(argv[1], F_OK) == 0) {
        // file exists
        printf("SYSTEM : Your file exists!\n");

        // check if password is good
        int userId = getuid();
        int access_granted = 0;
        //printf("Please enter your password: ");
        //scanf("%s", password);
        struct Tuple r = check_exist_pwd(userId, password_file);
        if (r.res == 0) {
            printf("SYSTEM : Your userid and password have been added to the file \n");
        }else if (check_exist_pwd(userId,password_file) == 0){
            printf("SYSTEM : You are already registered in the passwd file\n");

            printf("SYSTEM : Please enter your password: ");
            scanf("%s", password);
            hashed_password = hash(password);

            fp = fopen(password_file, "r");
                while ((read = getline(&line, &len, fp)) != -1) {
                    char *user = strtok(line, ":");
                    char *tmpPassword = strtok(NULL, ":");
                    // printf("User: %s, password: %s", &user[0], &tmpPassword[0]);

                    if (atoi(&user[0]) == userId) {
                        char *ptr;
                        unsigned long ret;
                        ret = strtoul(&tmpPassword[0], &ptr, 10);
                        if (ret == hashed_password) {
                            access_granted = 1;
                            break;
                        }
                    }
                }
            fclose(fp);
        }
        if (access_granted == 1) {
            // good password
            printf("SYSTEM : Access granted\n");

            int userGroupId = (int) getgid();
            grp = getgrgid(*argv[1]);

            if (grp->gr_gid == userGroupId) {
                printf("SYSTEM : Deleting the file");
            } else {
                printf("SYSTEM : You can't delete this file: wrong group");
            }


        } else {
            // false password
            printf("SYSTEM : Wrong passsword\n");
        }

    } else {
        // file doesn't exists
        printf("SYSTEM : The file doesn't exists...\n");
    }

    // printf("my group id is %d\n", (int) getgid());   // pour trouver group id de l'utilisateur
    return 0;
}


