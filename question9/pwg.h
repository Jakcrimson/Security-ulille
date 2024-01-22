#ifndef PWG_H
#define PWG_H
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
struct Tuple    
{
    const int res;
    const char* pwd;
};

struct Tuple check_exist_pwd(int userid, char password_file[]);
//checks is the user has already a password

char* hash_pwd(const char *salt);
//encrypts the password in the file

int main(int argc, char **argv);
//entry point of the program

#endif