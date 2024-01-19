#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <grp.h>



int main(int argc, char **argv) {
    int userGroupId = (int) getgid();
    int userId = getuid();

    printf("User group: %d\n", userGroupId);
    printf("User ID: %d\n", userId);

    return 0;
}
