#define _OPEN_SYS_ITOA_EXT

#define _GNU_SOURCE
#include <string.h>


#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pwg.c"
#include <grp.h>

char password[20] = {0};
unsigned long hashed_password;
// char password_file[] = "/home/admin/passwd"
char password_file[] = "passwd";

struct group *grp;


int main(int argc, char **argv) {

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("File to delete: %s\n", argv[1]);
    if (access(argv[1], F_OK) == 0) {
        // file exists
        printf("Your file exists!\n");

        // check if password is good
        int userId = getuid();
        int access_granted = 0;
        struct Tuple r = check_exist_pwd(userId, password_file);
        if (r.res == 0) {
            printf("SYSTEM : Your userid and password have been added to the file \n");
            access_granted = 1;
        }else if (r.res == 1) {
            printf("SYSTEM : You are already registered in the passwd file\n");
            fp = fopen(password_file, "r");
                while ((read = getline(&line, &len, fp)) != -1) {
                    char *user = strtok(line, ":");
                    char *tmpPassword = strtok(NULL, ":");

                    //printf("User: %s, password: %s, p %s", &user[0], &tmpPassword[0], r.pwd);

                    if (atoi(&user[0]) == userId) {
                        if (strcmp(&tmpPassword[0], r.pwd) == 0) {
                            access_granted = 1;
                            break;
                        }
                    }
                }
            fclose(fp);
        }

        if (access_granted == 1) {
            // good password
            printf("Access granted\n");

            int deleteFile = 0;

            struct stat info_fichier;
            stat(argv[1], &info_fichier);

            long max_groups = sysconf(_SC_NGROUPS_MAX);
            gid_t *group_ids = (gid_t *)malloc(max_groups * sizeof(gid_t));
            int num_groups = getgroups(max_groups, group_ids); 

            for (int i = 0; i < num_groups; i++) {
                if((int)group_ids[i] == (int)info_fichier.st_gid) {
                    deleteFile = 1;
                    break;
                }
            }
            free(group_ids);

            if (deleteFile == 1) {
                printf("Deleting the file\n");
                remove(argv[1]);
            } else {
                printf("You can't delete this file: wrong group\n");
            }


        } else {
            // false password
            printf("Wrong passsword\n");
        }

    } else {
        // file doesn't exists
        printf("The file doesn't exists...\n");
    }
    return 0;
}
