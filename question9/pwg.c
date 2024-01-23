#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "pwg.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>



struct Tuple check_exist_pwd(int userid, char password_file[]){
       
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int add = 0;
    char* user = "";
    char* tmpPassword = "";
    fp = fopen(password_file, "a+");
        while ((read = getline(&line, &len, fp)) != -1) {
            char *user = strtok(line, ":");
            char *tmpPassword = strtok(NULL, ":");

            if (atoi(&user[0])==userid){
                add = 1;
                break;
            }
        }
        if (add == 0) //there is no identical id in the file, we add the new one
        {
            fprintf(fp, "%d", userid);
            fprintf(fp, ":");
            const char* input_pwd = hash_pwd("123");
            fprintf(fp, "%s\n", input_pwd);

            printf("SYSTEM : Your userid and password have been added to the file \n");
            fclose(fp);
            struct Tuple r = {0, input_pwd};
            return r;
        }else // there is an identical id
        {
            const char* input_pwd = hash_pwd("123");
            printf("SYSTEM : You are already registered in the passwd file\n");
            fclose(fp);
            struct Tuple r = {1, input_pwd};
            return r;
        }
        
        
}
//checks is the user has already a password

char* hash_pwd(const char* salt){

    //https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_node/libc_650.html
    char *encrypted_pass;

    encrypted_pass = crypt(getpass("Enter your password :"), salt);

    return encrypted_pass;
}

//encrypts the password



int main(int argc, char **argv){
    
    struct Tuple r = check_exist_pwd(15764, "passwd");
    return 0;

}
//entry point of the program